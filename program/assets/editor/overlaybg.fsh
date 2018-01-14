#ifdef GL_ES
precision highp float;
#endif

varying vec2 UV;

uniform vec2 OVERLAY_RES;
uniform float ZOOM;

void main()
{
	float intensity=0.95;
	
	vec2 overlaypos = vec2(UV.x*OVERLAY_RES.x,UV.y*OVERLAY_RES.y); 
	if(mod(floor(overlaypos.x)+floor(overlaypos.y),2.0)==1.0){
		intensity=0.90;
	}
	/*
	if(ZOOM<0.2){
		if(
		mod(float(int(UV.x * OVERLAY_RES.x + UV.y * OVERLAY_RES.y)), 2.0) == 0.0
		){
			intensity = min(0.90, 0.95);
		}
	}
	else{
		if(mod(gl_FragCoord.x + gl_FragCoord.y,12.0)<6.0){
			intensity = 0.90;
		}
	}*/
	
	gl_FragColor.rgba = vec4(intensity, intensity, intensity, 1.0);
}
