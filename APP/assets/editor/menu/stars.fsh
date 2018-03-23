#ifdef GL_ES
precision highp float;
#endif

varying float intensity;

uniform float TIME;
uniform vec2 RESOLUTION;

void main()
{
	gl_FragColor.rgba = vec4(1.0,1.0,1.0,intensity);
}
