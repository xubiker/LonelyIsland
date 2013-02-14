#version 330

uniform sampler2D texture;

uniform float time;
uniform int doBlur;
uniform float winWidth;
uniform float winHeight;


void main()
{
	gl_FragColor = texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y)/winHeight));
	if(doBlur>0)
		if(time<1.5)
		{
			gl_FragColor.r = texture2D (texture, vec2((gl_FragCoord.x+time*5)/winWidth,(gl_FragCoord.y)/winHeight)).r;
			gl_FragColor.g = texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y-time*5)/winHeight)).g;
		}
		else
		{
			gl_FragColor.r = texture2D (texture, vec2((gl_FragCoord.x+(3.0-time)*5)/winWidth,(gl_FragCoord.y)/winHeight)).r;
			gl_FragColor.g = texture2D (texture, vec2((gl_FragCoord.x)/winWidth,(gl_FragCoord.y-(3.0-time)*5)/winHeight)).g;
		}
}
