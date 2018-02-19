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
	float bias = 1.2;
	gl_Position.xyzw = P * V * M * vec4(vec3(inPosition.x,(1.0-inPosition.y)-2.0,inPosition.z*bias),1.0);
	UV=vec2((inTexcoord.x-0.5)*bias+0.5,inTexcoord.y);
}
