varying mat4 invM;
varying mat4 M;
varying mat4 M1;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	M = gl_ModelViewProjectionMatrix;
	M1 = gl_ModelViewMatrix;
	invM = inverse(gl_ModelViewProjectionMatrix);
}