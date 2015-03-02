#version 410 core

in block
{
    vec2 Texcoord;
} In; 

uniform sampler2D ColorBuffer;
uniform sampler2D NormalBuffer;
uniform sampler2D DepthBuffer;

uniform vec3 CameraView;
uniform mat4 ScreenToWorld;

uniform vec3  AreaLightPosition;
uniform vec3  AreaLightDirection;
uniform vec3  AreaLightUp;
uniform vec3  AreaLightRight;
uniform vec2  AreaLightSize;
uniform vec3  AreaLightColor;
uniform float AreaLightDiffuseIntensity;
uniform float AreaLightSpecularIntensity;
uniform float AreaLightDistance;

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

	//Distance 
	float dist = distance(Position, nearestPointInside);

	vec3 lVector = normalize(nearestPointInside - Position);

	float attenuation = 1.0 / pow(dist,3);
/*
	//Normal 
	float lDistance = 1.0 - clamp(dist/AreaLightDistance, 0.0, 1.0);

	//Weighting
	float areaFactor = AreaLightSize.x * AreaLightSize.y;

	float NdotL = clamp( dot(Normal, -lVector), 0.0, 1.0 );
	//Make lighting Smoother
	NdotL = NdotL * NdotL * NdotL;

	float LNdotL = clamp( ( dot(-AreaLightDirection, lVector) * 2.0 - 1.0 ), 0.0, 1.0 );
	LNdotL = LNdotL * NdotL * areaFactor;
*/
	float NdotL = dot(AreaLightDirection, -lVector);

	// Looking at the plane ?
	if( NdotL > 0.0 && sideOfPlane(Position, AreaLightPosition, AreaLightDirection) == 1 )
	{
		// Specular Factor
		vec3 r = reflect(normalize(-Position), Normal);
		r = normalize(r);
		vec3 e = linePlaneIntersect(Position, r, AreaLightPosition, AreaLightDirection);

		float specAngle = dot(r, AreaLightDirection);
		if(specAngle > 0.0)
		{
			vec3 dirSpec = e - AreaLightPosition;
			vec2 dirSpec2D = vec2(dot(dirSpec, AreaLightRight), dot(dirSpec, AreaLightUp));
			vec2 nearestSpec2D = vec2(clamp( dirSpec2D.x, - AreaLightSize.x, AreaLightSize.x), clamp( dirSpec2D.y, - AreaLightSize.y, AreaLightSize.y));
			float specFactor = 1.0 - clamp(length(nearestSpec2D- dirSpec2D)*20.0, 0.0, 1.0);
			specular = AreaLightColor.rgb * AreaLightSpecularIntensity * specFactor * specAngle;
		}

		diffuse = AreaLightColor.rgb * AreaLightDiffuseIntensity * NdotL;
	}

	//return vec4(dir, 1.0);
	//return vec4( AreaLightDirection, 1.0 );
	//return vec4(vec3(nearestPointInside),1.0);
	return vec4( vec3(diffuse * diffuseColor), 1.0);
	//return vec4( vec3((diffuse * diffuseColor) + (specular * specularColor)) , 1.0 );
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

	// Convert texture coordinates into screen space coordinates
	vec2 xy = In.Texcoord * 2.0 - 1.0;
	// Convert depth to -1,1 range and multiply the point by ScreenToWorld matrix
	vec4 wP = vec4(xy, depth * 2.0 -1.0, 1.0) * ScreenToWorld;
	// Divide by w
	vec3 position = vec3(wP.xyz / wP.w);

	vec3 n = normalize(normalBuffer.rgb);
	vec3 v = -normalize(position);

	Color = AreaLightCalculation(position, n, diffuseColor, specularColor);

    //Color = vec4(0.0);

    //vec3 color = vec3(diffuseColor);

	//Color = vec4(color, 1.0);
}


//http://graphics.cs.williams.edu/data/meshes.xml
