#ifndef YSGLBUFFERMANAGER_GL2_IS_INCLUDED
#define YSGLBUFFERMANAGER_GL2_IS_INCLUDED
/* { */

#include <ysgl.h>
#include <ysclass.h>
#include "ysglbuffermanager.h"

class YsGLBufferManager::ActualBuffer
{
public:
	enum
	{
		MAX_NUM_VERTEX_PER_VBO=99999
	};

	class BufferSet
	{
	public:
		YSSIZE_T nVtx;
		GLuint vboIdent;
		GLuint vtxPtr,nomPtr,colPtr,pointSizePtr;

		BufferSet()
		{
			vboIdent=0;
			nVtx=0;
			vtxPtr=0;
			nomPtr=0;
			colPtr=0;
			pointSizePtr=0;
		}
	};
	YsArray <BufferSet> bufSet;

	void PrepareVbo(YSSIZE_T nVbo);
	void DrawPrimitiveVtx(YsGLSL3DRenderer *renderer,GLenum primType) const;
	void DrawPrimitiveVtxNom(YsGLSL3DRenderer *renderer,GLenum primType) const;
	void DrawPrimitiveVtxCol(YsGLSL3DRenderer *renderer,GLenum primType) const;
	void DrawPrimitiveVtxNomCol(YsGLSL3DRenderer *renderer,GLenum primType) const;
	void DrawPrimitiveVtxColPointSize(YsGLSL3DRenderer *renderer,GLenum primType) const;
};

/* } */
#endif
