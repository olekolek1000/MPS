#ifdef GL_ES
precision highp float;
#endif

varying vec2 UV;

uniform vec2 OVERLAY_RES;
uniform float ZOOM;

void main()
{
	float intensity=0.99;
	
	vec2 overlaypos = vec2(UV.x*OVERLAY_RES.x,UV.y*OVERLAY_RES.y); 
	if(mod(floor(overlaypos.x)+floor(overlaypos.y),2.0)==1.0){
		intensity=0.95;
	}
		
	gl_FragColor.rgba = vec4(intensity, intensity, intensity, 1.0);
}
