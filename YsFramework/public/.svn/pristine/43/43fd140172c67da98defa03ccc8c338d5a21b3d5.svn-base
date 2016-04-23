#include "ysglbuffermanager.h"


YsGLBufferManager::Unit::Unit()
{
	bufferPtr=nullptr;
}

YsGLBufferManager::Unit::~Unit()
{
	CleanUp();
}

void YsGLBufferManager::Unit::CleanUp(void)
{
	if(nullptr!=bufferPtr)
	{
		Delete(bufferPtr);
		bufferPtr=nullptr;
	}
}
YSSIZE_T YsGLBufferManager::Unit::AdjustMaxNumVtxPerVboForPrimitiveType(YsGL::PRIMITIVE_TYPE primType,YSSIZE_T nVtx)
{
	switch(primType)
	{
	default:
		break;
	case YsGL::LINES:
		return ((nVtx+1)/2)*2;
	case YsGL::TRIANGLES:
		return ((nVtx+2)/3)*3;
	}
	return nVtx;
}
const YsGLBufferManager::ActualBuffer *YsGLBufferManager::Unit::GetActualBuffer(void) const
{
	return bufferPtr;
}
////////////////////////////////////////////////////////////


/* static */ YsGLBufferManager &YsGLBufferManager::GetSharedBufferManager(void)
{
	static YsGLBufferManager sharedManager;
	return sharedManager;
}

YsGLBufferManager::Handle YsGLBufferManager::Create(void)
{
	auto hd=bufferArray.Create();
	bufferArray[hd]->CleanUp();
	return hd;
}
const YsGLBufferManager::Unit *YsGLBufferManager::GetBufferUnit(Handle hd) const
{
	return bufferArray[hd];
}

const YsGLBufferManager::ActualBuffer *YsGLBufferManager::GetActualBuffer(Handle hd) const
{
	auto unitPtr=GetBufferUnit(hd);
	if(Unit::EMPTY!=unitPtr->GetState())
	{
		return unitPtr->GetActualBuffer();
	}
	return nullptr;
}

void YsGLBufferManager::Delete(Handle hd)
{
	if(nullptr!=hd)
	{
		bufferArray[hd]->CleanUp();
		bufferArray.Delete(hd);
	}
}
void YsGLBufferManager::MakeVtx(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,YSSIZE_T maxNumVtxPerVbo)
{
	bufferArray[bfHd]->MakeVtx(primType,vtxBuf,maxNumVtxPerVbo);
}
void YsGLBufferManager::MakeVtxCol(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo)
{
	bufferArray[bfHd]->MakeVtxCol(primType,vtxBuf,colBuf,maxNumVtxPerVbo);
}
void YsGLBufferManager::MakeVtxNomCol(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLNormalBuffer &nomBuf,const YsGLColorBuffer &colBuf,YSSIZE_T maxNumVtxPerVbo)
{
	bufferArray[bfHd]->MakeVtxNomCol(primType,vtxBuf,nomBuf,colBuf,maxNumVtxPerVbo);
}
void YsGLBufferManager::MakeVtxColPointSize(Handle bfHd,YsGL::PRIMITIVE_TYPE primType,const YsGLVertexBuffer &vtxBuf,const YsGLColorBuffer &colBuf,const YsGLPointSizeBuffer &psBuf,YSSIZE_T maxNumVtxPerVbo)
{
	bufferArray[bfHd]->MakeVtxColPointSize(primType,vtxBuf,colBuf,psBuf,maxNumVtxPerVbo);
}
