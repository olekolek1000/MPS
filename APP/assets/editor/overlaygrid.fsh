#ifdef GL_ES
precision highp float;
#endif

varying vec2 UV;

uniform vec2 OVERLAY_RES;

uniform float ZOOM; 

void main()
{
	float grid_size = 0.03;
	
	float intensity = 1.0;
	
	vec2 overlaypos = vec2(UV.x*OVERLAY_RES.x+grid_size/2.0,UV.y*OVERLAY_RES.y+grid_size/2.0); 
	
	if(mod(overlaypos.x,1.0)<grid_size || mod(overlaypos.y,1.0)<grid_size){
		
		intensity = 1.0-clamp((ZOOM-20.0)/10.0,0.0,0.5);
	}
	
	float brightness = mod(UV.x*OVERLAY_RES.x + UV.y*OVERLAY_RES.y,1.0);
	
	gl_FragColor.rgba = vec4(brightness, brightness, brightness, 1.0-intensity);
}
