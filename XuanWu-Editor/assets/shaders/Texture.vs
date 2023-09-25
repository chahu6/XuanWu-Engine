#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TillingFactor;
layout(location = 5) in int a_EntityID;

//uniform mat4 u_ViewProjection;
//uniform mat4 u_Model; // ûʲô���ˣ�a_Position�Ѿ�����Model������

// uniform������///
// ʹ��0�Ż�����///
layout(std140, binding = 0) uniform Camera{ // std140�ǲ���
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