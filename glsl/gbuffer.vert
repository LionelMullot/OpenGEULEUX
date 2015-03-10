#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

precision highp float;
precision highp int;

uniform mat4 Projection;
uniform mat4 WorldToView;
uniform mat4 ObjectToWorld;

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

out gl_PerVertex
{
	vec4 gl_Position;
};

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
