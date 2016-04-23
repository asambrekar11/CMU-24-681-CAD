// Variables for texturing
uniform  LOWP  int textureType;
uniform  LOWP  mat4 textureTileTransform;
uniform int billBoardTransformType;
uniform bool useNegativeOneToPositiveOneTexCoord;
uniform  HIGHP  vec3 billBoardCenter;
uniform  HIGHP  vec2 billBoardDimension;
uniform sampler2D textureIdent;
varying  HIGHP  vec3 texCoordOut;
