#include "YS_GLSL_HEADER.glsl"

uniform bool lightEnabled[YSGLSL_MAX_NUM_LIGHT];
uniform  HIGHP vec4 lightPos[YSGLSL_MAX_NUM_LIGHT];
uniform  LOWP vec3 lightColor[YSGLSL_MAX_NUM_LIGHT];
uniform  LOWP vec3 specularColor;
uniform  MIDP float specularExponent;
uniform  LOWP vec3 ambientColor;

varying  HIGHP  vec3 normalOut;
varying  HIGHP  vec3 vecToCameraOut;
varying  LOWP  vec4 color;

#include "YS_GLSL_UTIL.glsl"

#include "YS_GLSL_TEXTURE_VARIABLE.glsl"
#include "YS_GLSL_ALPHA_CUTOFF_PARAMETER.glsl"
#include "YS_GLSL_FOG_VARIABLES.glsl"

#include "YS_GLSL_SHADOWMAP_UNIFORM.glsl"
#include "YS_GLSL_SHADOWMAP_VARYING.glsl"

void main()
{
	 HIGHP vec3 nomLocal=normalize(normalOut);
	 HIGHP vec3 vecToCamera=normalize(vecToCameraOut);

	#include "YS_GLSL_STANDARD_LIGHTING.glsl"

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


	#include "YS_GLSL_TEXTURE_IN_FRAGMENTSHADER.glsl"
	#include "YS_GLSL_ALPHA_CUTOFF_IN_FRAGMENT_SHADER.glsl"
	#include "YS_GLSL_FOGCALCULATION_IN_FRAGMENTSHADER.glsl"
}
