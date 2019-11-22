#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 m_ViewProjection;
uniform mat4 m_Transform;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = m_ViewProjection * m_Transform * vec4(a_Position, 1.0f);
}


#type fragment
#version 410 core
			
in vec2 v_TexCoord;

out vec4 Colour;

uniform sampler2D u_Texture;

void main()
{
	Colour = vec4(v_TexCoord, 0.0f, 1.0f);
	Colour = texture(u_Texture, v_TexCoord);
}