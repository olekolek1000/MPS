#ifdef GL_ES
precision highp float;
#endif

attribute vec4 starPos;   

uniform mat4 V;
uniform mat4 P;


varying float intensity;

void main()
{
	gl_Position = P * V * vec4(starPos.xyz,1.0);
	
	gl_PointSize = 1.0;
	
	intensity = starPos.w;
}
