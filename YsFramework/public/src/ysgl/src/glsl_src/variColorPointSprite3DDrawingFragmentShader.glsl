#include "YS_GLSL_HEADER.glsl"

uniform sampler2D textureIdent;

varying  MIDP  vec2 texCoordOut;
varying  LOWP  vec4 color;

#include "YS_GLSL_FOG_VARIABLES.glsl"
#include "YS_GLSL_ALPHA_CUTOFF_PARAMETER.glsl"
#include "YS_GLSL_POINT_SPRITE_VARIABLE.glsl"

void main()
{
	if(YSGLSL_POINTSPRITE_CLIPPING_CIRCLE==pointClippingType)
	{
		 MIDP  vec2 t=(gl_PointCoord.xy*2.0)-vec2(1.0,1.0);
		if(1.0<=dot(t,t))
		{
			discard;
		}
	}

	if(YSGLSL_TEX_TYPE_TILING==textureType ||
	   YSGLSL_TEX_TYPE_BILLBOARD==textureType)
	{
		 MIDP  vec2 texCoord=texCoordOut+gl_PointCoord*texCoordRange;
		 LOWP  vec4 texcell=texture2D(textureIdent,texCoord);
		gl_FragColor=color*texcell;
	}
	else if(YSGLSL_TEX_TYPE_NONE==textureType)
	{
		gl_FragColor=color;
	}
    else
    {
        gl_FragColor=vec4(0,0,0,1);
    }

	// gl_FragColor.rg=gl_PointCoord;  For debugging.

	#include "YS_GLSL_ALPHA_CUTOFF_IN_FRAGMENT_SHADER.glsl"
	#include "YS_GLSL_FOGCALCULATION_IN_FRAGMENTSHADER.glsl"
}
