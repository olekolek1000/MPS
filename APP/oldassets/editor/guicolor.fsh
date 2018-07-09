#ifdef GL_ES
precision highp float;
#endif

varying vec2 UV;

uniform vec4 COLOR;

void main()
{
	gl_FragColor.rgba = COLOR;
}
