#version 410 core

in block
{
    vec2 Texcoord;
} In; 

uniform sampler2D ColorBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;
uniform sampler2DShadow ShadowTexture;
uniform sampler2D LightTexture;

uniform vec3 CameraView;
uniform mat4 ScreenToWorld;
uniform mat4 WorldToLightScreen;

uniform vec3  AreaLightPosition;
uniform vec3  AreaLightDirection;
uniform vec3  AreaLightUp;
uniform vec3  AreaLightRight;
uniform vec2  AreaLightSize;
uniform vec3  AreaLightColor;
uniform float AreaLightDiffuseIntensity;
uniform float AreaLightSpecularIntensity;
uniform float AreaLightDistance;

uniform vec3 ShadowTexelSize;
uniform float ShadowBias;

precision highp float;
precision highp int;

layout(location = 0) out vec4 Color;

vec3 projectOnPlane(vec3 p, vec3 pc, vec3 pn)
{
	float dist = dot(pn, p - pc);
	return p - dist * pn;
}

int sideOfPlane(vec3 p, vec3 pc, vec3 pn)
{
	if( dot(p - pc, pn) >= 0.0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

vec3 linePlaneIntersect( vec3 lp, vec3 lv, vec3 pc, vec3 pn )
{
	return lp + lv * ( dot(pn, pc - lp) / dot(pn, lv) );
}

vec4 AreaLightCalculation(vec3 Position, vec3 Normal, vec3 diffuseColor, vec3 specularColor)
{	
	vec3 specular = vec3(0.0,0.0,0.0);
	vec3 diffuse = vec3(0.0,0.0,0.0);

	//project onto plane and calculate direction from center to the projection.
	vec3 projection = projectOnPlane(Position, AreaLightPosition, AreaLightDirection);
	vec3 dir = projection - AreaLightPosition;

	//calculate distance from area
	vec2 diagonal = vec2( dot(dir, AreaLightRight), dot(dir, AreaLightUp) );
	vec2 nearest2D = vec2( clamp(diagonal.x, -AreaLightSize.x, AreaLightSize.x), clamp(diagonal.y, -AreaLightSize.y, AreaLightSize.y) );
	vec3 nearestPointInside = AreaLightPosition + (AreaLightRight * nearest2D.x + AreaLightUp * nearest2D.y);

	//Distance to the light
	float dist = distance(Position, nearestPointInside);

	vec3 lVector = normalize(nearestPointInside - Position);

	float areaFactor = AreaLightSize.x * AreaLightSize.y;

	float attenuation = 1.0 / (dist + pow(dist, 3) + pow(dist,2));

	float NdotL = clamp( dot(Normal, lVector), 0.0, 1.0 );
	// Make lighting Smoother
	NdotL = NdotL * NdotL * NdotL;

	float LNdotL = clamp( ( dot(-AreaLightDirection, lVector) * 2.0 - 1.0 ), 0.0, 1.0 );
	LNdotL = LNdotL * NdotL * areaFactor;

	// Looking at the plane ?
	if( LNdotL > 0.0 && sideOfPlane(Position, AreaLightPosition, AreaLightDirection) == 1 )
	{
		//vec3 lightTextBuffer = texture2D(LightTexture, reflect(Position, lVector).xz).rgb;

		// Diffuse Factor
		vec3 lWeightDiffuse = vec3( LNdotL );
		//diffuse = lightTextBuffer * lWeightDiffuse * attenuation * AreaLightDiffuseIntensity;
		diffuse = AreaLightColor * lWeightDiffuse * attenuation * AreaLightDiffuseIntensity;

		// Specular Factor
		vec3 r = -reflect(lVector, Normal);
		r = normalize(r);

		float lWeightSpecular = pow( max(0.0, dot(r, Normal)), 20.0);

		//specular = LightTexture * lWeightSpecular * attenuation * AreaLightSpecularIntensity;
		specular = AreaLightColor * lWeightSpecular * attenuation * AreaLightSpecularIntensity;
	}

	return vec4( vec3((diffuse * diffuseColor) + (specular * specularColor)), 1.0 );
}

float SampleShadowMap(vec2 coords, float zcoord)
{
	return textureProj(ShadowTexture, vec4(coords.xy, zcoord - ShadowBias, 1.0), 0.0); 	
}

float SampleShadowMapLinear(vec2 coords, float zcoord, vec2 texelSize)
{
	vec2 pixelPos = (coords.xy / texelSize.xy) + vec2(0.5);
	vec2 fracPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart) * texelSize;

	float blTexel = SampleShadowMap(startTexel, zcoord); // Bottom left
	float brTexel = SampleShadowMap(startTexel + vec2(texelSize.x, 0.0), zcoord); // Bottom right
	float tlTexel = SampleShadowMap(startTexel + vec2(0.0, texelSize.y), zcoord); // Top left
	float trTexel = SampleShadowMap(startTexel + texelSize, zcoord); // Top right

	float mixA = mix(blTexel, tlTexel, fracPart.y);
	float mixB = mix(brTexel, trTexel, fracPart.y);

	return mix(mixA, mixB, fracPart.x);
}

const float BLOCKER_STEP_COUNT = 4.0f;
const float NUM_SAMPLES = 4.0f;

float SampleShadowMapPCF(vec3 coords, vec2 texelSize, vec2 penumbra)
{
	const float SAMPLES_START = (NUM_SAMPLES - 1.0f)/2.0f;
	const float NUM_SAMPLES_SQUARED = NUM_SAMPLES*NUM_SAMPLES;

	float result = 0.0f;
	for(float y = -SAMPLES_START; y <= SAMPLES_START; y += 1.0f)
	{
		for(float x = -SAMPLES_START; x <= SAMPLES_START; x += 1.0f)
		{
			vec2 coordsOffset = vec2(x, y) * texelSize;
			result += SampleShadowMapLinear( (coords.xy + coordsOffset), coords.z, texelSize);
		}
	}

	return result / NUM_SAMPLES_SQUARED;
}

float FindBlocker(vec3 coords, float distanceToLight, vec2 texelSize)
{
	float blockerSum = 0.0;
	float blockerCount = 0.0;
 	float shadowDepth;

 	vec2 widthSearch = (distanceToLight - 0.1) * AreaLightSize / distanceToLight;
 	vec2 stepUV = widthSearch / BLOCKER_STEP_COUNT;

    // iterate through search region and add up depth values
    for(float x = -BLOCKER_STEP_COUNT; x <= BLOCKER_STEP_COUNT; x += 1.0)
	{
        for(float y = -BLOCKER_STEP_COUNT; y < BLOCKER_STEP_COUNT; y += 1.0)
	    {
	    	vec2 coordsOffset = vec2(x, y) * stepUV;
        	shadowDepth = SampleShadowMap(coords.xy + coordsOffset, coords.z);   
 			
 			// found a blocker
            if(distanceToLight > shadowDepth)
		    {
                blockerSum += shadowDepth;
             	blockerCount += 1.0;
            }
        }
    }

	return (blockerSum / blockerCount);
}

vec2 EstimatePenumbra(float dReceiver, float dBlocker)
{
    return ( (AreaLightSize * (dReceiver - dBlocker)) / dBlocker );
}

float SampleShadowMapPCSS(vec3 coords, float distanceToLight, vec2 texelSize)
{
	float blocker = FindBlocker(coords, distanceToLight, texelSize);

    // Early out if no blocker found
	if(blocker == 0.0){ return 1.0; }

	vec2 penumbra = EstimatePenumbra(coords.z, blocker);

	float shadowDepth = SampleShadowMapPCF(coords, texelSize, penumbra);

	return shadowDepth;
}

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

	// Compute ambient color
	vec3 ambient = vec3(1.0, 1.0, 1.0);
	float ambientFactor = 0.18;
	vec3 ambientColor = vec3( ambient * ambientFactor * diffuseColor );

	// Convert texture coordinates into screen space coordinates
	vec2 xy = In.Texcoord * 2.0 - 1.0;
	// Convert depth to -1,1 range and multiply the point by ScreenToWorld matrix
	vec4 wP = vec4(xy, depth * 2.0 -1.0, 1.0) * ScreenToWorld;
	// Divide by w
	vec3 position = vec3(wP.xyz / wP.w);

	vec3 normal = normalize(normalBuffer.rgb);

	Color = AreaLightCalculation(position, normal, diffuseColor, specularColor);

	// It's for an optimisation
	// We don't have to calcul shadow map if we are not in the light
	if( (Color.r + Color.g + Color.b) > 0.001)
	{
		// Read Shadow map value
		vec4 wlP = WorldToLightScreen * vec4(position, 1.0);
		vec3 lP = vec3(wlP/wlP.w) * 0.5 + 0.5;
		
		vec3 I = position - AreaLightPosition;
		float distanceToLight = length(I);

		vec2 texelSize = ShadowTexelSize.xy;

		//float shadowDepth = SampleShadowMapPCF(lP, texelSize, vec2(2.0));

		float shadowDepth = SampleShadowMapPCSS(lP, distanceToLight, texelSize);

		Color = Color * vec4(vec3(shadowDepth), 1.0);
	}
	
	// Finally add ambient color
	Color = Color + vec4(vec3(ambientColor), 1.0);
}
