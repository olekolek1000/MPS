#ifdef GL_ES
precision highp float;
#endif

attribute vec2 inPosition;   
uniform mat4 M;
uniform mat4 P;

void main()
{
	gl_Position.xyzw = P * M * vec4(inPosition,0,1);
}
