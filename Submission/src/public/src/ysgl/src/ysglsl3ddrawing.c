#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ysglsl3ddrawing.h"
#include "ysglslutil.h"
#include "ysglmath.h"
#include "ysglbitmap.h"
#include "ysgldef.h"
#include "ysglslprogram.h"

/* Force Visual C++ to type-mismatching error. */
#pragma warning( error : 4028)
#pragma warning( error : 4047)


/*!
Memo: 2014/07/15
  Until one of my friends kindly told that glVertexAttrib function sets generic attribute in OpenGL ES 2.0,
  I didn't know that there was such thing called generic attribute.  :-P
  
  That's why I wrote single color (mono color) and gradation (vari color) programs separately.

  I'm going to unify them in the future, to simplify things.


2015/11/22
  Found another article.
    http://stackoverflow.com/questions/13348885/why-does-opengl-drawing-fail-when-vertex-attrib-array-zero-is-disabled
  Maybe that's it.  If I make sure one of the required attributes use attribute position 0, 
  maybe I can use generic attributes on a defective OpenGL drivers.
 
  Potentially eliminates the necessity to have vari-color and mono-color renderers.
  Also I should be able to add view-offset and pixel-offset on all shader programs.
*/


struct YsGLSL3DRenderer
{
	int inUse;

	// Shader and Program
	GLuint programId;
	GLuint vertexShaderId,fragmentShaderId;

	// Uniforms
	GLint uniformProjectionPos;
	GLint uniformModelViewPos;
	GLint uniformOffsetTransformPos;

	GLint uniformViewportWidthPos;
	GLint uniformViewportHeightPos;
	GLint uniformViewportOriginPos;

	GLint uniformMarkerTypePos;              // 0:Plain    1:Filled Circle   2:Filled Rect

	GLint uniformLightEnabledPos;     // -1 if no lighting   bool lightEnabled[YSGLMAXNUMLIGHT]
	GLint uniformLightPositionPos;    // -1 if no lighting   vec4 lightPos[YSGLMAXNUMLIGHT]
	GLint uniformLightColorPos;       // -1 if no lighting   vec4 lightColor[YSGLMAXNUMLIGHT]
	GLint uniformSpecularColorPos;    // -1 if no lighting   vec3 specular
	GLint uniformSpecularExponentPos; // -1 if no lighting   float specularExponent
	GLint uniformAmbientColorPos;     // 
	GLint uniformAlphaCutOffPos;

	GLint uniformFogEnabledPos;
	GLint uniformFogDensityPos;
	GLint uniformFogColorPos;

	GLint uniformDimensionPos;  // For flash and marker renderers
	GLint uniformRadius1Pos;  // For flash renderer
	GLint uniformRadius2Pos;  // For flash renderer
	GLint uniformMaxIntensityPos;  // For flash renderer
	GLint uniformAngleIntervalPos;  // For flash renderer
	GLint uniformAngleOffsetPos;  // For flash renderer
	GLint uniformExponentPos;  // For flash renderer

	GLint uniformColorPos;       // -1 if VariColor
	GLfloat uniformColor[4];

	// Texture mapping >>
	GLint uniformTextureTypePos;
	GLint uniformUseNegativeOneToPositiveOnTexCoordPos;

	GLint uniformTextureTileTransformPos;
	GLint uniformTexturePos;

	// GLint uniformBillBoardClippingTypePos;  2015/11/22 billBoardClippingType and pointClippingType are merged.
	GLint uniformBillBoardCenterPos;
	GLint uniformBillBoardTransformTypePos;
	GLint uniformBillBoardDimensionPos;
	// Texture mapping <<

	GLint uniformUseZOffsetPos;
	GLint uniformZOffsetPos;

	GLint uniformPointSizeModePos;
	GLint uniformPointClippingTypePos;
	// GLint uniformTextureEnabledPos;  2015/11/22 Merged with textureType
	GLint uniformTexCoordRangePos;

	// For programs that support uniformPointSize;
	GLint uniformPointSizePos;

	// Memo for adding uniforms:
	//   Uniform position must be set to -1 in YsGLSLCreateAndInitialize3DRenderer

	// Shadow map
	GLint uniformShadowMapModePos;
	GLint uniformShadowMapTransformPos;
	GLint uniformShadowMapTexturePos;




	// Attributes
	GLint attribVertexPos;
	GLint attribColorPos;        // -1 if MonoColor
	YSBOOL colorArrayEnabled;
	GLint attribNormalPos;       // -1 if no normal
	GLint attribTexCoordPos;     // -1 if no tex coord
	YSBOOL texCoordArrayEnabled;
	GLint attribViewOffsetPos;   // -1 if no view offset (offset vector in the view coordinate)
	GLint attribPixelOffsetPos;  // -1 if no pixel offset
	GLint attribPointSizePos;

	// Memo for adding attributes:
	//   Attribute position must be set to -1 in YsGLSLCreateAndInitialize3DRenderer
	//   Attributes array needs to be enabled in YsGLSLUse3DRenderer and disabled in YsGLSLEndUse3DRenderer.
	//   When an attribute is added, make sure enabling/disabling are also added in the above functions.


	// Saving transformations
	double projectionMatrix[16];
	double modelViewMatrix[16];

	// Saving lighting
	GLfloat ambientColor[3];
	GLint lightEnabled[YSGLSL_MAX_NUM_LIGHT];
	GLfloat lightPosition[4*YSGLSL_MAX_NUM_LIGHT];
	GLfloat lightColor[3*YSGLSL_MAX_NUM_LIGHT];
	GLfloat specularExponent;
	GLfloat specularColor[3];

	// Saving fog density
	GLfloat fogDensity;
};

static struct YsGLSL3DRenderer *YsGLSLCreateAndInitialize3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer;
	renderer=(struct YsGLSL3DRenderer *)malloc(sizeof(struct YsGLSL3DRenderer));

	renderer->inUse=0;

	renderer->programId=0;
	renderer->vertexShaderId=0;
	renderer->fragmentShaderId=0;

	renderer->uniformProjectionPos=-1;
	renderer->uniformModelViewPos=-1;
	renderer->uniformOffsetTransformPos=-1;
	renderer->uniformAlphaCutOffPos=-1;

	renderer->uniformViewportWidthPos=-1;
	renderer->uniformViewportHeightPos=-1;
	renderer->uniformViewportOriginPos=-1;

	renderer->uniformMarkerTypePos=-1;

	renderer->uniformLightEnabledPos=-1;
	renderer->uniformLightPositionPos=-1;
	renderer->uniformLightColorPos=-1;
	renderer->uniformSpecularColorPos=-1;
	renderer->uniformSpecularExponentPos=-1;
	renderer->uniformAmbientColorPos=-1;

	renderer->uniformFogEnabledPos=-1;
	renderer->uniformFogDensityPos=-1;
	renderer->uniformFogColorPos=-1;

	renderer->uniformDimensionPos=-1;
	renderer->uniformRadius1Pos=-1;
	renderer->uniformRadius2Pos=-1;
	renderer->uniformMaxIntensityPos=-1;
	renderer->uniformAngleIntervalPos=-1;
	renderer->uniformAngleOffsetPos=-1;
	renderer->uniformExponentPos=-1;

	renderer->uniformColorPos=-1;
	renderer->uniformColor[0]=0.0f;
	renderer->uniformColor[1]=0.0f;
	renderer->uniformColor[2]=0.0f;
	renderer->uniformColor[3]=1.0f;

	renderer->uniformTextureTypePos=-1;
	renderer->uniformUseNegativeOneToPositiveOnTexCoordPos=-1;

	renderer->uniformTextureTileTransformPos=-1;
	renderer->uniformTexturePos=-1;

	// renderer->uniformBillBoardClippingTypePos=-1;  2015/11/22 billBoardClippingType and pointClippingType are merged.
	renderer->uniformBillBoardCenterPos=-1;
	renderer->uniformBillBoardTransformTypePos=-1;
	renderer->uniformBillBoardDimensionPos=-1;


	renderer->uniformUseZOffsetPos=-1;
	renderer->uniformZOffsetPos=-1;

	renderer->uniformPointSizeModePos=-1;
	renderer->uniformPointClippingTypePos=-1;
	renderer->uniformTexCoordRangePos=-1;

	renderer->uniformPointSizePos=-1;


	renderer->uniformShadowMapModePos=-1;
	renderer->uniformShadowMapTransformPos=-1;
	renderer->uniformShadowMapTexturePos=-1;



	renderer->attribVertexPos=-1;
	renderer->attribColorPos=-1;
	renderer->colorArrayEnabled=YSFALSE;
	renderer->attribNormalPos=-1;
	renderer->attribTexCoordPos=-1;
	renderer->texCoordArrayEnabled=YSFALSE;
	renderer->attribViewOffsetPos=-1;
	renderer->attribPixelOffsetPos=-1;
	renderer->attribPointSizePos=-1;



	renderer->fogDensity=1.0;

	return renderer;
}

static void YsGLSLGetTextureUniformPos(struct YsGLSL3DRenderer *renderer)
{
	renderer->uniformTextureTypePos=glGetUniformLocation(renderer->programId,"textureType");
	renderer->uniformUseNegativeOneToPositiveOnTexCoordPos=glGetUniformLocation(renderer->programId,"useNegativeOneToPositiveOneTexCoord");

	renderer->uniformTexturePos=glGetUniformLocation(renderer->programId,"textureIdent");

	renderer->uniformTextureTileTransformPos=glGetUniformLocation(renderer->programId,"textureTileTransform");

	renderer->uniformBillBoardCenterPos=glGetUniformLocation(renderer->programId,"billBoardCenter");
	renderer->uniformBillBoardTransformTypePos=glGetUniformLocation(renderer->programId,"billBoardTransformType");
	renderer->uniformBillBoardDimensionPos=glGetUniformLocation(renderer->programId,"billBoardDimension");
}

static void YsGLSLGetShadowMapUniformPos(struct YsGLSL3DRenderer *renderer)
{
	renderer->uniformShadowMapModePos=glGetUniformLocation(renderer->programId,"shadowMapMode");
	renderer->uniformShadowMapTransformPos=glGetUniformLocation(renderer->programId,"shadowMapTransform");
	renderer->uniformShadowMapTexturePos=glGetUniformLocation(renderer->programId,"shadowMapTexture");
}

static void YsGLSLTexCoordIsNotGivenAsArray(struct YsGLSL3DRenderer *renderer)
{
	if(0<=renderer->attribTexCoordPos && YSTRUE==renderer->texCoordArrayEnabled)
	{
		glDisableVertexAttribArray(renderer->attribTexCoordPos);
		renderer->texCoordArrayEnabled=YSFALSE;
		glVertexAttrib2f(renderer->attribTexCoordPos,0.0f,0.0f);
	}
}

static void YsGLSLTexCoordIsGivenAsArray(struct YsGLSL3DRenderer *renderer)
{
	if(0<=renderer->attribTexCoordPos && YSTRUE!=renderer->texCoordArrayEnabled)
	{
		glEnableVertexAttribArray(renderer->attribTexCoordPos);
		renderer->texCoordArrayEnabled=YSTRUE;
	}
}

static void YsGLSLColorIsNotGivenAsArray(struct YsGLSL3DRenderer *renderer)
{
	if(0<=renderer->attribColorPos)
	{
		if(YSTRUE==renderer->colorArrayEnabled)
		{
			glDisableVertexAttribArray(renderer->attribColorPos);
			renderer->colorArrayEnabled=YSFALSE;
		}
		glVertexAttrib4f(renderer->attribColorPos,renderer->uniformColor[0],renderer->uniformColor[1],renderer->uniformColor[2],renderer->uniformColor[3]);
	}
}

static void YsGLSLColorIsGivenAsArray(struct YsGLSL3DRenderer *renderer)
{
	if(0<=renderer->attribColorPos && YSTRUE==renderer->colorArrayEnabled)
	{
		glEnableVertexAttribArray(renderer->attribColorPos);
		renderer->colorArrayEnabled=YSTRUE;
	}
}

void YsGLSLDelete3DRenderer(struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		glDeleteProgram(renderer->programId);
		glDeleteShader(renderer->vertexShaderId);
		glDeleteShader(renderer->fragmentShaderId);

		free(renderer);
	}
}

void YsGLSLSet3DRendererProjectionfv(struct YsGLSL3DRenderer *renderer,const GLfloat mat[16])
{
	if(NULL!=renderer)
	{
		int i;
		glUniformMatrix4fv(renderer->uniformProjectionPos,1,GL_FALSE,mat);
		for(i=0; i<16; ++i)
		{
			renderer->projectionMatrix[i]=mat[i];
		}
	}
}

void YsGLSLSet3DRendererProjectiondv(struct YsGLSL3DRenderer *renderer,const double mat[16])
{
	if(NULL!=renderer)
	{
		int i;
		GLfloat matf[16];
		for(i=0; i<16; ++i)
		{
			matf[i]=(GLfloat)mat[i];
			renderer->projectionMatrix[i]=mat[i];
		}
		glUniformMatrix4fv(renderer->uniformProjectionPos,1,GL_FALSE,matf);
	}
}

void YsGLSLGet3DRendererProjectionfv(GLfloat mat[16],const struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		int i;
		for(i=0; i<16; ++i)
		{
			mat[i]=(GLfloat)renderer->projectionMatrix[i];
		}
	}
}

void YsGLSLGet3DRendererProjectiondv(double mat[16],const struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		int i;
		for(i=0; i<16; ++i)
		{
			mat[i]=renderer->projectionMatrix[i];
		}
	}
}

void YsGLSLSet3DRendererModelViewfv(struct YsGLSL3DRenderer *renderer,const GLfloat mat[16])
{
	if(NULL!=renderer)
	{
		int i;
		for(i=0; i<16; ++i)
		{
			renderer->modelViewMatrix[i]=mat[i];
		}
		glUniformMatrix4fv(renderer->uniformModelViewPos,1,GL_FALSE,mat);
	}
}

void YsGLSLSet3DRendererModelViewdv(struct YsGLSL3DRenderer *renderer,const double mat[16])
{
	if(NULL!=renderer)
	{
		int i;
		GLfloat matf[16];
		for(i=0; i<16; ++i)
		{
			matf[i]=(GLfloat)mat[i];
			renderer->modelViewMatrix[i]=mat[i];
		}
		glUniformMatrix4fv(renderer->uniformModelViewPos,1,GL_FALSE,matf);
	}
}

void YsGLSLGet3DRendererModelViewfv(GLfloat mat[16],const struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		int i;
		for(i=0; i<16; ++i)
		{
			mat[i]=(GLfloat)renderer->modelViewMatrix[i];
		}
	}
}

void YsGLSLGet3DRendererModelViewdv(double mat[16],const struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		int i;
		for(i=0; i<16; ++i)
		{
			mat[i]=renderer->modelViewMatrix[i];
		}
	}
}

YSRESULT YsGLSLSet3DRendererUniformColorfv(struct YsGLSL3DRenderer *renderer,const GLfloat color[4])
{
	if(NULL!=renderer)
	{
		renderer->uniformColor[0]=color[0];
		renderer->uniformColor[1]=color[1];
		renderer->uniformColor[2]=color[2];
		renderer->uniformColor[3]=color[3];
		if(0<=renderer->uniformColorPos)
		{
			glUniform4fv(renderer->uniformColorPos,1,color);
			return YSOK;
		}
		else if(0<=renderer->attribColorPos)
		{
			// May be used as a generic color.
			return YSOK;
		}
		else
		{
			return YSERR;
		}
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererViewportDimensionf(const struct YsGLSL3DRenderer *renderer,const GLfloat width,const GLfloat height)
{
	if(NULL!=renderer && (0<=renderer->uniformViewportWidthPos || 0<=renderer->uniformViewportHeightPos))
	{
		if(0<=renderer->uniformViewportWidthPos)
		{
			glUniform1f(renderer->uniformViewportWidthPos,width);
		}
		if(0<=renderer->uniformViewportHeightPos)
		{
			glUniform1f(renderer->uniformViewportHeightPos,height);
		}
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererAlphaCutOff(const struct YsGLSL3DRenderer *renderer,GLfloat alphaCutOff)
{
	if(NULL!=renderer && 0<=renderer->uniformAlphaCutOffPos)
	{
		glUniform1f(renderer->uniformAlphaCutOffPos,alphaCutOff);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererTextureType(const struct YsGLSL3DRenderer *renderer,int textureType)
{
	if(NULL!=renderer && 0<=renderer->uniformTextureTypePos)
	{
		glUniform1i(renderer->uniformTextureTypePos,textureType);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererTextureIdentifier(const struct YsGLSL3DRenderer *renderer,GLuint textureIdent)
{
	if(NULL!=renderer && 0<=renderer->uniformTexturePos)
	{
		glUniform1i(renderer->uniformTexturePos,textureIdent);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformTextureTilingMatrixfv(const struct YsGLSL3DRenderer *renderer,const GLfloat texTfm[16])
{
	if(NULL!=renderer && 0<=renderer->uniformTextureTileTransformPos)
	{
		glUniformMatrix4fv(renderer->uniformTextureTileTransformPos,1,GL_FALSE,texTfm);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererBillBoardClippingType(const struct YsGLSL3DRenderer *renderer,int billBoardClippingType)
{
	return YsGLSLSet3DRendererPointSpriteClippingType(renderer,billBoardClippingType);
}

YSRESULT YsGLSLSet3DRendererBillBoardTransformType(const struct YsGLSL3DRenderer *renderer,int billBoardTfmType)
{
	if(NULL!=renderer && 0<=renderer->uniformBillBoardTransformTypePos)
	{
		glUniform1i(renderer->uniformBillBoardTransformTypePos,billBoardTfmType);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererBillBoardfv(const struct YsGLSL3DRenderer *renderer,const GLfloat center[3],const GLfloat dimension[2])
{
	if(NULL!=renderer && 0<=renderer->uniformBillBoardCenterPos && 0<=renderer->uniformBillBoardDimensionPos)
	{
		glUniform3fv(renderer->uniformBillBoardCenterPos,1,center);
		glUniform2fv(renderer->uniformBillBoardDimensionPos,1,dimension);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformLightDirectionfv(struct YsGLSL3DRenderer *renderer,int i,const GLfloat dir[3])
{
	if(NULL!=renderer && 0<=i && i<YSGLSL_MAX_NUM_LIGHT && 0<=renderer->uniformLightPositionPos)
	{
		GLdouble l;
		GLdouble dir4d[4];
		dir4d[0]=dir[0];
		dir4d[1]=dir[1];
		dir4d[2]=dir[2];
		dir4d[3]=0.0;

		YsGLMultMatrixVectordv(dir4d,renderer->modelViewMatrix,dir4d);

		l=sqrt(dir4d[0]*dir4d[0]+dir4d[1]*dir4d[1]+dir4d[2]*dir4d[2]);
		renderer->lightPosition[i*4  ]=(GLfloat)(dir4d[0]/l);
		renderer->lightPosition[i*4+1]=(GLfloat)(dir4d[1]/l);
		renderer->lightPosition[i*4+2]=(GLfloat)(dir4d[2]/l);
		renderer->lightPosition[i*4+3]=0.0f;

		glUniform4fv(renderer->uniformLightPositionPos,YSGLSL_MAX_NUM_LIGHT,renderer->lightPosition);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformLightDirectionInCameraCoordinatefv(struct YsGLSL3DRenderer *renderer,int i,const GLfloat dir[3])
{
	if(NULL!=renderer && 0<=i && i<YSGLSL_MAX_NUM_LIGHT && 0<=renderer->uniformLightPositionPos)
	{
		GLdouble l;
		GLdouble dir4d[4];
		dir4d[0]=dir[0];
		dir4d[1]=dir[1];
		dir4d[2]=dir[2];
		dir4d[3]=0.0;

		l=sqrt(dir4d[0]*dir4d[0]+dir4d[1]*dir4d[1]+dir4d[2]*dir4d[2]);
		renderer->lightPosition[i*4  ]=(GLfloat)(dir4d[0]/l);
		renderer->lightPosition[i*4+1]=(GLfloat)(dir4d[1]/l);
		renderer->lightPosition[i*4+2]=(GLfloat)(dir4d[2]/l);
		renderer->lightPosition[i*4+3]=0.0f;

		glUniform4fv(renderer->uniformLightPositionPos,YSGLSL_MAX_NUM_LIGHT,renderer->lightPosition);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererLightColor(struct YsGLSL3DRenderer *renderer,int idx,const GLfloat color[3])
{
	if(NULL!=renderer && 0<=idx && idx<YSGLSL_MAX_NUM_LIGHT && 0<=renderer->uniformLightColorPos)
	{
		renderer->lightColor[idx*3  ]=color[0];
		renderer->lightColor[idx*3+1]=color[1];
		renderer->lightColor[idx*3+2]=color[2];
		glUniform3fv(renderer->uniformLightColorPos,YSGLSL_MAX_NUM_LIGHT,renderer->lightColor);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererAmbientColor(struct YsGLSL3DRenderer *renderer,const GLfloat color[3])
{
	if(NULL!=renderer && 0<=renderer->uniformAmbientColorPos)
	{
		renderer->ambientColor[0]=color[0];
		renderer->ambientColor[1]=color[1];
		renderer->ambientColor[2]=color[2];
		glUniform3fv(renderer->uniformAmbientColorPos,1,color);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererSpecularColor(struct YsGLSL3DRenderer *renderer,const GLfloat color[3])
{
	if(NULL!=renderer && 0<=renderer->uniformSpecularColorPos)
	{
		renderer->specularColor[0]=color[0];
		renderer->specularColor[1]=color[1];
		renderer->specularColor[2]=color[2];
		glUniform3fv(renderer->uniformSpecularColorPos,1,color);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLGet3DRendererSpecularColor(GLfloat color[3],struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer && 0<=renderer->uniformSpecularColorPos)
	{
		color[0]=renderer->specularColor[0];
		color[1]=renderer->specularColor[1];
		color[2]=renderer->specularColor[2];
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererSpecularExponent(struct YsGLSL3DRenderer *renderer,const GLfloat exponent)
{
	if(NULL!=renderer && 0<=renderer->uniformSpecularExponentPos)
	{
		renderer->specularExponent=exponent;
		glUniform1f(renderer->uniformSpecularExponentPos,exponent);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformFlashSize(struct YsGLSL3DRenderer *renderer,GLfloat flashSize)
{
	if(NULL!=renderer && 0<=renderer->uniformDimensionPos)
	{
		glUniform1f(renderer->uniformDimensionPos,flashSize);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererFlashRadius(struct YsGLSL3DRenderer *renderer,GLfloat r1,GLfloat r2)
{
	if(NULL!=renderer && 0<=renderer->uniformRadius1Pos && 0<=renderer->uniformRadius2Pos)
	{
		glUniform1f(renderer->uniformRadius1Pos,r1);
		glUniform1f(renderer->uniformRadius2Pos,r2);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformFogEnabled(struct YsGLSL3DRenderer *renderer,int enableFog)
{
	if(NULL!=renderer && 0<=renderer->uniformFogEnabledPos)
	{
		glUniform1i(renderer->uniformFogEnabledPos,enableFog);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformFogDensity(struct YsGLSL3DRenderer *renderer,const GLfloat density)
{
	if(NULL!=renderer && 0<=renderer->uniformFogDensityPos)
	{
		renderer->fogDensity=density;
		glUniform1f(renderer->uniformFogDensityPos,density);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformFogColor(struct YsGLSL3DRenderer *renderer,const GLfloat fogColor[4])
{
	if(NULL!=renderer && 0<=renderer->uniformFogColorPos)
	{
		glUniform4fv(renderer->uniformFogColorPos,1,fogColor);
		return YSOK;
	}
	return YSERR;
}

GLfloat YsGLSLGet3DRendererFogDensity(struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		return renderer->fogDensity;
	}
	return 0.0;
}

YSRESULT YsGLSLSet3DRendererZOffsetEnabled(struct YsGLSL3DRenderer *renderer,int enableZOffset)
{
	if(NULL!=renderer && 0<=renderer->uniformUseZOffsetPos)
	{
		glUniform1i(renderer->uniformUseZOffsetPos,enableZOffset);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererZOffset(struct YsGLSL3DRenderer *renderer,GLfloat zOffset)
{
	if(NULL!=renderer && 0<=renderer->uniformZOffsetPos)
	{
		glUniform1f(renderer->uniformZOffsetPos,zOffset);
		return YSOK;
	}
	return YSERR;
}

GLuint YsGLSLUse3DRenderer(struct YsGLSL3DRenderer *renderer)
{
	GLuint prevProgramId;
	glGetIntegerv(GL_CURRENT_PROGRAM,(GLint *)&prevProgramId);

	if(NULL!=renderer)
	{
		if(1==renderer->inUse)
		{
			printf("Warning!!!! A 3DRenderer is activated again without being de-activated after last use!!!!\n");
		}

		glUseProgram(renderer->programId);

		if(0<=renderer->attribVertexPos)
		{
			glEnableVertexAttribArray(renderer->attribVertexPos);
		}
		if(0<=renderer->attribColorPos)
		{
			glEnableVertexAttribArray(renderer->attribColorPos);
			renderer->colorArrayEnabled=YSTRUE;
		}
		if(0<=renderer->attribNormalPos)
		{
			glEnableVertexAttribArray(renderer->attribNormalPos);
		}
		if(0<=renderer->attribTexCoordPos)
		{
			glEnableVertexAttribArray(renderer->attribTexCoordPos);
			renderer->texCoordArrayEnabled=YSTRUE;
		}
		if(0<=renderer->attribPixelOffsetPos)
		{
			glEnableVertexAttribArray(renderer->attribPixelOffsetPos);
		}
		if(0<=renderer->attribViewOffsetPos)
		{
			glEnableVertexAttribArray(renderer->attribViewOffsetPos);
		}
		if(0<=renderer->attribPointSizePos)
		{
			glEnableVertexAttribArray(renderer->attribPointSizePos);
		}

		renderer->inUse=1;
	}

	return prevProgramId;
}

void YsGLSLEndUse3DRenderer(struct YsGLSL3DRenderer *renderer)
{
	if(NULL!=renderer)
	{
		if(0<=renderer->attribVertexPos)
		{
			glDisableVertexAttribArray(renderer->attribVertexPos);
		}
		if(0<=renderer->attribColorPos)
		{
			glDisableVertexAttribArray(renderer->attribColorPos);
		}
		renderer->colorArrayEnabled=YSFALSE;
		if(0<=renderer->attribNormalPos)
		{
			glDisableVertexAttribArray(renderer->attribNormalPos);
		}
		if(0<=renderer->attribTexCoordPos && YSTRUE==renderer->texCoordArrayEnabled)
		{
			glDisableVertexAttribArray(renderer->attribTexCoordPos);
		}
		renderer->texCoordArrayEnabled=YSFALSE;
		if(0<=renderer->attribPixelOffsetPos)
		{
			glDisableVertexAttribArray(renderer->attribPixelOffsetPos);
		}
		if(0<=renderer->attribViewOffsetPos)
		{
			glDisableVertexAttribArray(renderer->attribViewOffsetPos);
		}
		if(0<=renderer->attribPointSizePos)
		{
			glDisableVertexAttribArray(renderer->attribPointSizePos);
		}

		renderer->inUse=0;
	}
}

int YsGLSLCheck3DRendererIsInUse(struct YsGLSL3DRenderer *renderer)
{
	return renderer->inUse;
}

int YsGLSL3DRendererApplySphereMap(int w,int h,GLfloat *rgba,struct YsGLSL3DRenderer *renderer,int useSpecular)
{
	if(NULL!=renderer)
	{
		YsGLApplySphereMap(
		    w,h,rgba,
		    renderer->ambientColor,
		    YSGLSL_MAX_NUM_LIGHT,
		    renderer->lightEnabled,
		    renderer->lightPosition,
		    renderer->lightColor,
		    useSpecular,
		    renderer->specularColor,
		    renderer->specularExponent);
		return YSOK;
	}
	return YSERR;
}

static void YsGLSL3DRendererInitializeUniform(struct YsGLSL3DRenderer *renderer)
{
	int i;
	GLuint prevProgramId;
	const GLfloat color[4]={1,1,1,1};
	const GLfloat fogColor[4]=
	{
		0,0,0,0
	};
	const GLfloat specularColor[3]={1,1,1};
	const GLfloat ambientColor[3]={0,0,0};

	const double identityMatrixd[16]=
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	glGetIntegerv(GL_CURRENT_PROGRAM,(GLint *)&prevProgramId);
	glUseProgram(renderer->programId);

	for(i=0; i<16; ++i)
	{
		renderer->modelViewMatrix[i]=identityMatrixd[i];
		renderer->projectionMatrix[i]=identityMatrixd[i];
	}

	YsGLSLLoadIdentityMatrix4(renderer->uniformProjectionPos);
	YsGLSLLoadIdentityMatrix4(renderer->uniformModelViewPos);
	if(0<=renderer->uniformColorPos)
	{
		glUniform4fv(renderer->uniformColorPos,1,color);
	}


	if(0<=renderer->uniformViewportWidthPos)
	{
		glUniform1f(renderer->uniformViewportWidthPos,640);
	}
	if(0<=renderer->uniformViewportHeightPos)
	{
		glUniform1f(renderer->uniformViewportHeightPos,320);
	}
	if(0<=renderer->uniformViewportOriginPos)
	{
		glUniform1f(renderer->uniformViewportOriginPos,0);
	}

	if(0<=renderer->uniformMarkerTypePos)
	{
		glUniform1i(renderer->uniformMarkerTypePos,0);
	}

	if(0<=renderer->uniformAlphaCutOffPos)
	{
		glUniform1f(renderer->uniformAlphaCutOffPos,0.005f);
	}


	if(0<=renderer->uniformTextureTypePos)
	{
		glUniform1i(renderer->uniformTextureTypePos,YSGLSL_TEX_TYPE_NONE);
	}
	if(0<=renderer->uniformUseNegativeOneToPositiveOnTexCoordPos)
	{
		glUniform1i(renderer->uniformUseNegativeOneToPositiveOnTexCoordPos,1);
	}
	if(0<=renderer->uniformTextureTileTransformPos)
	{
		YsGLSLLoadIdentityMatrix4(renderer->uniformTextureTileTransformPos);
	}
	if(0<=renderer->uniformTexturePos)
	{
		glUniform1i(renderer->uniformTexturePos,0);
	}

	if(0<=renderer->uniformBillBoardTransformTypePos)
	{
		glUniform1i(renderer->uniformBillBoardTransformTypePos,YSGLSL_TEX_BILLBOARD_PERS);
	}
	if(0<=renderer->uniformBillBoardCenterPos)
	{
		const GLfloat zero[3]={0.0f,0.0f,0.0f};
		glUniform3fv(renderer->uniformBillBoardCenterPos,1,zero);
	}
	if(0<=renderer->uniformBillBoardDimensionPos)
	{
		const GLfloat unit[2]={1.0f,1.0f};
		glUniform2fv(renderer->uniformBillBoardDimensionPos,1,unit);
	}

	if(0<=renderer->uniformFogEnabledPos)
	{
		glUniform1i(renderer->uniformFogEnabledPos,0);
	}
	if(0<=renderer->uniformFogDensityPos)
	{
		glUniform1f(renderer->uniformFogDensityPos,1.0f);
	}
	if(0<=renderer->uniformFogColorPos)
	{
		glUniform4fv(renderer->uniformFogColorPos,1,fogColor);
	}


	if(0<=renderer->uniformDimensionPos)
	{
		glUniform1f(renderer->uniformDimensionPos,0.5);
	}
	if(0<=renderer->uniformRadius1Pos)
	{
		glUniform1f(renderer->uniformRadius1Pos,0.2f);
	}
	if(0<=renderer->uniformRadius2Pos)
	{
		glUniform1f(renderer->uniformRadius2Pos,1.0f);
	}
	if(0<=renderer->uniformMaxIntensityPos)
	{
		glUniform1f(renderer->uniformMaxIntensityPos,1.1f);
	}
	if(0<=renderer->uniformAngleIntervalPos)
	{
		glUniform1f(renderer->uniformAngleIntervalPos,(GLfloat)YSGLPI/3.0);
	}
	if(0<=renderer->uniformAngleOffsetPos)
	{
		glUniform1f(renderer->uniformAngleOffsetPos,(GLfloat)YSGLPI/6.0);
	}
	if(0<=renderer->uniformExponentPos)
	{
		glUniform1f(renderer->uniformExponentPos,4);
	}


	for(i=0; i<YSGLSL_MAX_NUM_LIGHT; i++)
	{
		renderer->lightEnabled[i]=(i==0 ? 1 : 0);
		renderer->lightPosition[i*4  ]=0;
		renderer->lightPosition[i*4+1]=0;
		renderer->lightPosition[i*4+2]=1;
		renderer->lightPosition[i*4+3]=0;
		renderer->lightColor[i*3  ]=1;
		renderer->lightColor[i*3+1]=1;
		renderer->lightColor[i*3+2]=1;
	}
	if(0<=renderer->uniformLightEnabledPos)
	{
		glUniform1iv(renderer->uniformLightEnabledPos,YSGLSL_MAX_NUM_LIGHT,renderer->lightEnabled);
	}
	if(0<=renderer->uniformLightPositionPos)
	{
		glUniform4fv(renderer->uniformLightPositionPos,YSGLSL_MAX_NUM_LIGHT,renderer->lightPosition);
	}
	if(0<=renderer->uniformLightColorPos)
	{
		glUniform3fv(renderer->uniformLightColorPos,YSGLSL_MAX_NUM_LIGHT,renderer->lightColor);
	}
	if(0<=renderer->uniformColorPos)
	{
		glUniform4fv(renderer->uniformColorPos,1,color);
	}

	renderer->ambientColor[0]=ambientColor[0];
	renderer->ambientColor[1]=ambientColor[1];
	renderer->ambientColor[2]=ambientColor[2];
	if(0<=renderer->uniformAmbientColorPos)
	{
		glUniform3fv(renderer->uniformAmbientColorPos,1,ambientColor);
	}

	renderer->specularExponent=80.0f;
	if(0<=renderer->uniformSpecularExponentPos)
	{
		glUniform1f(renderer->uniformSpecularExponentPos,80.0f);
	}
	renderer->specularColor[0]=specularColor[0];
	renderer->specularColor[1]=specularColor[1];
	renderer->specularColor[2]=specularColor[2];
	if(0<=renderer->uniformSpecularColorPos)
	{
		glUniform3fv(renderer->uniformSpecularColorPos,1,specularColor);
	}


	if(0<=renderer->uniformUseZOffsetPos)
	{
		glUniform1i(renderer->uniformUseZOffsetPos,0);
	}
	if(0<=renderer->uniformZOffsetPos)
	{
		glUniform1f(renderer->uniformZOffsetPos,0.0f);
	}


	if(0<=renderer->uniformPointSizePos)
	{
		glUniform1f(renderer->uniformPointSizePos,1.0f);
	}


	if(0<=renderer->uniformTexCoordRangePos)
	{
		glUniform1f(renderer->uniformTexCoordRangePos,1.0f);
	}
	if(0<=renderer->uniformPointSizeModePos)
	{
		glUniform1i(renderer->uniformPointSizeModePos,YSGLSL_POINTSPRITE_SIZE_IN_PIXEL);
	}
	if(0<=renderer->uniformPointClippingTypePos)
	{
		glUniform1i(renderer->uniformPointClippingTypePos,YSGLSL_POINTSPRITE_CLIPPING_NONE);
	}


	if(0<=renderer->uniformShadowMapModePos)
	{
		glUniform1i(renderer->uniformShadowMapModePos,YSGLSL_SHADOWMAP_NONE);
	}
	if(0<=renderer->uniformShadowMapTransformPos)
	{
		YsGLSLLoadIdentityMatrix4(renderer->uniformShadowMapTransformPos);
	}
	if(0<=renderer->uniformShadowMapTexturePos)
	{
		glUniform1i(renderer->uniformShadowMapTexturePos,0);
	}


	glUseProgram(prevProgramId);
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateFlat3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor renderer no longer needed.
	return YsGLSLCreateVariColor3DRenderer();
}

YSRESULT YsGLSLDrawPrimitiveVtxfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an array.
	YsGLSLColorIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When color is not given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0>renderer->attribNormalPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}


////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorBillBoard3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor renderer no longer needed.
	return YsGLSLCreateVariColorBillBoard3DRenderer();
}


////////////////////////////////////////////////////////////
static void YsGLSLVariColorBillBoard3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColorBillBoard3DDrawingVertexShader_nLine,
	    YSGLSL_variColorBillBoard3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColorBillBoard3DDrawingFragmentShader_nLine,
	    YSGLSL_variColorBillBoard3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");
	renderer->uniformPointClippingTypePos=glGetUniformLocation(renderer->programId,"billBoardClippingType");
	renderer->uniformBillBoardTransformTypePos=glGetUniformLocation(renderer->programId,"billBoardTransformType");
	renderer->uniformTexturePos=glGetUniformLocation(renderer->programId,"textureIdent");
	renderer->uniformUseNegativeOneToPositiveOnTexCoordPos=glGetUniformLocation(renderer->programId,"useNegativeOneToPositiveOneTexCoord");

	renderer->uniformLightEnabledPos=glGetUniformLocation(renderer->programId,"lightEnabled");
	renderer->uniformLightPositionPos=glGetUniformLocation(renderer->programId,"lightPos");
	renderer->uniformLightColorPos=glGetUniformLocation(renderer->programId,"lightColor");
	renderer->uniformSpecularColorPos=glGetUniformLocation(renderer->programId,"specularColor");
	renderer->uniformSpecularExponentPos=glGetUniformLocation(renderer->programId,"specularExponent");
	renderer->uniformAmbientColorPos=glGetUniformLocation(renderer->programId,"ambientColor");


	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");

	renderer->uniformAlphaCutOffPos=glGetUniformLocation(renderer->programId,"alphaCutOff");


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	renderer->attribTexCoordPos=glGetAttribLocation(renderer->programId,"texCoord");
	renderer->attribViewOffsetPos=glGetAttribLocation(renderer->programId,"viewOffset");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColorBillBoard3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColorBillBoard3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

YSRESULT YsGLSLDrawPrimitiveVtxColVoffsetTexcoordfv(
    const struct YsGLSL3DRenderer *renderer,GLenum mode,
    GLsizei nVtx,
    const GLfloat vtx[],     // 3*nVtx
    const GLfloat col[],     // 4*nVtx
    const GLfloat vOffset[], // 3*nVtx   Let it modify Z as well
    const GLfloat texCoord[] // 2*nVtx
    )
{
	YsGLSLTexCoordIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever texCoord is given as an array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribViewOffsetPos &&
	   0<=renderer->attribTexCoordPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glVertexAttribPointer(renderer->attribViewOffsetPos,3,GL_FLOAT,GL_FALSE,0,vOffset);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}


YSRESULT YsGLSLDrawPrimitiveVtxVoffsetTexcoordfv(
    const struct YsGLSL3DRenderer *renderer,GLenum mode,
    GLsizei nVtx,
    const GLfloat vtx[],     // 3*nVtx
    const GLfloat vOffset[], // 3*nVtx   Let it modify Z as well
    const GLfloat texCoord[] // 2*nVtx
    )
{
	YsGLSLTexCoordIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever texCoord is given as an array.
	YsGLSLColorIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When color is not given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribViewOffsetPos &&
	   0<=renderer->attribTexCoordPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribViewOffsetPos,3,GL_FLOAT,GL_FALSE,0,vOffset);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}


////////////////////////////////////////////////////////////
static void YsGLSLFlash3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_flash3DDrawingVertexShader_nLine,
	    YSGLSL_flash3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_flash3DDrawingFragmentShader_nLine,
	    YSGLSL_flash3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformDimensionPos=glGetUniformLocation(renderer->programId,"dimension");
	renderer->uniformRadius1Pos=glGetUniformLocation(renderer->programId,"radius1");
	renderer->uniformRadius2Pos=glGetUniformLocation(renderer->programId,"radius2");
	renderer->uniformMaxIntensityPos=glGetUniformLocation(renderer->programId,"maxIntensity");
	renderer->uniformAngleIntervalPos=glGetUniformLocation(renderer->programId,"angleInterval");
	renderer->uniformAngleOffsetPos=glGetUniformLocation(renderer->programId,"angleOffset");
	renderer->uniformExponentPos=glGetUniformLocation(renderer->programId,"exponent");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribViewOffsetPos=glGetAttribLocation(renderer->programId,"viewOffset");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateFlash3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLFlash3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

////////////////////////////////////////////////////////////

static void YsGLSLFlashByPointSprite3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_flashByPointSprite3DDrawingVertexShader_nLine,
	    YSGLSL_flashByPointSprite3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_flashByPointSprite3DDrawingFragmentShader_nLine,
	    YSGLSL_flashByPointSprite3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformDimensionPos=glGetUniformLocation(renderer->programId,"dimension");
	renderer->uniformRadius1Pos=glGetUniformLocation(renderer->programId,"radius1");
	renderer->uniformRadius2Pos=glGetUniformLocation(renderer->programId,"radius2");
	renderer->uniformMaxIntensityPos=glGetUniformLocation(renderer->programId,"maxIntensity");
	renderer->uniformAngleIntervalPos=glGetUniformLocation(renderer->programId,"angleInterval");
	renderer->uniformAngleOffsetPos=glGetUniformLocation(renderer->programId,"angleOffset");
	renderer->uniformExponentPos=glGetUniformLocation(renderer->programId,"exponent");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");

	renderer->uniformViewportWidthPos=glGetUniformLocation(renderer->programId,"viewportWid");
	renderer->uniformViewportHeightPos=glGetUniformLocation(renderer->programId,"viewportHei");

	renderer->uniformPointSizeModePos=glGetUniformLocation(renderer->programId,"pointSizeMode");


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribViewOffsetPos=glGetAttribLocation(renderer->programId,"viewOffset");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	renderer->attribPointSizePos=glGetAttribLocation(renderer->programId,"pointSizeIn");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateFlashByPointSprite3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLFlashByPointSprite3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorMarker3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor renderer no longer needed.
	return YsGLSLCreateVariColorMarker3DRenderer();
}

YSRESULT YsGLSLDrawPrimitiveVtxPixelOffsetfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat pixelOffset[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When color is not given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribPixelOffsetPos &&
	   0<=renderer->attribVertexPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribPixelOffsetPos,2,GL_FLOAT,GL_FALSE,0,pixelOffset);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

YSRESULT YsGLSLSet3DRendererUniformMarkerDimension(struct YsGLSL3DRenderer *renderer,GLfloat dimension)
{
	if(NULL!=renderer && 0<=renderer->uniformDimensionPos)
	{
		glUniform1f(renderer->uniformDimensionPos,dimension);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformTextureCoordRange(struct YsGLSL3DRenderer *renderer,GLfloat texCoordRange)
{
	if(NULL!=renderer && 0<=renderer->uniformTexCoordRangePos)
	{
		glUniform1f(renderer->uniformTexCoordRangePos,texCoordRange);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformMarkerType(struct YsGLSL3DRenderer *renderer,int markerType)
{
	if(NULL!=renderer && 0<=renderer->uniformMarkerTypePos)
	{
		glUniform1i(renderer->uniformMarkerTypePos,markerType);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformShadowMapMode(const struct YsGLSL3DRenderer *renderer,int shadowMapMode)
{
	if(NULL!=renderer && 0<=renderer->uniformShadowMapModePos)
	{
		glUniform1i(renderer->uniformShadowMapModePos,shadowMapMode);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformShadowMapTexture(const struct YsGLSL3DRenderer *renderer,int textureIdent)
{
	if(NULL!=renderer && 0<=renderer->uniformShadowMapTexturePos)
	{
		glUniform1i(renderer->uniformShadowMapTexturePos,textureIdent);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformShadowMapTransformation(const struct YsGLSL3DRenderer *renderer,const GLfloat tfm[16])
{
	if(NULL!=renderer && 0<=renderer->uniformShadowMapTransformPos)
	{
		glUniformMatrix4fv(renderer->uniformShadowMapTransformPos,1,GL_FALSE,tfm);
		return YSOK;
	}
	return YSERR;
}

////////////////////////////////////////////////////////////

static void YsGLSLVariColorMarker3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColorMarker3DDrawingVertexShader_nLine,
	    YSGLSL_variColorMarker3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColorMarker3DDrawingFragmentShader_nLine,
	    YSGLSL_variColorMarker3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformViewportWidthPos=glGetUniformLocation(renderer->programId,"viewportWid");
	renderer->uniformViewportHeightPos=glGetUniformLocation(renderer->programId,"viewportHei");
	renderer->uniformViewportOriginPos=glGetUniformLocation(renderer->programId,"viewportOrigin");

	renderer->uniformMarkerTypePos=glGetUniformLocation(renderer->programId,"markerType");
	renderer->uniformDimensionPos=glGetUniformLocation(renderer->programId,"dimension");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribPixelOffsetPos=glGetAttribLocation(renderer->programId,"pixelOffset");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColorMarker3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColorMarker3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

YSRESULT YsGLSLDrawPrimitiveVtxColPixelOffsetfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat col[],const GLfloat pixelOffset[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribPixelOffsetPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribVertexPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glVertexAttribPointer(renderer->attribPixelOffsetPos,2,GL_FLOAT,GL_FALSE,0,pixelOffset);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

////////////////////////////////////////////////////////////

static void YsGLSLVariColorMarkerByPointSprite3DRenderer_CreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColorMarkerByPointSprite3DDrawingVertexShader_nLine,
	    YSGLSL_variColorMarkerByPointSprite3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColorMarkerByPointSprite3DDrawingFragmentShader_nLine,
	    YSGLSL_variColorMarkerByPointSprite3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformMarkerTypePos=glGetUniformLocation(renderer->programId,"markerType");
	renderer->uniformDimensionPos=glGetUniformLocation(renderer->programId,"dimension");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColorMarkerByPointSprite3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColorMarkerByPointSprite3DRenderer_CreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorMarkerByPointSprite3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor renderer no longer needed.
	return YsGLSLCreateVariColorMarkerByPointSprite3DRenderer();
}

////////////////////////////////////////////////////////////

YSRESULT YsGLSLDrawPrimitiveVtxColVoffsetfv(const struct YsGLSL3DRenderer *renderer,GLsizei nVtx,const GLfloat vtx[],const GLfloat col[],const GLfloat viewOffset[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribViewOffsetPos &&
	   0<=renderer->attribVertexPos && 
	   0<=renderer->attribColorPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribViewOffsetPos,3,GL_FLOAT,GL_FALSE,0,viewOffset);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glDrawArrays(GL_TRIANGLES,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}


////////////////////////////////////////////////////////////

static void YsGLSLVariColor3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColor3DDrawingVertexShader_nLine,
	    YSGLSL_variColor3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColor3DDrawingFragmentShader_nLine,
	    YSGLSL_variColor3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");

	renderer->uniformAlphaCutOffPos=glGetUniformLocation(renderer->programId,"alphaCutOff");

	renderer->uniformTexturePos=glGetUniformLocation(renderer->programId,"textureIdent");
	renderer->uniformTextureTypePos=glGetUniformLocation(renderer->programId,"textureType");

	renderer->uniformPointSizePos=glGetUniformLocation(renderer->programId,"pointSizeIn");

	YsGLSLGetTextureUniformPos(renderer);


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	renderer->attribTexCoordPos=glGetAttribLocation(renderer->programId,"texCoord");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColor3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColor3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

YSRESULT YsGLSLDrawPrimitiveVtxColfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat col[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribColorPos &&
	   0>renderer->attribNormalPos &&
	   0>renderer->attribPixelOffsetPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

YSRESULT YsGLSLDrawPrimitiveVtxTexCoordColfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat texCoord[],const GLfloat col[])
{
	YsGLSLTexCoordIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribTexCoordPos &&
	   0>renderer->attribNormalPos &&
	   0>renderer->attribPixelOffsetPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorPerVtxShading3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor renderer no longer needed.
	return YsGLSLCreateVariColorPerVtxShading3DRenderer();
}

YSRESULT YsGLSLDrawPrimitiveVtxNomfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat nom[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When color is not given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribNormalPos &&
	   0>renderer->attribPixelOffsetPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribNormalPos,3,GL_FLOAT,GL_FALSE,0,nom);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

YSRESULT YsGLSLDrawPrimitiveVtxTexCoordNomfv(const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat texCoord[],const GLfloat nom[])
{
	YsGLSLTexCoordIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever texCoord is given as an array.
	YsGLSLColorIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When color is not given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribNormalPos &&
	   0<=renderer->attribTexCoordPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribNormalPos,3,GL_FLOAT,GL_FALSE,0,nom);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorPerPixShading3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor renderer no longer needed.
	return YsGLSLCreateVariColorPerPixShading3DRenderer();
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorPerPixShadingWithTexCoord3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor and WithTexCoord renderer no longer needed.
	return YsGLSLCreateVariColorPerPixShading3DRenderer();
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateMonoColorPerVtxShadingWithTexCoord3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  MonoColor and WithTexCoord renderer no longer needed.
	return YsGLSLCreateVariColorPerVtxShading3DRenderer();
}

////////////////////////////////////////////////////////////

static void YsGLSLVariColorPerVtxShading3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColorPerVtxShading3DDrawingVertexShader_nLine,
	    YSGLSL_variColorPerVtxShading3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColorPerVtxShading3DDrawingFragmentShader_nLine,
	    YSGLSL_variColorPerVtxShading3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformLightEnabledPos=glGetUniformLocation(renderer->programId,"lightEnabled");
	renderer->uniformLightPositionPos=glGetUniformLocation(renderer->programId,"lightPos");
	renderer->uniformLightColorPos=glGetUniformLocation(renderer->programId,"lightColor");
	renderer->uniformSpecularColorPos=glGetUniformLocation(renderer->programId,"specularColor");
	renderer->uniformSpecularExponentPos=glGetUniformLocation(renderer->programId,"specularExponent");
	renderer->uniformAmbientColorPos=glGetUniformLocation(renderer->programId,"ambientColor");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");

	renderer->uniformAlphaCutOffPos=glGetUniformLocation(renderer->programId,"alphaCutOff");

	YsGLSLGetTextureUniformPos(renderer);
	YsGLSLGetShadowMapUniformPos(renderer);

	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribNormalPos=glGetAttribLocation(renderer->programId,"normal");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"color");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColorPerVtxShading3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColorPerVtxShading3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

YSRESULT YsGLSLDrawPrimitiveVtxNomColfv(const struct YsGLSL3DRenderer *renderer, GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat nom[],const GLfloat col[])
{
	if(0>renderer->attribColorPos)
	{
		YsGLSLSet3DRendererUniformColorfv((struct YsGLSL3DRenderer *)renderer,col);
	}

	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribNormalPos &&
	   0>renderer->attribPixelOffsetPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribNormalPos,3,GL_FLOAT,GL_FALSE,0,nom);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else if(0<=renderer->attribVertexPos &&
	        0>renderer->attribColorPos &&
	        0<=renderer->attribNormalPos &&
	        0>renderer->attribTexCoordPos &&
	        0>renderer->attribPixelOffsetPos &&
	        0<=renderer->uniformColorPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribNormalPos,3,GL_FLOAT,GL_FALSE,0,nom);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

YSRESULT YsGLSLDrawPrimitiveVtxTexCoordNomColfv(const struct YsGLSL3DRenderer *renderer, GLenum mode,GLsizei nVtx,const GLfloat vtx[],const GLfloat texCoord[],const GLfloat nom[],const GLfloat col[])
{
	if(0>renderer->attribColorPos)
	{
		YsGLSLSet3DRendererUniformColorfv((struct YsGLSL3DRenderer *)renderer,col);
	}

	YsGLSLTexCoordIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // When texCoord is not given as an input array.
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer); // Whenever color is given as an array.

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribTexCoordPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribNormalPos &&
	   0>renderer->attribPixelOffsetPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glVertexAttribPointer(renderer->attribNormalPos,3,GL_FLOAT,GL_FALSE,0,nom);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,col);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else if(0<=renderer->attribVertexPos &&
	        0<=renderer->attribTexCoordPos &&
	        0>renderer->attribColorPos &&
	        0<=renderer->attribNormalPos &&
	        0>renderer->attribTexCoordPos &&
	        0>renderer->attribPixelOffsetPos &&
	        0<=renderer->uniformColorPos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glVertexAttribPointer(renderer->attribNormalPos,3,GL_FLOAT,GL_FALSE,0,nom);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

////////////////////////////////////////////////////////////

static void YsGLSLVariColorPerPixShading3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColorPerPixShading3DDrawingVertexShader_nLine,
	    YSGLSL_variColorPerPixShading3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColorPerPixShading3DDrawingFragmentShader_nLine,
	    YSGLSL_variColorPerPixShading3DDrawingFragmentShader);

	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");

	renderer->uniformLightEnabledPos=glGetUniformLocation(renderer->programId,"lightEnabled");
	renderer->uniformLightPositionPos=glGetUniformLocation(renderer->programId,"lightPos");
	renderer->uniformLightColorPos=glGetUniformLocation(renderer->programId,"lightColor");
	renderer->uniformSpecularColorPos=glGetUniformLocation(renderer->programId,"specularColor");
	renderer->uniformSpecularExponentPos=glGetUniformLocation(renderer->programId,"specularExponent");
	renderer->uniformAmbientColorPos=glGetUniformLocation(renderer->programId,"ambientColor");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");

	renderer->uniformAlphaCutOffPos=glGetUniformLocation(renderer->programId,"alphaCutOff");

	YsGLSLGetTextureUniformPos(renderer);
	YsGLSLGetShadowMapUniformPos(renderer);


	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribNormalPos=glGetAttribLocation(renderer->programId,"normal");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	renderer->attribTexCoordPos=glGetAttribLocation(renderer->programId,"texCoord");
	printf("Verify Vertex Attrib Location (Must be zero)=%d\n",renderer->attribVertexPos);
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColorPerPixShading3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColorPerPixShading3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateVariColorPerPixShadingWithTexCoord3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  WithTexCoord renderer no longer needed.
	return YsGLSLCreateVariColorPerPixShading3DRenderer();
}

////////////////////////////////////////////////////////////

struct YsGLSL3DRenderer *YsGLSLCreateVariColorPerVtxShadingWithTexCoord3DRenderer(void)
{
	// 2015/12/09 Finally figured how to use generic attribute.  WithTexCoord renderer no longer needed.
	return YsGLSLCreateVariColorPerVtxShading3DRenderer();
}

////////////////////////////////////////////////////////////

static void YsGLSLVariColorPointSprite3DRendererCreateProgram(struct YsGLSL3DRenderer *renderer)
{
	renderer->vertexShaderId=glCreateShader(GL_VERTEX_SHADER);
	renderer->fragmentShaderId=glCreateShader(GL_FRAGMENT_SHADER);
	renderer->programId=glCreateProgram();

	YsGLSLCompileAndLinkVertexAndFragmentShader(
	    renderer->programId,
	    renderer->vertexShaderId,
	    YSGLSL_variColorPointSprite3DDrawingVertexShader_nLine,
	    YSGLSL_variColorPointSprite3DDrawingVertexShader,
	    renderer->fragmentShaderId,
	    YSGLSL_variColorPointSprite3DDrawingFragmentShader_nLine,
	    YSGLSL_variColorPointSprite3DDrawingFragmentShader);


	// Uniforms
	renderer->uniformProjectionPos=glGetUniformLocation(renderer->programId,"projection");
	renderer->uniformModelViewPos=glGetUniformLocation(renderer->programId,"modelView");
	renderer->uniformTexturePos=glGetUniformLocation(renderer->programId,"textureIdent");

	renderer->uniformFogEnabledPos=glGetUniformLocation(renderer->programId,"fogEnabled");
	renderer->uniformFogDensityPos=glGetUniformLocation(renderer->programId,"fogDensity");
	renderer->uniformFogColorPos=glGetUniformLocation(renderer->programId,"fogColor");

	renderer->uniformUseZOffsetPos=glGetUniformLocation(renderer->programId,"useZOffset");
	renderer->uniformZOffsetPos=glGetUniformLocation(renderer->programId,"zOffset");

	renderer->uniformViewportWidthPos=glGetUniformLocation(renderer->programId,"viewportWid");
	renderer->uniformViewportHeightPos=glGetUniformLocation(renderer->programId,"viewportHei");

	renderer->uniformPointSizeModePos=glGetUniformLocation(renderer->programId,"pointSizeMode");
	renderer->uniformPointClippingTypePos=glGetUniformLocation(renderer->programId,"pointClippingType");
	renderer->uniformTextureTypePos=glGetUniformLocation(renderer->programId,"textureType");
	renderer->uniformTexCoordRangePos=glGetUniformLocation(renderer->programId,"texCoordRange");

	renderer->uniformAlphaCutOffPos=glGetUniformLocation(renderer->programId,"alphaCutOff");

	// Attributes
	renderer->attribVertexPos=glGetAttribLocation(renderer->programId,"vertex");
	renderer->attribColorPos=glGetAttribLocation(renderer->programId,"colorIn");
	renderer->attribTexCoordPos=glGetAttribLocation(renderer->programId,"texCoordIn");
	renderer->attribPointSizePos=glGetAttribLocation(renderer->programId,"pointSizeIn");
}

struct YsGLSL3DRenderer *YsGLSLCreateVariColorPointSprite3DRenderer(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLCreateAndInitialize3DRenderer();
	if(NULL!=renderer)
	{
		YsGLSLVariColorPointSprite3DRendererCreateProgram(renderer);
		YsGLSL3DRendererInitializeUniform(renderer);
	}
	return renderer;
}

YSRESULT YsGLSLSet3DRendererPointSpriteClippingType(const struct YsGLSL3DRenderer *renderer,int pointSpriteClippingType)
{
	if(NULL!=renderer && 0<=renderer->uniformPointClippingTypePos)
	{
		glUniform1i(renderer->uniformPointClippingTypePos,pointSpriteClippingType);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererPointSizeMode(const struct YsGLSL3DRenderer *renderer,int pointSizeMode)
{
	if(NULL!=renderer && 0<=renderer->uniformPointSizeModePos)
	{
		glUniform1i(renderer->uniformPointSizeModePos,pointSizeMode);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLSet3DRendererUniformPointSize(const struct YsGLSL3DRenderer *renderer,float pointSize)
{
	if(NULL!=renderer && 0<=renderer->uniformPointSizePos)
	{
		glUniform1f(renderer->uniformPointSizePos,pointSize);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLDrawPrimitiveVtxTexCoordColPointSizefv(
    const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,
    const GLfloat vtx[],const GLfloat texCoord[],const GLfloat color[],const GLfloat pointSize[])
{
	YsGLSLTexCoordIsGivenAsArray((struct YsGLSL3DRenderer *)renderer);
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer);

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribTexCoordPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribPointSizePos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribTexCoordPos,2,GL_FLOAT,GL_FALSE,0,texCoord);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,color);
		glVertexAttribPointer(renderer->attribPointSizePos,1,GL_FLOAT,GL_FALSE,0,pointSize);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	return YSERR;
}

YSRESULT YsGLSLDrawPrimitiveVtxColPointSizefv(
    const struct YsGLSL3DRenderer *renderer,GLenum mode,GLsizei nVtx,
    const GLfloat vtx[],const GLfloat color[],const GLfloat pointSize[])
{
	YsGLSLTexCoordIsNotGivenAsArray((struct YsGLSL3DRenderer *)renderer);
	YsGLSLColorIsGivenAsArray((struct YsGLSL3DRenderer *)renderer);

	if(NULL!=renderer &&
	   0<=renderer->attribVertexPos &&
	   0<=renderer->attribColorPos &&
	   0<=renderer->attribPointSizePos)
	{
		glVertexAttribPointer(renderer->attribVertexPos,3,GL_FLOAT,GL_FALSE,0,vtx);
		glVertexAttribPointer(renderer->attribColorPos,4,GL_FLOAT,GL_FALSE,0,color);
		glVertexAttribPointer(renderer->attribPointSizePos,1,GL_FLOAT,GL_FALSE,0,pointSize);
		glDrawArrays(mode,0,nVtx);
		return YSOK;
	}
	return YSERR;
}


