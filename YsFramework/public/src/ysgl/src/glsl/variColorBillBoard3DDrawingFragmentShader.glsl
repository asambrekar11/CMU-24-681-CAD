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


uniform int pointClippingType;
uniform int billBoardTransformType;
uniform sampler2D textureIdent;

varying  HIGHP  vec2 texCoordOut;
varying  LOWP  vec4 color;

// Variables for fog
uniform bool fogEnabled;
uniform  MIDP  float fogDensity;
uniform  LOWP  vec4 fogColor;
varying  HIGHP  float zInViewCoord;

// Variables for alpha cutoff
uniform  LOWP  float alphaCutOff;


void main()
{
	if(YSGLSL_BILLBOARD_CLIPPING_CIRCLE==pointClippingType)
	{
		 MIDP  vec2 nomLocal=(texCoordOut.xy*2.0)-vec2(1.0,1.0);
		if(1.0<=dot(nomLocal,nomLocal))
		{
			discard;
		}
	}

	 LOWP  vec4 texcell=texture2D(textureIdent,texCoordOut);
	gl_FragColor=color*texcell;

	if(gl_FragColor.a<alphaCutOff)
	{
		discard;
	}

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
