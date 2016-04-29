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


uniform bool lightEnabled[YSGLSL_MAX_NUM_LIGHT];
uniform  HIGHP vec4 lightPos[YSGLSL_MAX_NUM_LIGHT];
uniform  LOWP vec3 lightColor[YSGLSL_MAX_NUM_LIGHT];
uniform  LOWP vec3 specularColor;
uniform  MIDP float specularExponent;
uniform  LOWP vec3 ambientColor;

varying  HIGHP  vec3 normalOut;
varying  HIGHP  vec3 vecToCameraOut;
varying  LOWP  vec4 color;


LOWP vec4 YsGLSLRainbowColor(HIGHP float t)
{
	// 0     0.25  0.5    0.75    1
	// Blue->Cyan->Green->Yellow->Red

	HIGHP float tt;
	if(t<0.0)
	{
		return vec4(0,0,1,1);
	}
	else if(t<0.25)
	{
		tt=t/0.25;
		return vec4(0,tt,1,1);
	}
	else if(t<0.5)
	{
		tt=(t-0.25)/0.25;
		return vec4(0,1,1.0-tt,1);
	}
	else if(t<0.75)
	{
		tt=(t-0.5)/0.25;
		return vec4(tt,1,0,1);
	}
	else if(t<1.0)
	{
		tt=(t-0.75)/0.25;
		return vec4(1,1.0-tt,0,1);
	}
	else
	{
		return vec4(1,0,0,1);
	}
}



// Variables for texturing
uniform  LOWP  int textureType;
uniform  LOWP  mat4 textureTileTransform;
uniform int billBoardTransformType;
uniform bool useNegativeOneToPositiveOneTexCoord;
uniform  HIGHP  vec3 billBoardCenter;
uniform  HIGHP  vec2 billBoardDimension;
uniform sampler2D textureIdent;
varying  HIGHP  vec3 texCoordOut;

// Variables for alpha cutoff
uniform  LOWP  float alphaCutOff;

// Variables for fog
uniform bool fogEnabled;
uniform  MIDP  float fogDensity;
uniform  LOWP  vec4 fogColor;
varying  HIGHP  float zInViewCoord;


uniform LOWP int shadowMapMode;
uniform HIGHP mat4 shadowMapTransform;
uniform sampler2D shadowMapTexture;

varying HIGHP vec4 shadowCoord;


void main()
{
	 HIGHP vec3 nomLocal=normalize(normalOut);
	 HIGHP vec3 vecToCamera=normalize(vecToCameraOut);

	// Lighting
	//	Input variables: vec4 color
	//	                 vec3 nomLocal
	//	                 vec3 vecToCamera <- must be normalized
	//	Output variable: vec4 accumColor
	
	 HIGHP  vec3 accumColor;
	accumColor=ambientColor*color.xyz;
	
	for(int lightNo=0; lightNo<YSGLSL_MAX_NUM_LIGHT; lightNo++)
	{
		if(true==lightEnabled[lightNo])
		{
			 MIDP  float diffuseIntensity=max(dot(nomLocal,lightPos[lightNo].xyz),0.0);
			accumColor+=diffuseIntensity*(color.xyz*lightColor[lightNo]);
	
			if(0.01<specularColor.r || 0.01<specularColor.g || 0.01<specularColor.b)
			{
				 HIGHP  vec3 unitVecToCamera=normalize(vecToCamera);
				 HIGHP  vec3 mid=normalize(lightPos[lightNo].xyz+unitVecToCamera);
				 HIGHP  float specularIntensity=pow(max(dot(mid,nomLocal),0.0),specularExponent);
				accumColor+=specularIntensity*specularColor;
			}
		}
	}


	gl_FragColor=vec4(accumColor,color[3]);


	// Shadow Map (Tentative) >>
	if(YSGLSL_SHADOWMAP_NONE==shadowMapMode)
	{
	}
	else if(YSGLSL_SHADOWMAP_USE==shadowMapMode ||
	        YSGLSL_SHADOWMAP_DEBUG==shadowMapMode)
	{
		HIGHP vec3 shadowCoordTfm=shadowCoord.xyz/shadowCoord.w;
		shadowCoordTfm=(shadowCoordTfm+vec3(1,1,1))/2.0;
		if(0.0<shadowCoordTfm.x && shadowCoordTfm.x<1.0 &&
		   0.0<shadowCoordTfm.y && shadowCoordTfm.y<1.0)
		{
			HIGHP float depth;
			depth=texture2D(shadowMapTexture,shadowCoordTfm.xy).r;
			gl_FragColor=YsGLSLRainbowColor(depth);
		}
		else
		{
			gl_FragColor=vec4(1,0,1,1);
		}
	}
	// Shadow Map (Tentative) <<


	if(YSGLSL_TEX_TYPE_NONE==textureType)
	{
	}
	else if(YSGLSL_TEX_TYPE_TILING==textureType ||
	        YSGLSL_TEX_TYPE_BILLBOARD==textureType ||
	        YSGLSL_TEX_TYPE_ATTRIBUTE==textureType)
	{
		 LOWP  vec4 texcell=texture2D(textureIdent,texCoordOut.xy);
		gl_FragColor=gl_FragColor*texcell;
	}
	else if(YSGLSL_TEX_TYPE_3DTILING==textureType)
	{
		 LOWP  vec4 texcell[3],avg;
		texcell[0]=texture2D(textureIdent,texCoordOut.xy);
		texcell[1]=texture2D(textureIdent,texCoordOut.xz);
		texcell[2]=texture2D(textureIdent,texCoordOut.yz);
		avg=texcell[0]/3.0+texcell[1]/3.0+texcell[2]/3.0;
		gl_FragColor=gl_FragColor*avg;
	}
	

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
