#version 330

uniform sampler2D texture;
uniform sampler2D depth;

uniform float focus;
uniform float glass;
uniform float time;
uniform int doBlur;
uniform float winWidth;
uniform float winHeight;

varying mat4 invM;
varying mat4 M;
varying mat4 M1;
uniform mat4 modelMatrix;

vec4 conv(float k)
{
	return (texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y)/winHeight)) + 
		   
		   (texture2D (texture, vec2((gl_FragCoord.x-1)/winWidth,(gl_FragCoord.y-1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y-1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+1)/winWidth,(gl_FragCoord.y-1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x-1)/winWidth,(gl_FragCoord.y)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+1)/winWidth,(gl_FragCoord.y)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x-1)/winWidth,(gl_FragCoord.y+1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y+1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+1)/winWidth,(gl_FragCoord.y+1)/winHeight)))*k +
		   
		   (texture2D (texture, vec2((gl_FragCoord.x-2)/winWidth,(gl_FragCoord.y-2)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y-2)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+2)/winWidth,(gl_FragCoord.y-2)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x-2)/winWidth,(gl_FragCoord.y)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+2)/winWidth,(gl_FragCoord.y)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x-2)/winWidth,(gl_FragCoord.y+2)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y+2)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+2)/winWidth,(gl_FragCoord.y+2)/winHeight)))*k +

		   (texture2D (texture, vec2((gl_FragCoord.x-3)/winWidth,(gl_FragCoord.y-3)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x-1)/winWidth,(gl_FragCoord.y-3)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+1)/winWidth,(gl_FragCoord.y-3)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+3)/winWidth,(gl_FragCoord.y-3)/winHeight)) +
		   texture2D (texture, vec2((gl_FragCoord.x-3)/winWidth,(gl_FragCoord.y-1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x+3)/winWidth,(gl_FragCoord.y-1)/winHeight)) + 
		   texture2D (texture, vec2((gl_FragCoord.x-3)/winWidth,(gl_FragCoord.y+1)/winHeight)) +
		   texture2D (texture, vec2((gl_FragCoord.x+3)/winWidth,(gl_FragCoord.y+1)/winHeight)) +
		   texture2D (texture, vec2((gl_FragCoord.x-3)/winWidth,(gl_FragCoord.y+3)/winHeight)) +
		   texture2D (texture, vec2((gl_FragCoord.x-1)/winWidth,(gl_FragCoord.y+3)/winHeight)) +
		   texture2D (texture, vec2((gl_FragCoord.x+1)/winWidth,(gl_FragCoord.y+3)/winHeight)) +
		   texture2D (texture, vec2((gl_FragCoord.x+3)/winWidth,(gl_FragCoord.y+3)/winHeight)))*k*k

		   )/(1.0 + k*(8.0 + 8.0 + 12.0*k));
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float k = 1/(texture2D (depth, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y)/winHeight)).x) - 1/focus;
	if(k<0)
		k = -k;
	k = k/(1.0-focus);
	k = k*k;
	float t = sqrt(((gl_FragCoord.x*2.0 - winWidth)/winWidth)*((gl_FragCoord.x*2.0 - winWidth)/winWidth)
		 + ((gl_FragCoord.y*2.0 - winHeight)/winHeight)*((gl_FragCoord.y*2.0 - winHeight)/winHeight));
	k=k*0.1 + k*t*t*0.9;

	if(glass<0.5)
	{
		if(gl_FragCoord.x>3 && gl_FragCoord.x<winWidth-13 && gl_FragCoord.y>3 && gl_FragCoord.y<winHeight-13)
			gl_FragColor = conv(k);
		else
			gl_FragColor = texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y)/winHeight));
			
	}
	else
	{
		float deep = texture2D (depth, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y)/winHeight)).x;
		vec4 point = vec4(gl_FragCoord.x,gl_FragCoord.y,deep,1.0);

		float r1 = rand(point.xy+vec2(time,time));
		float r2 = rand(point.yx+vec2(0.81*time,0.13*time));
		point.x += 150.0*point.z*point.z*(1.0-sqrt(point.z))*(1.0 + 3.0*(1.0-point.z))*(r1);
		point.y += 30.0*point.z*point.z*(1.0-sqrt(point.z))*(1.0 + 3.0*(1.0-point.z))*(r2);
		if(point.x<0 || point.x/winWidth>0.99)
		{
			point.x -= 150.0*(1.0-sqrt(point.z))*(1.0 + 3.0*(1.0-point.z))*(r1);
		}
		if(point.y<0 || point.y>winHeight)
		{
			point.y -= 30.0*(1.0-sqrt(point.z))*(1.0 + 3.0*(1.0-point.z))*(r2);
		}
		gl_FragColor = texture2D (texture, vec2(point.x/winWidth,point.y/winHeight));
	}

	/*if(doBlur>0)
		if(time<1.5)
		{
			gl_FragColor.r = texture2D (texture, vec2((gl_FragCoord.x+time*3)/winWidth,(gl_FragCoord.y)/winHeight)).r;
			gl_FragColor.g = texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y-time*3)/winHeight)).g;
		}
		else
		{
			gl_FragColor.r = texture2D (texture, vec2((gl_FragCoord.x+(3.0-time)*3)/winWidth,(gl_FragCoord.y)/winHeight)).r;
			gl_FragColor.g = texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y-(3.0-time)*3)/winHeight)).g;
		}*/
}
