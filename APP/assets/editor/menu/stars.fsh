#ifdef GL_ES
precision highp float;
#endif

varying float intensity;
varying vec2 UV;

uniform sampler2D tex;

uniform float TIME;
uniform vec2 RESOLUTION;


void main()
{
	gl_FragColor.rgba = texture2D(tex, UV).rgba;
	gl_FragColor.a*=intensity;

}
