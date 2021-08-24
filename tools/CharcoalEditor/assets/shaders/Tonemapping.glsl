#type vertex
#version 330

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
	v_TexCoords = a_TexCoords;
	gl_Position = vec4(a_Pos, 0.0f, 1.0f);
}


#type fragment
#version 330

in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D u_HDRTexture;

void main()
{
	FragColor = texture(u_HDRTexture, v_TexCoords);
}
