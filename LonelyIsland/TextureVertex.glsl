uniform vec4 LightPosition = vec4(15.0, 80.0, 28.0, 1.0);

varying vec3 eyeSurfaceNormal;
varying vec3 eyeLightDirection;
varying vec3 eyeViewerDirection;
varying vec3 eyeSurfacePosition;
varying vec2 texCoords;

varying	vec3 lt;
varying	vec3 et;

varying vec4 vertex;

	// matrices of light source
uniform mat4 lightModelview1;
uniform mat4 lightProj1;
uniform mat4 modelMatrix;
uniform mat4 mirror;
varying vec4 liCoor1;

void main()
{
	vec4 position =	gl_ModelViewMatrix * gl_Vertex;	
	vertex = mirror*inverse(transpose(modelMatrix))*gl_ModelViewMatrix*gl_Vertex;
	eyeSurfacePosition = position.xyz / position.w;
	vec3 eyeLightPosition = LightPosition.xyz / LightPosition.w; 

	eyeViewerDirection = normalize(-eyeSurfacePosition);
	eyeSurfaceNormal   = normalize(gl_NormalMatrix * gl_Normal);
	eyeLightDirection  = normalize(eyeLightPosition - eyeSurfacePosition);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	texCoords = vec2(gl_MultiTexCoord0.x, gl_MultiTexCoord0.y);

	vec3	p = vec3      ( gl_ModelViewMatrix * gl_Vertex );		// transformed point to world space

	vec3	l = normalize ( eyeLightDirection - p);					// vector to light source
	vec3	e = normalize ( eyeViewerDirection - p );					// vector to the eye
	vec3	n = gl_NormalMatrix * gl_Normal;						// transformed n

	vec3 t = gl_NormalMatrix * gl_MultiTexCoord1.xyz;				// transformed t
	vec3 b = gl_NormalMatrix * gl_MultiTexCoord2.xyz;				// transformed b

																	// now remap l, and e into tangent space
	et = vec3 ( dot ( e, t ), dot ( e, b ), dot ( e, n ) );

//	lt = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );

	liCoor1 = transpose(lightModelview1)* vertex;
	liCoor1 = transpose(lightProj1) * liCoor1;
	liCoor1.x = (liCoor1.x/liCoor1.w + 1.0) /2.0;
	liCoor1.y = (liCoor1.y/liCoor1.w + 1.0) /2.0;
	liCoor1.z = (liCoor1.z/liCoor1.w)*0.5 + 0.5;
}