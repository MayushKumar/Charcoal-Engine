#type vertex
#version 450 core

in layout(location=0) vec3 a_Pos;

out vec3 v_Pos;

uniform mat4 m_ViewProjection;

void main()
{
	gl_Position = vec4(m_ViewProjection * vec4(a_Pos * 2.0f, 1.0f));
	v_Pos = normalize(a_Pos);
}


#type fragment
#version 450 core

#define PI 3.14159265359
vec2 Hammersley(uint i, uint N);
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float alpha);

in vec3 v_Pos;

out vec4 Colour;

uniform samplerCube envMap;
uniform float roughness;

const uint SAMPLE_COUNT = 700;

void main()
{
	Colour = vec4(0.0f);
	float totalWeight = 0.0f;
	float alpha = roughness * roughness;
	vec3 normal = normalize(v_Pos);

	for(uint i = 0u; i < SAMPLE_COUNT; i++)
	{
		vec3 H = ImportanceSampleGGX(Hammersley(i, SAMPLE_COUNT), normal, alpha);
		vec3 L = reflect(-normal, H);

		float NdotL = dot(L, normal);

		if(NdotL > 0.0f)
		{
			// Weighting by NdotL just improves visual quality.
			// It is not an actual part of the Split Sum Approximation
			Colour += vec4(texture(envMap, L).rgb * NdotL, 0.0f); 
			totalWeight += NdotL;
		}
	}

	Colour = vec4(Colour.rgb / totalWeight, 1.0f);
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

	vec3 up = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}
