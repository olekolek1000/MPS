#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tex;
varying vec2 UV;

uniform float TIME;

const float limit = 0.9;

void main()
{
	//todo AA
	float alpha = 0.0;
	float dist = sqrt(pow((UV.x-0.5)*2.0, 2.0) + pow((UV.y-0.5)*2.0, 2.0));
	if(dist>limit&&dist<1.0){
		alpha = 0.5;
	}
	if(alpha<0.05){
		discard;
	}
	else{
		float intensity;
		if(dist<(limit+1.0)/2.0){
			intensity = clamp(sin(atan2(UV.x-0.5, UV.y-0.5)*12.0+TIME*2.0)*5.0, 0.8, 1.0);
		}
		else{
			intensity = 0.0;
		}
		gl_FragColor.rgba = vec4(intensity, intensity, intensity, alpha);
	}
}
