#ifdef GL_ES
precision highp float;
#endif

attribute vec2 inPosition;   
attribute vec2 inTexcoord; 

varying vec2 UV;

uniform mat4 M;
uniform mat4 P;

void main()
{
	gl_Position.xyzw = P * M * vec4(inPosition,0,1);
	UV=inTexcoord;
}
