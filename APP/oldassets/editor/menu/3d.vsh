#ifdef GL_ES
precision highp float;
#endif

attribute vec3 inPosition;   
attribute vec2 inTexcoord; 

varying vec2 UV;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	gl_Position.xyzw = P * V * M * vec4(inPosition,1);
	UV=inTexcoord;
}
