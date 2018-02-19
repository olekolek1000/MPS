#ifdef GL_ES
precision highp float;
#endif

attribute vec2 inPosition;   
attribute vec4 inColor; 

varying vec4 color;

uniform mat4 P;
uniform mat4 M;

void main()
{
	color = inColor;
	gl_Position.xyzw = P * M * vec4(inPosition,0,1);
}
