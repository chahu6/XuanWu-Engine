#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TillingFactor;
layout(location = 5) in int a_EntityID;

//uniform mat4 u_ViewProjection;
//uniform mat4 u_Model; // 没什么用了，a_Position已经乘以Model矩阵了

// uniform缓冲区///
// 使用0号缓冲区///
layout(std140, binding = 0) uniform Camera{ // std140是布局
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TexIndex;
	float TillingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TexIndex = a_TexIndex;
	Output.TillingFactor = a_TillingFactor;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}