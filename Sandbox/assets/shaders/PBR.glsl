#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

struct PointLight
{
	vec3 Pos;
	vec3 Colour;
	float LuminousPower; //Lumens
};

out VS_Out
{
	vec3 v_FragPos;
	vec2 v_TexCoord;
	vec3 v_ViewPos;
	vec3 v_TestNormal;
	float v_TestFloat;
	mat3 invTBNMatrix;
	PointLight pointLights[2];
} vs_out;

uniform mat4 m_ViewProjection;
uniform mat4 m_Transform;
uniform mat3 m_NormalMatrix;
uniform vec3 ViewPos;
uniform int pointLightCount;
uniform PointLight pointLights[2];

void main()
{
	// Gram-Schmidt Process TODO: Move to CMFCompiler
	vec3 temp_Tangent = normalize(a_Tangent - (dot(a_Normal, a_Tangent) * a_Normal));
	vec3 temp_Bitangent = cross(a_Normal, temp_Tangent);
	
	// mat3 invTBNMatrix = transpose(mat3(normalize(m_NormalMatrix * a_Tangent), normalize(m_NormalMatrix * a_Bitangent), normalize(m_NormalMatrix * a_Normal)));
	mat3 invTBNMatrix = transpose(mat3(normalize(m_NormalMatrix * temp_Tangent), normalize(m_NormalMatrix * temp_Bitangent), normalize(m_NormalMatrix * a_Normal)));

	for (uint i = 0; i < pointLightCount; i++)
	{
		vs_out.pointLights[i].Pos = invTBNMatrix * pointLights[i].Pos;
		vs_out.pointLights[i].Colour = pointLights[i].Colour;
		vs_out.pointLights[i].LuminousPower = pointLights[i].LuminousPower;
	}

	//vs_out.lightVec = invTBNMatrix * (pointLights[0].Pos - (m_Transform * vec4(a_Position, 1.0f)).xyz);
	vs_out.v_TexCoord = a_TexCoord;
	vec4 vertexPos = m_Transform * vec4(a_Position, 1.0f);
	vs_out.v_FragPos = invTBNMatrix * vertexPos.xyz;
	vs_out.v_ViewPos = invTBNMatrix * ViewPos;
	vs_out.v_TestNormal = temp_Bitangent;
	vs_out.v_TestFloat =  dot(a_Bitangent, temp_Bitangent);
	vs_out.invTBNMatrix = invTBNMatrix;
	gl_Position = m_ViewProjection * vertexPos;
}


#type fragment
#version 410 core

#define PI 3.1415

#define Fresnel F_Schlick
#define N_Distr D_GGX
#define V_Func V_SmithCorrelated

float D_GGX(float NdotH, float roughness);
vec3 F_Schlick(float VdotH, vec3 f0, float f90);
float V_SmithCorrelated(float NdotL, float NdotV, float roughness);

struct Material
{
	sampler2D AlbedoMap;
	sampler2D NormalMap;
	sampler2D MetallicMap;
	sampler2D RoughnessMap;

	vec3  AmbientColour;
	vec3  DiffuseColour;
	float MetallicFactor;
	float RoughnessFactor;
};

struct PointLight
{
	vec3 Pos;
	vec3 Colour;
	float LuminousPower; //Lumens
};

in VS_Out
{
	vec3 v_FragPos;
	vec2 v_TexCoord;
	vec3 v_ViewPos;
	vec3 v_TestNormal;
	float v_TestFloat;
	mat3 invTBNMatrix;
	PointLight pointLights[2];
} fs_in;

out vec4 Colour;

uniform Material material;
uniform int pointLightCount;
uniform samplerCube cubeMap;

void main()
{
	vec3 normal = normalize(texture(material.NormalMap, fs_in.v_TexCoord).rgb * 2.0 - 1.0);
	vec3 albedo = texture(material.AlbedoMap, fs_in.v_TexCoord).rgb;
	float re_roughness = clamp(pow(texture(material.RoughnessMap, fs_in.v_TexCoord).r, 2.2), 0.0003, 1.0f);
	float metalness = texture(material.MetallicMap, fs_in.v_TexCoord).r;
	// re_roughness = 0.001;
	// normal = vec3(0.0f, 0.0f, 1.0f);
	vec3 viewDir = normalize(fs_in.v_ViewPos - fs_in.v_FragPos);

	vec3 cubeMapColour = texture(cubeMap,transpose(fs_in.invTBNMatrix) * reflect(-viewDir, normal)).xyz;
	
	Colour = vec4(0);
	for (int i = 0; i < pointLightCount; i++)
	{
		vec3 lightDir = normalize(fs_in.pointLights[i].Pos - fs_in.v_FragPos);
		vec3 h = normalize(viewDir + lightDir);
		float NdotV = dot(normal, viewDir);
		float NdotL = dot(normal, lightDir);
		float NdotH = dot(normal, h);
		float VdotH = dot(viewDir, h);
		
		if (NdotL >= 0)
			{
				float lightStrength = (1.0f / pow(length(fs_in.pointLights[i].Pos - fs_in.v_FragPos), 2)) * fs_in.pointLights[i].LuminousPower;
				vec3 lightColour = fs_in.pointLights[i].Colour;
				
				vec3 F = Fresnel(VdotH, vec3(0.04), 1.0f) * (1.0f - metalness) + metalness * Fresnel(VdotH, vec3(0.9f), 1.0f);
				float D = N_Distr(NdotH, re_roughness);//clamp(N_Distr(NdotH, re_roughness), 0.0f, 1.0f);
				float V = V_Func(NdotL, NdotV, re_roughness);//clamp(V_Func(NdotL, NdotV, 1), 0.0f, 1.0f);
				// vec3 f_spec = metalness * albedo * F * D * V + (1-metalness) * F * D * V;
				// vec3 f_spec = metalness * F * albedo * V + (1-metalness) * vec3(F * V);
				vec3 f_spec = F * V * D;
				vec3 f_diffuse = (1 - metalness) * (1 - F) * albedo / PI;
				Colour += vec4((f_spec * mix(vec3(1.0f), albedo, metalness) + f_diffuse) * lightStrength * lightColour * NdotL, 1.0f);
				// Colour += vec4(f_spec + f_diffuse, lightStrength);
			}
	}
	Colour += vec4(albedo * 0.1, 1.0f);
	Colour = vec4(texture(cubeMap, transpose(fs_in.invTBNMatrix) * reflect(-viewDir, vec3(0.0f, 0.0f, 1.0f))).xyz * albedo, 1.0f);
}



//FUNCTIONS

float D_GGX(float NdotH, float roughness)
{
	float a = roughness * roughness;
	float k = (NdotH * NdotH) * (a - 1) + 1;
	return (a / (PI * k * k));
}

vec3 F_Schlick(float VdotH, vec3 f0, float f90)
{
	return (f0 + (vec3(f90) - f0) * pow(1.0f - VdotH, 5));
}

float V_SmithCorrelated(float NdotL, float NdotV, float roughness)
{
	// float a = roughness;
	// float GGXL = NdotV * (NdotL * (1.0f - a) + a);
	// float GGXV = NdotL * (NdotV * (1.0f - a) + a);
	// return 0.5 / mix(2 * NdotL * NdotV, NdotL + NdotV, a);

	float a2 = roughness * roughness;
    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0f - a2) + a2);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0f - a2) + a2);
    return (0.5 / (GGXV + GGXL));

	// float a2 = roughness * roughness;
    // float GGXV = (NdotV * NdotV) * NdotL;
    // float GGXL = (NdotL * NdotL) * NdotV;
    // return 0.5f / (GGXL * GGXV);

	// float a = roughness;
    // float GGXV = NdotL * (NdotV * (1.0 - a) + a);
    // float GGXL = NdotV * (NdotL * (1.0 - a) + a);
    // return 0.5 / (GGXV + GGXL);
	
	//return (0.5f / (GGXL + GGXV));
}
