#include <ysglbuffermanager_gl1.h>
#include <ysshellextgl.h>



void YsHasShellExtVboSet::Render(const YsMatrix4x4 &viewModelTfm,RenderingOption opt) const
{
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glLoadIdentity();
	glScaled(1.0,1.0,-1.0);

	double mat[16];
	viewModelTfm.GetOpenGlCompatibleMatrix(mat);
	glMultMatrixd(mat);

	auto &bufManager=YsGLBufferManager::GetSharedBufferManager();
	if(YSTRUE==opt.solidPolygon)
	{
		auto unitPtr=bufManager.GetBufferUnit(vboSet.solidShadedPosNomColHd);
		if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
		{
			glEnable(GL_LIGHTING);
			unitPtr->GetActualBuffer()->DrawPrimitiveVtxNomCol(GL_TRIANGLES);
		}
		unitPtr=bufManager.GetBufferUnit(vboSet.solidUnshadedPosColHd);
		if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
		{
			glDisable(GL_LIGHTING);
			unitPtr->GetActualBuffer()->DrawPrimitiveVtxCol(GL_TRIANGLES);
		}
	}
	if(YSTRUE==opt.backFace)
	{
		auto unitPtr=bufManager.GetBufferUnit(vboSet.backFacePosColHd);
		if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
		{
			glDisable(GL_LIGHTING);
			unitPtr->GetActualBuffer()->DrawPrimitiveVtxCol(GL_TRIANGLES);
		}
	}
	if(YSTRUE==opt.transparentPolygon)
	{
		auto unitPtr=bufManager.GetBufferUnit(vboSet.trspShadedPosNomColHd);
		if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
		{
			glEnable(GL_LIGHTING);
			unitPtr->GetActualBuffer()->DrawPrimitiveVtxNomCol(GL_TRIANGLES);
		}
		unitPtr=bufManager.GetBufferUnit(vboSet.trspUnshadedPosColHd);
		if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
		{
			glDisable(GL_LIGHTING);
			unitPtr->GetActualBuffer()->DrawPrimitiveVtxNomCol(GL_TRIANGLES);
		}
	}
	glDisable(GL_CULL_FACE);

	if(YSTRUE==opt.shadow)
	{
		glDisable(GL_LIGHTING);
		glColor3ub(0,0,0);
		for(auto hd : vboSet.plainPlgPosHd)
		{
			auto unitPtr=bufManager.GetBufferUnit(hd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				unitPtr->GetActualBuffer()->DrawPrimitiveVtx(GL_TRIANGLES);
			}
		}
	}

	if(YSTRUE==opt.light)
	{
		auto unitPtr=bufManager.GetBufferUnit(vboSet.lightPosColPointSizeHd);
		if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
		{
			glPointSize(3);
			glDisable(GL_LIGHTING);
			unitPtr->GetActualBuffer()->DrawPrimitiveVtxColPointSize(GL_POINTS);
			glPointSize(1);
		}
	}

	glPopMatrix();
}
