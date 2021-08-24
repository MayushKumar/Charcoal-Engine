#type vertex
#version 410

layout(location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

uniform mat4 m_ViewProjection;

void main()
{
	gl_Position = (m_ViewProjection * vec4(a_Position, 1.0f)).xyww;
	v_TexCoords = a_Position;
}

#type fragment
#version 410

in vec3 v_TexCoords;

out vec4 FragColour;

uniform samplerCube cubeMap;

void main()
{
	FragColour = texture(cubeMap, v_TexCoords);
}
