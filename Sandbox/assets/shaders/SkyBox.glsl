#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 m_ViewProjection;

void main()
{
	gl_Position = (m_ViewProjection * vec4(a_Position, 1.0f)).xyww;
	v_TexCoords = a_Position;
}

#type fragment
#version 450 core

in vec3 v_TexCoords;

out vec4 FragColour;

uniform samplerCube cubeMap;

void main()
{
	FragColour = vec4(texture(cubeMap, v_TexCoords).rgb, 1.0f);
	// FragColour = vec4(pow(textureLod(cubeMap, v_TexCoords, 1).rgb, vec3(gamma)), 1.0f);
}
