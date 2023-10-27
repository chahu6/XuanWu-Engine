#version 450 core
layout(location = 0)out vec4 o_Color;
layout(location = 1)out int o_EntityID;

struct VertexOutput
{
	vec3 Normal;
	vec2 TexCoords;
};

layout(location = 0) in VertexOutput Input;

// EntityID
uniform int u_EntityID;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

void main()
{	
	o_Color = texture(diffuseMap, Input.TexCoords);
	//o_Color.a = 1.0;
	o_Color.a = texture(specularMap, Input.TexCoords).r;

	o_EntityID = u_EntityID;
}