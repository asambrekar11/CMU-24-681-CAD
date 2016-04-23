#include <ysgl.h>
#include <ysglslcpp.h>
#include <ysglbuffermanager_gl2.h>
#include "ysshellextgl.h"



void YsHasShellExtVboSet::Render(const YsMatrix4x4 &viewModelTfm,RenderingOption opt) const
{
	float mat[16],push[16];
	if(YSLEFT_ZPLUS_YPLUS==YsCoordSysModel)
	{
		YsMatrix4x4 total;
		total.Scale(1.0,1.0,-1.0);
		total*=viewModelTfm;
		total.GetOpenGlCompatibleMatrix(mat);
		glFrontFace(GL_CW);
	}
	else if(YSRIGHT_ZMINUS_YPLUS==YsCoordSysModel)
	{
		viewModelTfm.GetOpenGlCompatibleMatrix(mat);
	}

	// YsGLBufferManager::Handle solidShadedPosNomColHd;
	// YsGLBufferManager::Handle solidUnshadedPosColHd;
	// YsGLBufferManager::Handle trspShadedPosNomColHd;
	// YsGLBufferManager::Handle trspUnshadedPosColHd;
	// YsGLBufferManager::Handle lightPosColPointSizeHd;

	auto &bufManager=YsGLBufferManager::GetSharedBufferManager();
	if(YSTRUE==opt.solidPolygon)
	{
		if(NULL!=YsGLSLSharedVariColorShaded3DRenderer())
		{
			auto unitPtr=bufManager.GetBufferUnit(vboSet.solidShadedPosNomColHd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				YsGLSLRenderer renderer(YsGLSLSharedVariColorShaded3DRenderer());
				renderer.GetModelView(push);
				renderer.SetModelView(mat);
				unitPtr->GetActualBuffer()->DrawPrimitiveVtxNomCol(renderer,GL_TRIANGLES);
				renderer.SetModelView(push);
			}
		}
		if(NULL!=YsGLSLSharedVariColor3DRenderer())
		{
			auto unitPtr=bufManager.GetBufferUnit(vboSet.solidUnshadedPosColHd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				YsGLSLRenderer renderer(YsGLSLSharedVariColor3DRenderer());
				renderer.GetModelView(push);
				renderer.SetModelView(mat);
				unitPtr->GetActualBuffer()->DrawPrimitiveVtxCol(renderer,GL_TRIANGLES);
				renderer.SetModelView(push);
			}
		}
	}
	if(YSTRUE==opt.backFace)
	{
		if(NULL!=YsGLSLSharedVariColor3DRenderer())
		{
			auto unitPtr=bufManager.GetBufferUnit(vboSet.backFacePosColHd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				YsGLSLRenderer renderer(YsGLSLSharedVariColor3DRenderer());
				renderer.GetModelView(push);
				renderer.SetModelView(mat);
				unitPtr->GetActualBuffer()->DrawPrimitiveVtxCol(renderer,GL_TRIANGLES);
				renderer.SetModelView(push);
			}
		}
	}
	if(YSTRUE==opt.transparentPolygon)
	{
		if(NULL!=YsGLSLSharedVariColorShaded3DRenderer())
		{
			auto unitPtr=bufManager.GetBufferUnit(vboSet.trspShadedPosNomColHd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				YsGLSLRenderer renderer(YsGLSLSharedVariColorShaded3DRenderer());
				renderer.GetModelView(push);
				renderer.SetModelView(mat);
				unitPtr->GetActualBuffer()->DrawPrimitiveVtxNomCol(renderer,GL_TRIANGLES);
				renderer.SetModelView(push);
			}
		}
		if(NULL!=YsGLSLSharedVariColor3DRenderer())
		{
			auto unitPtr=bufManager.GetBufferUnit(vboSet.trspUnshadedPosColHd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				YsGLSLRenderer renderer(YsGLSLSharedVariColor3DRenderer());
				renderer.GetModelView(push);
				renderer.SetModelView(mat);
				unitPtr->GetActualBuffer()->DrawPrimitiveVtxCol(renderer,GL_TRIANGLES);
				renderer.SetModelView(push);
			}
		}
	}
	if(YSTRUE==opt.light)
	{
		if(NULL!=YsGLSLSharedFlashByPointSprite3DRenderer())
		{
			auto unitPtr=bufManager.GetBufferUnit(vboSet.lightPosColPointSizeHd);
			if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
			{
				int viewport[4];
				glGetIntegerv(GL_VIEWPORT,viewport);
#ifdef GL_PROGRAM_POINT_SIZE
				glEnable(GL_PROGRAM_POINT_SIZE);  // Needed for enabling gl_PointSize in the vertex shader.
#endif
#ifdef GL_POINT_SPRITE
                glEnable(GL_POINT_SPRITE);        // Needed for enabling gl_PointCoord in the fragment shader.
#endif
				YsGLSLRenderer renderer(YsGLSLSharedFlashByPointSprite3DRenderer());
				YsGLSLSet3DRendererViewportDimensionf(renderer,(float)viewport[2],(float)viewport[3]);
				YsGLSLSet3DRendererPointSizeMode(renderer,YSGLSL_POINTSPRITE_SIZE_IN_3DSPACE);

				renderer.GetModelView(push);
				renderer.SetModelView(mat);
				unitPtr->GetActualBuffer()->DrawPrimitiveVtxColPointSize(renderer,GL_POINTS);
				renderer.SetModelView(push);

#ifdef GL_PROGRAM_POINT_SIZE
				glDisable(GL_PROGRAM_POINT_SIZE);
#endif
#ifdef GL_POINT_SPRITE
                glDisable(GL_POINT_SPRITE);
#endif
            }
		}
	}
	if(YSTRUE==opt.shadow)
	{
		if(NULL!=YsGLSLSharedFlat3DRenderer())
		{
			for(auto hd : vboSet.plainPlgPosHd)
			{
				auto unitPtr=bufManager.GetBufferUnit(hd);
				if(nullptr!=unitPtr && YsGLBufferManager::Unit::EMPTY!=unitPtr->GetState())
				{
					YsGLSLRenderer renderer(YsGLSLSharedFlat3DRenderer());
					renderer.SetUniformColor(0.0,0.0,0.0,1.0);
					renderer.GetModelView(push);
					renderer.SetModelView(mat);
					unitPtr->GetActualBuffer()->DrawPrimitiveVtx(renderer,GL_TRIANGLES);
					renderer.SetModelView(push);
				}
			}
		}
	}

	if(YSLEFT_ZPLUS_YPLUS==YsCoordSysModel)
	{
		glFrontFace(GL_CCW);
	}
	else if(YSRIGHT_ZMINUS_YPLUS==YsCoordSysModel)
	{
	}
}

