#type vertex
#version 450 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
	gl_Position = vec4(a_Pos.xy, 0.0f, 1.0f);
	v_TexCoords = a_TexCoords;
}


#type fragment
#version 450 core

const uint SAMPLE_COUNT = 1024u; 
const float PI = 3.14159;

in vec2 v_TexCoords;

// layout(location = 0) out vec4 Colour;
layout(location = 0) out vec2 Colour;
// layout(location = 0) out vec2 Colour;

float Geometry_Smith(float NdotV, float NdotL, float alpha);
float Geometry_SchlickGGX(float NdotV, float alpha);
vec2 Hammersley(uint i, uint N);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float alpha);
vec2 IntegrateBRDF(float NdotV, float roughness);

void main()
{
	// Colour = vec4(IntegrateBRDF(v_TexCoords.x, v_TexCoords.y), 0.0f, 1.0f);
	Colour = vec2(IntegrateBRDF(v_TexCoords.x, v_TexCoords.y));
	// Colour = vec3(IntegrateBRDF(v_TexCoords.x, v_TexCoords.y), 0.0f);
	// Colour = vec3(0.7f, 0.3f, 0.0f);
}

vec2 IntegrateBRDF(float NdotV, float roughness)
{
	float alpha = roughness * roughness;
	vec3 V = normalize(vec3(sqrt(1.0f - NdotV * NdotV), 0.0f, NdotV));

	float A = 0;
	float B = 0;

	vec3 N = vec3(0.0f, 0.0f, 1.0f);

	for (uint i = 0u; i < SAMPLE_COUNT; i++)
	{
		vec2 Xi = Hammersley(i, SAMPLE_COUNT);
		vec3 H = ImportanceSampleGGX(Xi, N, alpha);

		float HdotV = dot(H, V);
		vec3 L = normalize(2.0f * HdotV * H - V);
		float NdotL = L.z;
		float NdotV = V.z;
		float NdotH = H.z;

		if(NdotL > 0.0f)
		{
			float G_Smith = Geometry_Smith(NdotL, NdotV, alpha);
			float Gvis = G_Smith * HdotV / (NdotV * NdotH);
			float Fc = pow(1.0f - HdotV, 5.0f);
		
			A += (1.0f - Fc) * Gvis;
			B += Fc * Gvis;
		}
	}

	// if(A<0.0f || B<0.0f)
	// 	{
	// 		return vec2(0.0f, 0.0f);
	// 	}
	
	return vec2(A, B) / SAMPLE_COUNT;
}

vec2 Hammersley(uint i, uint N)
{
	uint bits = i;
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

	float element = float(bits) * 2.3283064365386963e-10; // divide by 0x10000000
	return vec2(float(i) / float(N), element);
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float alpha)
{
	float phi = 2.0f * PI * Xi.x;
	float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (alpha * alpha - 1.0f) * Xi.y));
	float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	// vec3 up = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 up = vec3(1.0f, 0.0f, 0.0f);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

float Geometry_Smith(float NdotV, float NdotL, float alpha)
{
	return Geometry_SchlickGGX(NdotV, alpha) * Geometry_SchlickGGX(NdotL, alpha);
}

float Geometry_SchlickGGX(float NdotV, float alpha)
{
	float k = alpha / 2.0f;
	return NdotV / (NdotV * (1.0f - k) + k);
}
