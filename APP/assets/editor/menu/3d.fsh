#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tex;
varying vec2 UV;

uniform float TIME;

uniform float PROP;

void main()
{
	float intensity = 1.0;
	float border = 600.0;
	
	//left
	intensity *= min( UV.x*border ,1.0);
	//top
	intensity *= min( (1.0-UV.y)*border/PROP, 1.0);
	//right
	intensity *= min( (1.0-UV.x)*border, 1.0);
	
	
	gl_FragColor.rgba = texture2D(tex,UV).rgba * intensity;
}
