#include "YS_GLSL_HEADER.glsl"

uniform  HIGHP  mat4 transform;

attribute  LOWP  vec2 vertex;
attribute  LOWP  vec4 colorIn;

varying  LOWP  vec4 colorOut;

void main()
{
	vec4 vertex4d=vec4(vertex,0,1);
	gl_Position=transform*vertex4d;
	colorOut=colorIn;
}
