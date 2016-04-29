#include "ysglbuffermanager_nownd.h"

////////////////////////////////////////////////////////////

/* static */ YsGLBufferManager::ActualBuffer *YsGLBufferManager::Unit::Create(void)
{
	return new YsGLBufferManager::ActualBuffer;
}
/* static */ void YsGLBufferManager::Unit::Delete(YsGLBufferManager::ActualBuffer *ptr)
{
	delete ptr;
}

YsGLBufferManager::Unit::STATE YsGLBufferManager::Unit::GetState(void) const
{
	return EMPTY;
}

void YsGLBufferManager::Unit::MakeVtx(YsGL::PRIMITIVE_TYPE ,const YsGLVertexBuffer &,YSSIZE_T )
{
}
void YsGLBufferManager::Unit::MakeVtxCol(YsGL::PRIMITIVE_TYPE ,const YsGLVertexBuffer &,const YsGLColorBuffer &,YSSIZE_T )
{
}
void YsGLBufferManager::Unit::MakeVtxNomCol(YsGL::PRIMITIVE_TYPE ,const YsGLVertexBuffer &,const YsGLNormalBuffer &,const YsGLColorBuffer &,YSSIZE_T )
{
}
void YsGLBufferManager::Unit::MakeVtxColPointSize(YsGL::PRIMITIVE_TYPE ,const YsGLVertexBuffer &,const YsGLColorBuffer &,const YsGLPointSizeBuffer &,YSSIZE_T )
{
}
