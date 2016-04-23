#ifndef YSGLBUFFERMANAGER_GL1_IS_INCLUDED
#define YSGLBUFFERMANAGER_GL1_IS_INCLUDED
/* { */

#include <ysglheader.h>
#include <ysglbuffermanager.h>

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
		GLuint listIdent;

		BufferSet()
		{
			listIdent=0;
		}
	};
	YsArray <BufferSet> bufSet;

	void PrepareDisplayList(YSSIZE_T nVbo);
	void DrawPrimitive(void) const;

	// Following functions are for more common coding with OpenGL 2.x/ES2.
	// Patameters are ignored, and just DrawPrimitive is called from inside.
	void DrawPrimitiveVtx(GLenum) const;
	void DrawPrimitiveVtxNom(GLenum) const;
	void DrawPrimitiveVtxCol(GLenum) const;
	void DrawPrimitiveVtxNomCol(GLenum) const;
	void DrawPrimitiveVtxColPointSize(GLenum) const;
};

/* } */
#endif
