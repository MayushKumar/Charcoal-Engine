#type vertex
#version 450 core

layout (location = 0) in vec3 a_Pos;

out vec3 v_Pos;

uniform mat4 m_ViewProjection;

void main()
{
	v_Pos = a_Pos;
	gl_Position = vec4(m_ViewProjection * vec4(a_Pos * 2.0f, 1.0f)); // a_Pos is doubled to get the positions to match the NDCs
}


#type fragment
#version 450 core

layout (location = 0) out vec4 FragColor;

in vec3 v_Pos;

uniform samplerCube envMap;

#define PI 3.14159

const float sampleDel = 0.15f;

void main()
{
	vec3 normal = normalize(v_Pos);
	vec3 bitangent = vec3(0.0f, 1.0f, 0.0f);
	vec3 tangent = normalize(cross(normal, bitangent));
	bitangent = cross(tangent, normal);

	float sampleCount = 0.0f;
	vec3 irradiance = vec3(0.0f);
	
	for(float phi = 0; phi < 2.0f * PI; phi += sampleDel)
	{
		for(float theta = 0; theta < 0.5f * PI; theta += sampleDel)
		{
			// Space: Tangent
			vec3 sampleDir = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			//Space: World
			sampleDir = tangent * sampleDir.x + bitangent * sampleDir.y + normal * sampleDir.z;
			irradiance += texture(envMap, sampleDir).rgb * cos(theta) * sin(theta);
			sampleCount++;
		}
	}

	irradiance = irradiance * PI * PI / sampleCount; // (2PI * PI)/(2del * del) = PI^2 / del^2 = PI^2 / sampleCount
	FragColor = vec4(irradiance, 1.0f);
}
