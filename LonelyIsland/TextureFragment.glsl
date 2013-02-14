#version 330

uniform vec4 MaterialSpecular = vec4(0.0, 0.0, 0.0, 0.0);
uniform vec4 MaterialAmbient = vec4(0.8, 0.8, 0.8, 0.5);
uniform sampler2D diffuseMap;
uniform sampler2D diffuseMap2;
uniform sampler2D shadowTexture1;
uniform sampler2D shadowTexture2;
uniform sampler2D shadowTexture3;
uniform sampler2D shadowTexture4;

uniform float Shininess = 5.0;

uniform vec4 LightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 LightAmbient = vec4(0.5, 0.5, 0.5, 1.0);

vec4 GetAmbientReflection(
	vec4 surfaceColor,
    vec4 lightColor) 
{
	return lightColor * surfaceColor;
}

vec4 GetDiffuseReflection(
	vec4 surfaceColor,
	vec3 surfaceNormal,
    vec4 lightColor,
	vec3 lightDirection) 
{
	float diffuseFactor = 
		clamp(
			dot(lightDirection, surfaceNormal),0.0,1.0);
	return lightColor * surfaceColor * diffuseFactor;
}

vec4 GetSpecularReflection(
	vec4 surfaceColor,
    float  surfaceShininess,
    vec3 surfaceNormal,
    vec4 lightColor,
    vec3 halfAngle) 
{
	float specularFactor = 
	   pow(clamp(dot(halfAngle, surfaceNormal),0.0,1.0), surfaceShininess);
  
	return lightColor * surfaceColor * specularFactor;       
}

vec4 GetBlinnReflection(
	vec4 ambientSurfaceColor,
    vec4 ambientLightColor,
    vec4 diffuseSurfaceColor,
    vec4 specularSurfaceColor,
    float  surfaceShininess,
    vec3 surfaceNormal,
    vec3 halfAngle,
    vec3 lightDirection,
    vec4 lightColor) {
                       
	vec4 ambient = GetAmbientReflection(ambientSurfaceColor, ambientLightColor);

	vec4 diffuse = GetDiffuseReflection(
		diffuseSurfaceColor, 
		surfaceNormal, 
		lightColor, 
		lightDirection);
                                     
	vec4 specular = GetSpecularReflection(
		specularSurfaceColor, 
		surfaceShininess, 
		surfaceNormal,
		lightColor, 
		halfAngle);

	if (dot(lightDirection, surfaceNormal) <= 0)
	{
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
    
	return diffuse + specular + ambient;
}

varying vec3 eyeSurfaceNormal;
varying vec3 eyeLightDirection;
varying vec3 eyeViewerDirection;
varying vec3 eyeSurfacePosition;
varying vec2 texCoords;

varying	vec3 et;
varying	vec3 lt;

varying vec4 vertex;

uniform float density = 1.0;

varying vec4 liCoor1; //coords in light plane
varying mat4 invTr;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	const vec4 fogColor = vec4(0.5, 0.8, 0.5, 1.0);
	const float e = 2.71828;
	const float fh0 = 0.1;
	const float b = 0.7;

	float fh = (vertex.y / vertex.w);
	float fogFactor;

	if (fh < fh0) {
		fogFactor = 1;
	} else {
		fogFactor = b * (fh - fh0);
		fogFactor *= fogFactor;
		fogFactor = clamp(pow(e, - b * fogFactor), 0.0, 1.0);
	}

	vec3 nEyeLightDir = normalize(eyeLightDirection);
	vec3 eyeHalfAngle = normalize(normalize(eyeViewerDirection) + nEyeLightDir);
	vec3 eyeViewerDirection = normalize(-eyeSurfacePosition);
	
	vec4 materialDiffuse = texture2D(diffuseMap, texCoords);
	vec4 materialDiffuse2 = texture2D(diffuseMap, texCoords);

	vec2 tex = texCoords;
	const float numSteps = 10.0;
	const float scale = 1.0;
	const float step = 0.1;
	float height = 2.0;
	vec2 dtex = et.xy * scale / ( numSteps * et.z );
	float h = texture2D(diffuseMap2, tex).g;

	/*while ( h < height ) {
		height -= step;
		tex    += dtex;
		h       = texture2D(diffuseMap2, tex).g;
	}*/

	//shadow begin
	float shad = float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x, liCoor1.y)).x+0.0001);
	shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x+0.001333, liCoor1.y)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x+0.001333, liCoor1.y+0.001333)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x+0.001333, liCoor1.y-0.001333)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x, liCoor1.y+0.001333)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x, liCoor1.y-0.001333)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x-0.001333, liCoor1.y)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x-0.001333, liCoor1.y+0.001333)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x-0.001333, liCoor1.y-0.001333)).x+0.0001);
		shad *= 0.5;
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x+0.0006667, liCoor1.y)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x+0.0006667, liCoor1.y+0.0006667)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x+0.0006667, liCoor1.y-0.0006667)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x, liCoor1.y-0.0006667)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x, liCoor1.y+0.0006667)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x-0.0006667, liCoor1.y)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x-0.0006667, liCoor1.y+0.0006667)).x+0.0001);
		shad+=float(liCoor1.z > texture2D (shadowTexture1, vec2(liCoor1.x-0.0006667, liCoor1.y-0.0006667)).x+0.0001);
	materialDiffuse = (1-shad/27.0)*materialDiffuse;
	vec4 materialSpecular = (shad/13.5)*MaterialSpecular;
	// Shadows end

	if((vertex.x<-25.7 || vertex.x>25.7) || (vertex.z<-25.7 || vertex.z>25.7))
		fogFactor = 0;
		gl_FragColor = mix (
		
		(GetBlinnReflection(
			materialDiffuse, 
			LightAmbient,
			materialDiffuse, 
			MaterialSpecular, 
			Shininess,
			normalize(eyeSurfaceNormal), 
			eyeHalfAngle,
			nEyeLightDir, 
			LightDiffuse)),
		
			fogColor,

			fogFactor);
}
