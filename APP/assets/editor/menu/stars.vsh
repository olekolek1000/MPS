#ifdef GL_ES
precision highp float;
#endif

attribute vec3 position;
attribute vec2 inUV;
attribute float starData;   

uniform mat4 V;
uniform mat4 P;

varying float intensity;
varying vec2 UV;

void main()
{
	gl_Position = P * V * vec4(position.xyz,1.0);
	
	intensity = starData;
	
	UV = inUV;

}
