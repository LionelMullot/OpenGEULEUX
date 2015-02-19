#version 410 core

in block
{
    vec2 Texcoord;
} In; 

uniform sampler2D ColorBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;

layout(location = 0) out vec4 Color;

void main(void)
{
	// Read gbuffer values
	vec4 colorBuffer = texture(ColorBuffer, In.Texcoord).rgba;
	vec4 normalBuffer = texture(NormalBuffer, In.Texcoord).rgba;
	float depth = texture(DepthBuffer, In.Texcoord).r;

	// Unpack values stored in the gbuffer
	vec3 diffuseColor = colorBuffer.rgb;
	vec3 specularColor = colorBuffer.aaa;
	float specularPower = normalBuffer.a;
	vec3 n = normalBuffer.rgb;

    Color = vec4(0.0);

    vec3 color = vec3(diffuseColor + specularColor);

	Color = vec4(color, 1.0);
}


//http://graphics.cs.williams.edu/data/meshes.xml
