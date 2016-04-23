#define YSGLSL_TEX_TYPE_NONE                0
#define YSGLSL_TEX_TYPE_TILING              1
#define YSGLSL_TEX_TYPE_BILLBOARD           2
#define YSGLSL_TEX_TYPE_3DTILING            3
#define YSGLSL_TEX_TYPE_ATTRIBUTE           4
#define YSGLSL_TEX_BILLBOARD_PERS           0
#define YSGLSL_TEX_BILLBOARD_ORTHO          1
#define YSGLSL_TEX_WRAP_CUTOFF              0
#define YSGLSL_TEX_WRAP_REPEAT              1
#define YSGLSL_BILLBOARD_CLIPPING_NONE      0
#define YSGLSL_BILLBOARD_CLIPPING_CIRCLE    1
#define YSGLSL_POINTSPRITE_CLIPPING_NONE      0
#define YSGLSL_POINTSPRITE_CLIPPING_CIRCLE    1
#define YSGLSL_MARKER_TYPE_PLAIN            0
#define YSGLSL_MARKER_TYPE_CIRCLE           1
#define YSGLSL_MARKER_TYPE_RECT             2
#define YSGLSL_MARKER_TYPE_STAR             3
#define YSGLSL_MARKER_TYPE_EMPTY_RECT       4
#define YSGLSL_MARKER_TYPE_EMPTY_CIRCLE     5
#define YSGLSL_MARKER_TYPE_EMPTY_STAR       6
#define YSGLSL_MAX_NUM_LIGHT                8
#define YSGLSL_POINTSPRITE_SIZE_IN_PIXEL    0
#define YSGLSL_POINTSPRITE_SIZE_IN_3DSPACE  1
#define YSGLSL_RENDERER_TYPE_NONE           0
#define YSGLSL_RENDERER_TYPE_PLAIN2D        1
#define YSGLSL_RENDERER_TYPE_MARKER2D       2
#define YSGLSL_RENDERER_TYPE_POINTSPRITE2D  3
#define YSGLSL_RENDERER_TYPE_PLAIN3D        10
#define YSGLSL_RENDERER_TYPE_SHADED3D       11
#define YSGLSL_RENDERER_TYPE_FLASH          12
#define YSGLSL_RENDERER_TYPE_MARKER3D       13
#define YSGLSL_RENDERER_TYPE_POINTSPRITE3D  14
#define YSGLSL_SHADOWMAP_NONE               0
#define YSGLSL_SHADOWMAP_USE                1
#define YSGLSL_SHADOWMAP_DEBUG              2

#ifdef GL_ES
	#define LOWP lowp
	#define MIDP mediump
	#define HIGHP highp
#else
	#define LOWP
	#define MIDP
	#define HIGHP
#endif


uniform  MIDP  float radius1;
uniform  MIDP  float radius2;
uniform  MIDP  float maxIntensity;
uniform  MIDP  float angleInterval;
uniform  MIDP  float angleOffset;
uniform  MIDP  float exponent;

varying  LOWP  vec2 uv;
varying  LOWP  vec4 color;

// No alpha cut off in the flash renderer.
// Variables for fog
uniform bool fogEnabled;
uniform  MIDP  float fogDensity;
uniform  LOWP  vec4 fogColor;
varying  HIGHP  float zInViewCoord;


void main()
{
	const  MIDP  float PI=3.1415927;
	const  MIDP  float halfPI=3.1415927/2.0;
	const  MIDP  float twoPI=3.1415927*2.0;
	 MIDP  float r=length(uv);
	if(r<=radius1)
	{
		 MIDP  float param=halfPI*(radius1-r)/radius1;
		 MIDP  float additive=sin(param)*maxIntensity;
		gl_FragColor[0]=min(1.0,color[0]+additive);
		gl_FragColor[1]=min(1.0,color[1]+additive);
		gl_FragColor[2]=min(1.0,color[2]+additive);
		gl_FragColor[3]=color[3];

		if(false!=fogEnabled)
		{
			 MIDP  float d=fogDensity*abs(zInViewCoord);
			 MIDP  float f=clamp(exp(-d*d),0.0,1.0);
			 LOWP  vec3 fogMix=mix(fogColor.rgb,gl_FragColor.rgb,f);
			gl_FragColor[0]=fogMix[0];
			gl_FragColor[1]=fogMix[1];
			gl_FragColor[2]=fogMix[2];
		}

	}
	else
	{
		 MIDP  float halfInterval=angleInterval/2.0;
		 MIDP  float theata=twoPI+angleOffset+atan(uv.y,uv.x);
		 MIDP  float t=mod(theata,angleInterval);
		 MIDP  float s=1.0-abs(t-halfInterval)/halfInterval;

		 MIDP  float rf=radius1+(radius2-radius1)*pow(s,exponent);
		if(rf<r)
		{
			discard;
		}
		else
		{
			 MIDP  float alpha=max(1.0-(r-radius1)/(rf-radius1),0.0);
			gl_FragColor[0]=color[0];
			gl_FragColor[1]=color[1];
			gl_FragColor[2]=color[2];
			gl_FragColor[3]=color[3]*alpha;

			if(false!=fogEnabled)
			{
				 MIDP  float d=fogDensity*abs(zInViewCoord);
				 MIDP  float f=clamp(exp(-d*d),0.0,1.0);
				 LOWP  vec3 fogMix=mix(fogColor.rgb,gl_FragColor.rgb,f);
				gl_FragColor[0]=fogMix[0];
				gl_FragColor[1]=fogMix[1];
				gl_FragColor[2]=fogMix[2];
			}

		}
    }
}
