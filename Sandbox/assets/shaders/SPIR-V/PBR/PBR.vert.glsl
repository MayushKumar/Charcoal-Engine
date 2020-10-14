#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec3 a_Bitangent;
layout(location = 4) in vec2 a_TexCoord;

struct PointLight
{
	vec3 Pos;
	vec3 Colour;
	float LuminousPower; //Lumens
};

out VS_Out
{
	vec3 v_FragPos;
	vec2 v_TexCoord;
	vec3 v_ViewPos;
	vec3 v_TestNormal;
	float v_TestFloat;
	mat3 invTBNMatrix;
	PointLight pointLights[2];
} vs_out;

uniform mat4 m_ViewProjection;
uniform mat4 m_Transform;
uniform mat3 m_NormalMatrix;
uniform vec3 ViewPos;
uniform int pointLightCount;
uniform PointLight pointLights[2];

void main()
{
	// Gram-Schmidt Process TODO: Move to CMFCompiler
	vec3 temp_Tangent = normalize(a_Tangent - (dot(a_Normal, a_Tangent) * a_Normal));
	vec3 temp_Bitangent = cross(a_Normal, temp_Tangent);
	
	// mat3 invTBNMatrix = transpose(mat3(normalize(m_NormalMatrix * a_Tangent), normalize(m_NormalMatrix * a_Bitangent), normalize(m_NormalMatrix * a_Normal)));
	mat3 invTBNMatrix = transpose(mat3(normalize(m_NormalMatrix * temp_Tangent), normalize(m_NormalMatrix * temp_Bitangent), normalize(m_NormalMatrix * a_Normal)));

	for (uint i = 0; i < pointLightCount; i++)
	{
		vs_out.pointLights[i].Pos = invTBNMatrix * pointLights[i].Pos;
		vs_out.pointLights[i].Colour = pointLights[i].Colour;
		vs_out.pointLights[i].LuminousPower = pointLights[i].LuminousPower;
	}

	//vs_out.lightVec = invTBNMatrix * (pointLights[0].Pos - (m_Transform * vec4(a_Position, 1.0f)).xyz);
	vs_out.v_TexCoord = a_TexCoord;
	vec4 vertexPos = m_Transform * vec4(a_Position, 1.0f);
	vs_out.v_FragPos = invTBNMatrix * vertexPos.xyz;
	vs_out.v_ViewPos = invTBNMatrix * ViewPos;
	vs_out.v_TestNormal = temp_Bitangent;
	vs_out.v_TestFloat =  dot(a_Bitangent, temp_Bitangent);
	vs_out.invTBNMatrix = invTBNMatrix;
	gl_Position = m_ViewProjection * vertexPos;
}
