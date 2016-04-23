#include "YS_GLSL_HEADER.glsl"

uniform  HIGHP mat4 projection;
uniform  HIGHP mat4 modelView;

uniform bool lightEnabled[YSGLSL_MAX_NUM_LIGHT];
uniform  HIGHP vec4 lightPos[YSGLSL_MAX_NUM_LIGHT];
uniform  LOWP vec3 lightColor[YSGLSL_MAX_NUM_LIGHT];
uniform  LOWP vec3 specularColor;
uniform  MIDP float specularExponent;
uniform  LOWP vec3 ambientColor;

attribute  HIGHP  vec3 vertex;
attribute  HIGHP  vec3 normal;
attribute  LOWP   vec4 color;    // <- Only difference from monoColorPerVtxShader
attribute  HIGHP  vec2 texCoord;

varying  LOWP  vec4 colorOut;

#include "YS_GLSL_TEXTURE_VARIABLE.glsl"
#include "YS_GLSL_ZOFFSET_VARIABLE.glsl"
#include "YS_GLSL_FOG_VARIABLES.glsl"

void main()
{
	#include "YS_GLSL_VIEW_TRANSFORM_IN_VERTEX_SHADER.glsl"
	#include "YS_GLSL_PROJECTION_TRANSFORM_IN_VERTEX_SHADER.glsl"

	 HIGHP  vec3 nomLocal=normalize((modelView*vec4(normal,0)).xyz);
	 HIGHP  vec3 vecToCamera=-normalize(vertexInView.xyz);

	#include "YS_GLSL_STANDARD_LIGHTING.glsl"

	colorOut=vec4(accumColor,color[3]);

	#include "YS_GLSL_TEXTURE_IN_VERTEXSHADER.glsl"
	#include "YS_GLSL_FOG_CALCULATION_IN_VERTEXSHADER.glsl"
	#include "YS_GLSL_ZOFFSET_IN_VERTEXSHADER.glsl"
}
