#ifdef GL_ES
precision highp float;
#endif

uniform sampler2D tex;
varying vec2 UV;

uniform float TIME;

const int blurquality = 4;

void main()
{
	float mul = 1.0-UV.y;
	
	float blur_size = (1.0-mul)*0.013;
	
	vec2 fUV;
	vec4 fCOLOR = vec4(0.0,0.0,0.0,0.0);
	int count=0;
	for(int y=-blurquality;y<blurquality;y++){
		for(int x=-blurquality;x<blurquality;x++){
			fUV = vec2(
						UV.x + float(x)*blur_size, 
			            UV.y + float(y)*blur_size
			          );
			fCOLOR += texture2D(tex,fUV).rgba;
			count++;
		}
	}
	fCOLOR=fCOLOR/float(count);
	
	gl_FragColor.rgba = vec4(fCOLOR.r-0.1,fCOLOR.g-0.05,fCOLOR.b,fCOLOR.a)*mul;
}
