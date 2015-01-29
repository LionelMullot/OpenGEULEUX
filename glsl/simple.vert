#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2

layout(location = POSITION) in vec3 Position;
layout(location = NORMAL) in vec3 Normal;
layout(location = TEXCOORD) in vec2 TexCoord;

uniform mat4 MVP;

out block
{
	vec3 Normal;
	vec2 TexCoord; 
	vec3 Position;
} Out;

void main()
{
	Out.Normal = Normal;
	Out.TexCoord = TexCoord;
	
	vec4 pos = vec4(Position, 1.0);
	gl_Position = MVP * pos;

}