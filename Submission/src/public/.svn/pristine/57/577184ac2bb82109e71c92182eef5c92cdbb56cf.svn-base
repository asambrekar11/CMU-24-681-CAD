#include "ysglbuffermanager_gl2.h"

void YsGLBufferManager::ActualBuffer::PrepareVbo(YSSIZE_T nVbo)
{
	auto nCurrent=bufSet.GetN();

	for(auto i=nVbo; i<nCurrent; ++i)
	{
		if(GL_TRUE==glIsBuffer(bufSet[i].vboIdent))
		{
			glDeleteBuffers(1,&bufSet[i].vboIdent);
		}
	}

	bufSet.Resize(nVbo);
	for(auto i=nCurrent; i<nVbo; ++i)
	{
		GLuint newVboIdent[1];
		glGenBuffers(1,newVboIdent);
		bufSet[i].vboIdent=newVboIdent[0];
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
		if(GL_TRUE==glIsBuffer(buf.vboIdent))
		{
			glDeleteBuffers(1,&buf.vboIdent);
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

template <class T>
static int PushBufferSubData(YSSIZE_T &bufPtr,const T &buf,YSSIZE_T vtxTop,YSSIZE_T nVtx)
{
	auto currentPtr=bufPtr;
	auto bufLength=buf.GetNBytePerElem()*nVtx;

	glBufferSubData(GL_ARRAY_BUFFER,bufPtr,bufLength,buf.GetArrayPointer()+buf.GetNPerElem()*vtxTop);

	bufPtr+=bufLength;
	return (int)currentPtr;   // <- By wrongfully writing bufPtr, I spent about four hours to debug it....
}

void YsGLBufferManager::Unit::MakeVtx(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareVbo(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		buf.nVtx=nVtx;

		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YSSIZE_T totalBufSize=vtxBuf.GetNBytePerElem()*nVtx;

		glBufferData(GL_ARRAY_BUFFER,totalBufSize,nullptr,GL_STATIC_DRAW);

		YSSIZE_T bufPtr=0;
		buf.vtxPtr=PushBufferSubData(bufPtr,vtxBuf,vtxTop,nVtx);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtx(YsGLSL3DRenderer *renderer,GLenum primType) const
{
	for(auto &buf : bufSet)
	{
		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YsGLSLDrawPrimitiveVtxfv(renderer,primType,(int)buf.nVtx,(GLfloat *)buf.vtxPtr);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxNom(YsGLSL3DRenderer *renderer,GLenum primType) const
{
	for(auto &buf : bufSet)
	{
		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YsGLSLDrawPrimitiveVtxNomfv(renderer,primType,(int)buf.nVtx,(GLfloat *)buf.vtxPtr,(GLfloat *)buf.nomPtr);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxCol(YsGLSL3DRenderer *renderer,GLenum primType) const
{
	for(auto &buf : bufSet)
	{
		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YsGLSLDrawPrimitiveVtxColfv(renderer,primType,(int)buf.nVtx,(GLfloat *)buf.vtxPtr,(GLfloat *)buf.colPtr);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxNomCol(YsGLSL3DRenderer *renderer,GLenum primType) const
{
	for(auto &buf : bufSet)
	{
		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YsGLSLDrawPrimitiveVtxNomColfv(renderer,primType,(int)buf.nVtx,(GLfloat *)buf.vtxPtr,(GLfloat *)buf.nomPtr,(GLfloat *)buf.colPtr);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void YsGLBufferManager::ActualBuffer::DrawPrimitiveVtxColPointSize(YsGLSL3DRenderer *renderer,GLenum primType) const
{
	for(auto &buf : bufSet)
	{
		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YsGLSLDrawPrimitiveVtxColPointSizefv(renderer,primType,(int)buf.nVtx,(GLfloat *)buf.vtxPtr,(GLfloat *)buf.colPtr,(GLfloat *)buf.pointSizePtr);
	}
	glBindBuffer(GL_ARRAY_BUFFER,0);
}


void YsGLBufferManager::Unit::MakeVtxCol(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareVbo(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		buf.nVtx=nVtx;

		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YSSIZE_T totalBufSize=(vtxBuf.GetNBytePerElem()+colBuf.GetNBytePerElem())*nVtx;

		glBufferData(GL_ARRAY_BUFFER,totalBufSize,nullptr,GL_STATIC_DRAW);

		YSSIZE_T bufPtr=0;
		buf.vtxPtr=PushBufferSubData(bufPtr,vtxBuf,vtxTop,nVtx);
		buf.colPtr=PushBufferSubData(bufPtr,colBuf,vtxTop,nVtx);
	}

	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void YsGLBufferManager::Unit::MakeVtxNomCol(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLNormalBuffer &nomBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareVbo(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		buf.nVtx=nVtx;

		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		// Memo: Be careful about totalBufSize.  Took two hours to debug.  2015/09/19
		YSSIZE_T totalBufSize=(vtxBuf.GetNBytePerElem()+colBuf.GetNBytePerElem()+nomBuf.GetNBytePerElem())*nVtx;

		glBufferData(GL_ARRAY_BUFFER,totalBufSize,nullptr,GL_STATIC_DRAW);

		YSSIZE_T bufPtr=0;
		buf.vtxPtr=PushBufferSubData(bufPtr,vtxBuf,vtxTop,nVtx);
		buf.colPtr=PushBufferSubData(bufPtr,colBuf,vtxTop,nVtx);
		buf.nomPtr=PushBufferSubData(bufPtr,nomBuf,vtxTop,nVtx);
	}

	glBindBuffer(GL_ARRAY_BUFFER,0);
}
void YsGLBufferManager::Unit::MakeVtxColPointSize(YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,const YsGLPointSizeBuffer &psBuf,YSSIZE_T maxNumVtxPerVbo)
{
	maxNumVtxPerVbo=AdjustMaxNumVtxPerVboForPrimitiveType(primType,maxNumVtxPerVbo);
	if(nullptr==bufferPtr)
	{
		bufferPtr=YsGLBufferManager::Unit::Create();
	}
	bufferPtr->PrepareVbo(1+vtxBuf.GetN()/maxNumVtxPerVbo);
	for(YSSIZE_T vboIndex=0; vboIndex<bufferPtr->bufSet.GetN(); ++vboIndex)
	{
		auto &buf=bufferPtr->bufSet[vboIndex];
		auto vtxTop=maxNumVtxPerVbo*vboIndex;
		auto nVtx=YsSmaller(maxNumVtxPerVbo,vtxBuf.GetN()-vtxTop);

		buf.nVtx=nVtx;

		glBindBuffer(GL_ARRAY_BUFFER,buf.vboIdent);
		YSSIZE_T totalBufSize=(vtxBuf.GetNBytePerElem()+colBuf.GetNBytePerElem()+psBuf.GetNBytePerElem())*nVtx;

		glBufferData(GL_ARRAY_BUFFER,totalBufSize,nullptr,GL_STATIC_DRAW);

		YSSIZE_T bufPtr=0;
		buf.vtxPtr=PushBufferSubData(bufPtr,vtxBuf,vtxTop,nVtx);
		buf.colPtr=PushBufferSubData(bufPtr,colBuf,vtxTop,nVtx);
		buf.pointSizePtr=PushBufferSubData(bufPtr,psBuf,vtxTop,nVtx);
	}

	glBindBuffer(GL_ARRAY_BUFFER,0);
}
