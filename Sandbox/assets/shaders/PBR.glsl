#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;

uniform mat4 m_ViewProjection;
uniform mat4 m_Transform;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	gl_Position = m_ViewProjection * m_Transform * vec4(a_Position, 1.0f);
}


#type fragment
#version 410 core
			
in vec3 v_Normal;
in vec2 v_TexCoord;

out vec4 Colour;

struct Material
{
	sampler2D AlbedoMap;
	sampler2D NormalMap;
	sampler2D MetallicMap;
	sampler2D RoughnessMap;

	vec3 AmbientColour;
	vec3 DiffuseColour;
	float MetallicFactor;
	float RoughnessFactor;
};

uniform Material material;

void main()
{
	// Colour = vec4(v_Normal.xy, 1.0f, 1.0f);
	Colour = texture(material.AlbedoMap, v_TexCoord);
	// Colour = vec4(material.AmbientColour, 1.0f);
}
