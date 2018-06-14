#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tekstura;
varying vec2 UV;

void main()
{
	gl_FragColor.rgba = texture2D(tekstura,UV).rgba;
}
