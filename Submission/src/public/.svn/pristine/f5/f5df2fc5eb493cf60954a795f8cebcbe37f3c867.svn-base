#include "ysglbuffermanager_gl1.h"

void YsGLBufferManager::ActualBuffer::PrepareDisplayList(YSSIZE_T nList)
{
	auto nCurrent=bufSet.GetN();

	for(auto i=nList; i<nCurrent; ++i)
	{
		if(GL_TRUE==glIsList(bufSet[i].listIdent))
		{
			glDeleteLists(bufSet[i].listIdent,1);
		}
	}

	bufSet.Resize(nList);
	for(auto i=nCurrent; i<nList; ++i)
	{
		bufSet[i].listIdent=glGenLists(1);
	}
}

////////////////////////////////////////////////////////////

/* static */ YsGLBufferManager::ActualBuffer *YsGLBufferManager::Unit::Create(void)
{
	return new YsGLBufferManager::ActualBuffer;
}
/* static */ void YsGLBufferManager::Unit::Delete(YsGLBufferManager::ActualBuffer *ptr)
{
	for(auto &buf : ptr->bufSet)
	{
		if(GL_TRUE==glIsList(buf.listIdent))
		{
			glDeleteLists(buf.listIdent,1);
		}
	}
	delete ptr;
}

YsGLBufferManager::Unit::STATE YsGLBufferManager::Unit::GetState(void) const
{
	if(nullptr==bufferPtr)
	{
		return EMPTY;
	}
	return POPULATED;
}

static GLenum ConvertPrimType(YsGL::PRIMITIVE_TYPE primType)
{
	switch(primType)
	{
	default:
		break;
	case YsGL::POINTS:
		return GL_POINTS;
	case YsGL::LINES:
		return GL_LINES;
	case YsGL::LINE_STRIP:
		return GL_LINE_STRIP;
	case YsGL::LINE_LOOP:
		return GL_LINE_LOOP;
	case YsGL::TRIANGLES:
		return GL_TRIANGLES;
	case YsGL::TRIANGLE_STRIP:
		return GL_TRIANGLE_STRIP;
	case YsGL::TRIANGLE_FAN:
		return GL_TRIANGLE_FAN;
	};
	return GL_POINTS;
}

void YsGLBufferManager::Unit::MakeVtx(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareDisplayList(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		glNewList(buf.listIdent,GL_COMPILE);
		glBegin(ConvertPrimType(primType));
		for(YSSIZE_T i=0; i<nVtx; ++i)
		{
			auto vtIdx=vtxTop+i;
			glVertex3fv(vtxBuf.GetElemPointer(vtIdx));
		}
		glEnd();
		glEndList();
	}
}
void YsGLBufferManager::Unit::MakeVtxCol(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareDisplayList(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		glNewList(buf.listIdent,GL_COMPILE);
		glBegin(ConvertPrimType(primType));
		for(YSSIZE_T i=0; i<nVtx; ++i)
		{
			auto vtIdx=vtxTop+i;
			glColor4fv(colBuf.GetElemPointer(vtIdx));
			glVertex3fv(vtxBuf.GetElemPointer(vtIdx));
		}
		glEnd();
		glEndList();
	}
}
void YsGLBufferManager::Unit::MakeVtxNomCol(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLNormalBuffer &nomBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareDisplayList(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		glNewList(buf.listIdent,GL_COMPILE);
		glBegin(ConvertPrimType(primType));
		for(YSSIZE_T i=0; i<nVtx; ++i)
		{
			auto vtIdx=vtxTop+i;
			glColor4fv(colBuf.GetElemPointer(vtIdx));
			glNormal3fv(nomBuf.GetElemPointer(vtIdx));
			glVertex3fv(vtxBuf.GetElemPointer(vtIdx));
		}
		glEnd();
		glEndList();
	}
}
void YsGLBufferManager::Unit::MakeVtxColPointSize(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,const YsGLPointSizeBuffer &psBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareDisplayList(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		glNewList(buf.listIdent,GL_COMPILE);
		glBegin(ConvertPrimType(primType));
		for(YSSIZE_T i=0; i<nVtx; ++i)
		{
			auto vtIdx=vtxTop+i;
			glColor4fv(colBuf.GetElemPointer(vtIdx));
			glVertex3fv(vtxBuf.GetElemPointer(vtIdx));
		}
		glEnd();
		glEndList();
	}
}

void YsGLBufferManager::ActualBuffer::DrawPrimitive(void) const
{
	for(auto &buf : bufSet)
	{
		glCallList(buf.listIdent);
	}
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtx(GLenum) const
{
	DrawPrimitive();
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxNom(GLenum) const
{
	DrawPrimitive();
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxCol(GLenum primType) const
{
	DrawPrimitive();
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxNomCol(GLenum primType) const
{
	DrawPrimitive();
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxColPointSize(GLenum) const
{
	DrawPrimitive();
}
