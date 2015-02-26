#version 410 core

#define POSITION	0
#define NORMAL		1
#define TEXCOORD	2
#define FRAG_COLOR	0

uniform sampler2D Diffuse;
uniform sampler2D Specular;

uniform float SpecularPower;

// Write in GL_COLOR_ATTACHMENT0
layout(location = 0 ) out vec4 Color;
// Write in GL_COLOR_ATTACHMENT1
layout(location = 1) out vec4 Normal;

in block
{
	vec2 TexCoord;
	vec3 Normal;
} In;


void main()
{
	vec3 diffuse = texture(Diffuse, In.TexCoord).rgb;
	vec3 specular = texture(Specular, In.TexCoord).rgb;

	Color = vec4( diffuse, specular.r );
	Normal = vec4( In.Normal, SpecularPower );
}
