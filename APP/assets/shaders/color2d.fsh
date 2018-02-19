#ifdef GL_ES
precision highp float;
#endif

uniform vec4 COLOR;

void main()
{
	gl_FragColor.rgba = COLOR;
}
