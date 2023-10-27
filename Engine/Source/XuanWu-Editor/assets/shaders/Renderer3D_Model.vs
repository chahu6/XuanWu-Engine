#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

// uniform缓冲区///
// 使用0号缓冲区///
layout(std140, binding = 0) uniform Camera{ // std140是布局
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 Normal;
	vec2 TexCoords;
};

layout(location = 0) out VertexOutput Output;

uniform mat4 u_Transform;

void main()
{
	Output.Normal = a_Normal;
	Output.TexCoords = a_TexCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}