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

void main()
{
	vec3 normal = normalize(texture(material.NormalMap, fs_in.v_TexCoord).rgb * 2.0 - 1.0);
	vec3 albedo = texture(material.AlbedoMap, fs_in.v_TexCoord).rgb;
	float re_roughness = clamp(pow(texture(material.RoughnessMap, fs_in.v_TexCoord).r, 2.2), 0.0003, 1.0f);
	float metalness = texture(material.MetallicMap, fs_in.v_TexCoord).r;
	// re_roughness = 0.001;
	// normal = vec3(0.0f, 0.0f, 1.0f);
	vec3 viewDir = normalize(fs_in.v_ViewPos - fs_in.v_FragPos);

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
	//Colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
