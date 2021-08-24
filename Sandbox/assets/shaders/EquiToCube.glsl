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

layout (location = 0) out vec4 color;

in vec3 v_Pos;

uniform sampler2D equiMap;

const float PI = 3.14159f;
const vec2 normFactor = vec2(1.0f / (PI), 1.0f / (PI * 0.5f)) * 0.5f; // 0.5f is there so that after normalization, mapping lies between -0.5f to 0.5f

void main()
{
	vec3 normPos = normalize(v_Pos);
	vec2 mapping = vec2(atan(normPos.z, normPos.x), asin(normPos.y));
	mapping *= normFactor;
	mapping += 0.5f;
	color = texture(equiMap, mapping);
}
