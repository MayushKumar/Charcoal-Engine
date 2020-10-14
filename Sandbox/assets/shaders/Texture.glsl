#type vertex
#version 410 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TextureSlot;
layout(location = 3) in vec4 a_Colour;
layout(location = 4) in float a_TilingFactor;

out vec2 v_TexCoord;
out float v_TextureSlot;
out vec4 v_Colour;
out float v_TilingFactor;

void main()
{
	v_TexCoord = a_TexCoord;
	v_TextureSlot = a_TextureSlot;
	v_Colour = a_Colour;
	v_TilingFactor = a_TilingFactor;

	gl_Position = vec4(a_Position, 1.0f);
}


#type fragment
#version 410 core

in vec2 v_TexCoord;
in float v_TextureSlot;
in vec4 v_Colour;
in float v_TilingFactor;

uniform sampler2D u_TextureSlots[32];

out vec4 Colour;

void main()
{
	if (v_TextureSlot == 0)  Colour = texture(u_TextureSlots[0],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 1)  Colour = texture(u_TextureSlots[1],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 2)  Colour = texture(u_TextureSlots[2],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 3)  Colour = texture(u_TextureSlots[3],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 4)  Colour = texture(u_TextureSlots[4],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 5)  Colour = texture(u_TextureSlots[5],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 6)  Colour = texture(u_TextureSlots[6],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 7)  Colour = texture(u_TextureSlots[7],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 8)  Colour = texture(u_TextureSlots[8],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 9)  Colour = texture(u_TextureSlots[9],  v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 10) Colour = texture(u_TextureSlots[10], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 11) Colour = texture(u_TextureSlots[11], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 12) Colour = texture(u_TextureSlots[12], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 13) Colour = texture(u_TextureSlots[13], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 14) Colour = texture(u_TextureSlots[14], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 15) Colour = texture(u_TextureSlots[15], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 16) Colour = texture(u_TextureSlots[16], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 17) Colour = texture(u_TextureSlots[17], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 18) Colour = texture(u_TextureSlots[18], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 19) Colour = texture(u_TextureSlots[19], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 20) Colour = texture(u_TextureSlots[20], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 21) Colour = texture(u_TextureSlots[21], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 22) Colour = texture(u_TextureSlots[22], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 23) Colour = texture(u_TextureSlots[23], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 24) Colour = texture(u_TextureSlots[24], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 25) Colour = texture(u_TextureSlots[25], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 26) Colour = texture(u_TextureSlots[26], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 27) Colour = texture(u_TextureSlots[27], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 28) Colour = texture(u_TextureSlots[28], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 29) Colour = texture(u_TextureSlots[29], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 30) Colour = texture(u_TextureSlots[30], v_TexCoord * v_TilingFactor) * v_Colour;
	if (v_TextureSlot == 31) Colour = texture(u_TextureSlots[31], v_TexCoord * v_TilingFactor) * v_Colour;

}
