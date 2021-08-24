#type vertex
#version 450 core

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
	mat3 invTBNMatrix = transpose(mat3(normalize(m_NormalMatrix * temp_Tangent),
									   normalize(m_NormalMatrix * temp_Bitangent),
									   normalize(m_NormalMatrix * a_Normal)));

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
	vs_out.invTBNMatrix = invTBNMatrix;
	gl_Position = m_ViewProjection * vertexPos;
}


#type fragment
#version 450 core

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
	mat3 invTBNMatrix;
	PointLight pointLights[2];
} fs_in;

out vec4 Colour;

uniform Material material;
uniform int pointLightCount;
uniform samplerCube irradianceMap;
uniform samplerCube radianceMap;
uniform sampler2D specularLUT;

const float gamma = 2.2;
const int MAX_RADIANCE_LODS = 4;
// const float gamma = 1.0;

void main()
{
	vec3 normal = normalize(texture(material.NormalMap, fs_in.v_TexCoord).rgb * 2.0 - 1.0);
	vec3 albedo = texture(material.AlbedoMap, fs_in.v_TexCoord).rgb;
	albedo = pow(albedo, vec3(gamma));

	float roughness = texture(material.RoughnessMap, fs_in.v_TexCoord).r;
	float alpha = roughness * roughness;

	float metalness = texture(material.MetallicMap, fs_in.v_TexCoord).r;

	vec3 viewDir = normalize(fs_in.v_ViewPos - fs_in.v_FragPos);
	vec3 reflDir = normalize(transpose(fs_in.invTBNMatrix) * reflect(-viewDir, normal));

	float NdotV = dot(normal, viewDir);

	vec3 f0 = mix(vec3(0.04f), albedo, metalness);
	vec3 Fn = Fresnel(NdotV, f0, 1.0f);
	vec3 Ks = Fn;
	vec3 Kd = (vec3(1.0f) - Ks) * (1.0f - metalness);

	vec3 ambient;
	 // AMBIENT
		vec3 irradianceMapColour = texture(irradianceMap, transpose(fs_in.invTBNMatrix) * normal).rgb;
		vec3 diffuse = irradianceMapColour * albedo;

		vec3 radianceMapColour = textureLod(radianceMap, reflDir, MAX_RADIANCE_LODS * roughness).rgb;
		vec2 brdfLUTval = texture(specularLUT, vec2(max(NdotV, 0.0f), roughness)).rg;
		// vec2 brdfLUTval = texture(specularLUT, vec2(1.0f, roughness)).rg;
		vec3 specular = radianceMapColour * (Fn * brdfLUTval.x + brdfLUTval.y);

		ambient = (Kd * diffuse + specular) * 1.0f;
	
	
	// normal = vec3(pow(texture(material.NormalMap, fs_in.v_TexCoord).r, 1.0/2.2),
	// pow(texture(material.NormalMap, fs_in.v_TexCoord).g, 1.0/2.2),
	// pow(texture(material.NormalMap, fs_in.v_TexCoord).b, 1.0/2.2));
	// normal = normalize(normal);

	Colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec3 direct = vec3(0);
	// { // DIRECT
	// 	for (int i = 0; i < pointLightCount; i++)
	// 	{
	// 		vec3 lightDir = normalize(fs_in.pointLights[i].Pos - fs_in.v_FragPos);
	// 		vec3 h = normalize(viewDir + lightDir);
	// 		float NdotL = dot(normal, lightDir);
	// 		float NdotH = dot(normal, h);
	// 		float VdotH = dot(viewDir, h);
		
	// 		if (NdotL >= 0)
	// 		{
	// 			float lightStrength = (1.0f / pow(length(fs_in.pointLights[i].Pos - fs_in.v_FragPos), 2))
	// 				* fs_in.pointLights[i].LuminousPower;
	// 			vec3 lightColour = fs_in.pointLights[i].Colour;
	// 			vec3 lightRadiance = lightStrength * lightColour;

	// 			vec3 F = Fresnel(VdotH, f0, 1.0f);
	// 			float D = N_Distr(NdotH, alpha);
	// 			float V = V_Func(NdotL, NdotV, alpha);

	// 			vec3 kS = F;
	// 			vec3 kD = (vec3(1.0f) - kS) * (1.0f - metalness);

	// 			vec3 f_spec = (F * V * D);
	// 			vec3 f_diffuse = albedo / PI;
	// 			direct += (f_spec + kD * f_diffuse) * lightRadiance * NdotL;
	// 		}
	// 	}
		
	// }
	Colour = vec4(ambient + direct, 1.0f);
	// Colour = vec4(specular, 1.0f);
	// Colour += vec4(irradianceMapColour * albedo / PI, 0.0f);
}



//FUNCTIONS

float D_GGX(float NdotH, float alpha)
{
	float alphaSq = alpha * alpha;
	float k = (NdotH * NdotH) * (alphaSq - 1.0) + 1.0;
	return (alphaSq / (PI * k * k));
}

vec3 F_Schlick(float VdotH, vec3 f0, float f90)
{
	return (f0 + (vec3(f90) - f0) * pow(1.0f - VdotH, 5.0f));
}

float V_SmithCorrelated(float NdotL, float NdotV, float alpha)
{
	// float a = roughness;
	// float GGXL = NdotV * (NdotL * (1.0f - a) + a);
	// float GGXV = NdotL * (NdotV * (1.0f - a) + a);
	// return 0.5 / mix(2 * NdotL * NdotV, NdotL + NdotV, a);

	float a2 = alpha * alpha;
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
