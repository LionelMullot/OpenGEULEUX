#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

uniform mat4 Projection;
uniform mat4 WorldToView;
uniform mat4 ObjectToWorld;

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

out block
{
	vec3 Normal;
	vec2 TexCoord; 
} Out;

void main()
{	
	Out.TexCoord = TexCoord;
	Out.Normal = Normal;

	vec4 pos = vec4(Position, 1.0);

	gl_Position = Projection * WorldToView * ObjectToWorld * pos;
}
