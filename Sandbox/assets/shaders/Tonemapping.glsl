#type vertex
#version 450 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoords;

void main()
{
	v_TexCoords = a_TexCoords;
	gl_Position = vec4(a_Pos, 0.0f, 1.0f);
}


#type fragment
#version 450 core

in vec2 v_TexCoords;

out vec4 FragColor;

uniform sampler2D u_HDRTexture;

const float gamma = 2.2;

void main()
{
	vec3 texColor = texture(u_HDRTexture, v_TexCoords).rgb;
	vec3 toneMapped = texColor / (texColor + vec3(1.0f));
	FragColor = vec4(pow(toneMapped, vec3(1.0/gamma)), 1.0);
	// FragColor = vec4(texture(u_HDRTexture, v_TexCoords));
}
