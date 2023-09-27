#version 450 core
layout(location = 0)out vec4 FragColor;
layout(location = 1)out int FragColor2;

struct VertexOutput {
	vec4 Color;
	vec2 TexCoord;
	// float TexIndex; 写在这里有问题,会自动插值
	float TillingFactor;
};

layout(location = 0) in VertexOutput Input;

layout(location = 3) in flat float v_TexIndex; // 加flat就不会自动插值了
layout(location = 4) in flat int v_EntityID; // 4 是因为TilingFactor 是3

layout(binding = 0) uniform sampler2D u_Textures[32];

void main()
{
	//只能在N卡上运行，AMD卡不能这么写，应该吧
	//FragColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TillingFactor)* v_Color;

	//兼容AMD卡的写法
	vec4 texColor = Input.Color;
	//switch(int(Input.TexIndex))
	switch(int(v_TexIndex))
	{
		case 0:  texColor *= texture(u_Textures[0],  Input.TexCoord * Input.TillingFactor); break;
		case 1:  texColor *= texture(u_Textures[1],  Input.TexCoord * Input.TillingFactor); break;
		case 2:  texColor *= texture(u_Textures[2],  Input.TexCoord * Input.TillingFactor); break;
		case 3:  texColor *= texture(u_Textures[3],  Input.TexCoord * Input.TillingFactor); break;
		case 4:  texColor *= texture(u_Textures[4],  Input.TexCoord * Input.TillingFactor); break;
		case 5:  texColor *= texture(u_Textures[5],  Input.TexCoord * Input.TillingFactor); break;
		case 6:  texColor *= texture(u_Textures[6],  Input.TexCoord * Input.TillingFactor); break;
		case 7:  texColor *= texture(u_Textures[7],  Input.TexCoord * Input.TillingFactor); break;
		case 8:  texColor *= texture(u_Textures[8],  Input.TexCoord * Input.TillingFactor); break;
		case 9:  texColor *= texture(u_Textures[9],  Input.TexCoord * Input.TillingFactor); break;
		case 10: texColor *= texture(u_Textures[10], Input.TexCoord * Input.TillingFactor); break;
		case 11: texColor *= texture(u_Textures[11], Input.TexCoord * Input.TillingFactor); break;
		case 12: texColor *= texture(u_Textures[12], Input.TexCoord * Input.TillingFactor); break;
		case 13: texColor *= texture(u_Textures[13], Input.TexCoord * Input.TillingFactor); break;
		case 14: texColor *= texture(u_Textures[14], Input.TexCoord * Input.TillingFactor); break;
		case 15: texColor *= texture(u_Textures[15], Input.TexCoord * Input.TillingFactor); break;
		case 16: texColor *= texture(u_Textures[16], Input.TexCoord * Input.TillingFactor); break;
		case 17: texColor *= texture(u_Textures[17], Input.TexCoord * Input.TillingFactor); break;
		case 18: texColor *= texture(u_Textures[18], Input.TexCoord * Input.TillingFactor); break;
		case 19: texColor *= texture(u_Textures[19], Input.TexCoord * Input.TillingFactor); break;
		case 20: texColor *= texture(u_Textures[20], Input.TexCoord * Input.TillingFactor); break;
		case 21: texColor *= texture(u_Textures[21], Input.TexCoord * Input.TillingFactor); break;
		case 22: texColor *= texture(u_Textures[22], Input.TexCoord * Input.TillingFactor); break;
		case 23: texColor *= texture(u_Textures[23], Input.TexCoord * Input.TillingFactor); break;
		case 24: texColor *= texture(u_Textures[24], Input.TexCoord * Input.TillingFactor); break;
		case 25: texColor *= texture(u_Textures[25], Input.TexCoord * Input.TillingFactor); break;
		case 26: texColor *= texture(u_Textures[26], Input.TexCoord * Input.TillingFactor); break;
		case 27: texColor *= texture(u_Textures[27], Input.TexCoord * Input.TillingFactor); break;
		case 28: texColor *= texture(u_Textures[28], Input.TexCoord * Input.TillingFactor); break;
		case 29: texColor *= texture(u_Textures[29], Input.TexCoord * Input.TillingFactor); break;
		case 30: texColor *= texture(u_Textures[30], Input.TexCoord * Input.TillingFactor); break;
		case 31: texColor *= texture(u_Textures[31], Input.TexCoord * Input.TillingFactor); break;
	}

	FragColor = texColor;
	FragColor2 = v_EntityID;
}