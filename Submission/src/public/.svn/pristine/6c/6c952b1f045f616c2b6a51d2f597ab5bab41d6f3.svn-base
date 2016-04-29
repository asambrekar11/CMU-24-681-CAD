#ifndef YSGLBUFFERMANAGER_IS_INCLUDED
#define YSGLBUFFERMANAGER_IS_INCLUDED
/* { */

#include "ysglcpp.h"
#include "ysglbuffer.h"
#include <ysclass.h>

/*!
YsGLBufferManager detaches high-level vertex-buffer data structure and the graphics tookit's own vertex-buffer management structure.
The concept of the vertex-buffer is independent of OpenGL, Direct3D, or anything else.
Native handle to the vertex-buffer is necessary only when the vertex-buffer object is populated and when the primitives are drawn.
Until then, the program should be free of native handles.

The goal of this class is to encapculate native handles in the implementation library (ysglbuffermanager_gl2, ysglbuffermanager_gl1)
and reduce the toolkit dependent code as much as possible.
*/
class YsGLBufferManager
{
public:
	static YsGLBufferManager &GetSharedBufferManager(void);

public:
	friend class Unit;

	class ActualBuffer;
	class Unit
	{
	private:
		ActualBuffer *bufferPtr;
	public:
		enum STATE
		{
			EMPTY,
			POPULATED
		};

		Unit();
		~Unit();
		void CleanUp(void);
		const ActualBuffer *GetActualBuffer(void) const;
		YSSIZE_T AdjustMaxNumVtxPerVboForPrimitiveType(YsGL::PRIMITIVE_TYPE primType,YSSIZE_T nVtx);

		// Implemented in toolkit-specific library >>
		static ActualBuffer *Create(void);
		static void Delete(ActualBuffer *ptr);
		STATE GetState(void) const;
		void MakeVtx(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,YSSIZE_T maxNumVtxPerVbo);
		void MakeVtxCol(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo);
		void MakeVtxNomCol(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLNormalBuffer &nomBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo);
		void MakeVtxColPointSize(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,const YsGLPointSizeBuffer &psBuf,YSSIZE_T maxNumVtxPerVbo);
		// Implemented in toolkit-specific library <<
	};

	typedef YsEditArrayObjectHandle <Unit,8> Handle;

private:
	YsEditArray <Unit,8> bufferArray;

public:
	Handle Create(void);
	const Unit *GetBufferUnit(Handle hd) const;
	const ActualBuffer *GetActualBuffer(Handle hd) const;
	void Delete(Handle hd);
	void MakeVtx(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,YSSIZE_T maxNumVtxPerVbo=0x1fffffff);
	void MakeVtxCol(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo=0x1fffffff);
	void MakeVtxNomCol(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLNormalBuffer &nomBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo=0x1fffffff);
	void MakeVtxColPointSize(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,const YsGLPointSizeBuffer &psBuf,YSSIZE_T maxNumVtxPerVbo=0x1fffffff);
};


/* } */
#endif
