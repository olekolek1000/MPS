#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tex;
varying vec2 UV;

uniform vec2 OVERLAY_RES;

uniform float ALPHA;

uniform float TIME;

void main()
{
	gl_FragColor.rgba = texture2D(tex,UV).rgba;
	gl_FragColor.a *= ALPHA;
}
