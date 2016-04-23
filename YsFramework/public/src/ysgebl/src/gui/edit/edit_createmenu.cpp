/* ////////////////////////////////////////////////////////////

File Name: edit_createmenu.cpp
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include "../ysgebl_gui_editor_base.h"
#include "../miscdlg.h"
#include "edit_mirrorimagedialog.h"
#include "edit_createprimitivedialog.h"
#include "edit_solidofrevolutiondialog.h"
#include <ysgl.h>
#include <ysshellext_boundaryutil.h>
#include <ysshellext_duplicateutil.h>
#include <ysshellext_orientationutil.h>
#include <ysshellextedit_duplicateutil.h>

#include "../textresource.h"

/*static*/ PolyCreCreatePrimitiveDialog *PolyCreCreatePrimitiveDialog::Create(GeblGuiEditorBase *canvas)
{
	PolyCreCreatePrimitiveDialog *dlg=new PolyCreCreatePrimitiveDialog;
	dlg->canvas=canvas;
	dlg->shapeType=0;
	dlg->division=16;
	dlg->orientation=0;
	return dlg;
}

/*static*/ void PolyCreCreatePrimitiveDialog::Delete(PolyCreCreatePrimitiveDialog *ptr)
{
	delete ptr;
}

void PolyCreCreatePrimitiveDialog::Make(void)
{
	Initialize();

	okBtn=AddTextButton(0,FSKEY_SPACE,FSGUI_PUSHBUTTON,FSGUI_DLG_COMMON_CREATE,YSTRUE);
	cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);

	const wchar_t * const primType[]=
	{
		FSGUI_DLG_CREATEPRIMITIVESHAPE_BOX,
		FSGUI_DLG_CREATEPRIMITIVESHAPE_CYLINDER,
		FSGUI_DLG_CREATEPRIMITIVESHAPE_ELLIPSOID,
		FSGUI_DLG_CREATEPRIMITIVESHAPE_CONE
	};


	primTypeDrp=AddDropList(0,FSKEY_NULL,"Primitive Type",4,primType,5,10,10,YSTRUE);
	primTypeDrp->Select(shapeType);

	orientationBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_DLG_CREATEPRIMITIVESHAPE_CHANGEORIENTATION,YSFALSE);

	numDivTxt=AddTextBox(0,FSKEY_NULL,FSGUI_DLG_COMMON_DIVCOUNT,L"16",5,YSTRUE);
	numDivTxt->SetTextType(FSGUI_INTEGER);
	numDivTxt->SetInteger(division);

	if(0==shapeType)
	{
		orientationBtn->Disable();
		numDivTxt->Disable();
	}
	else
	{
		orientationBtn->Enable();
		numDivTxt->Enable();
	}

	SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
	SetBackgroundAlpha(0.3);
	Fit();
}

void PolyCreCreatePrimitiveDialog::PrepareShape(void)
{
	tmpShl.CleanUp();
	vtxBuf.CleanUp();

	shapeType=primTypeDrp->GetSelection();
	division=YsGreater(3,numDivTxt->GetInteger());
	switch(shapeType)
	{
	case 0: // Box
		PrepareBox();
		break;
	case 1: // Cylinder
		PrepareCylinder();
		break;
	case 2: // Ellipsoid
		PrepareEllipsoid();
		break;
	case 3: // Cone
		PrepareCone();
		break;
	}
}

void PolyCreCreatePrimitiveDialog::GetTransformation(YsVec3 &cen,YsVec3 &uVec,YsVec3 &vVec,YsVec3 &wVec) const
{
	cen=(box[0]+box[1])/2.0;

	const double dx=(box[1].x()-box[0].x())/2.0;
	const double dy=(box[1].y()-box[0].y())/2.0;
	const double dz=(box[1].z()-box[0].z())/2.0;

	switch(orientation%6)
	{
	case 0:
		uVec.Set( dx,0.0,0.0);
		vVec.Set(0.0, dy,0.0);
		wVec.Set(0.0,0.0, dz);
		break;
	case 1:
		uVec.Set( dx,0.0,0.0);
		vVec.Set(0.0,0.0,-dz);
		wVec.Set(0.0, dy,0.0);
		break;
	case 2:
		uVec.Set(0.0, dy,0.0);
		vVec.Set(0.0,0.0,-dz);
		wVec.Set(-dx,0.0,0.0);
		break;
	case 3:
		uVec.Set(-dx,0.0,0.0);
		vVec.Set(0.0,-dy,0.0);
		wVec.Set(0.0,0.0,-dz);
		break;
	case 4:
		uVec.Set(-dx,0.0,0.0);
		vVec.Set(0.0,0.0, dz);
		wVec.Set(0.0,-dy,0.0);
		break;
	case 5:
		uVec.Set(0.0,-dy,0.0);
		vVec.Set(0.0,0.0, dz);
		wVec.Set( dx,0.0,0.0);
		break;
	}
}

void PolyCreCreatePrimitiveDialog::PrepareBox(void)
{
	const YsVec3 boxVtPos[8]=
	{
		YsVec3(box[0].x(),box[0].y(),box[0].z()),
		YsVec3(box[1].x(),box[0].y(),box[0].z()),
		YsVec3(box[1].x(),box[0].y(),box[1].z()),
		YsVec3(box[0].x(),box[0].y(),box[1].z()),
		YsVec3(box[0].x(),box[1].y(),box[0].z()),
		YsVec3(box[1].x(),box[1].y(),box[0].z()),
		YsVec3(box[1].x(),box[1].y(),box[1].z()),
		YsVec3(box[0].x(),box[1].y(),box[1].z())
	};
	YsShellVertexHandle boxVtHd[8]=
	{
		tmpShl.AddVertex(boxVtPos[0]),
		tmpShl.AddVertex(boxVtPos[1]),
		tmpShl.AddVertex(boxVtPos[2]),
		tmpShl.AddVertex(boxVtPos[3]),
		tmpShl.AddVertex(boxVtPos[4]),
		tmpShl.AddVertex(boxVtPos[5]),
		tmpShl.AddVertex(boxVtPos[6]),
		tmpShl.AddVertex(boxVtPos[7])
	};
	const int boxVtIdx[6][4]={
		{0,1,2,3},
		{1,0,4,5},
		{2,1,5,6},
		{3,2,6,7},
		{0,3,7,4},
		{7,6,5,4}
	};
	const int boxEdVtIdx[12][2]=
	{
		{0,1},
		{1,2},
		{2,3},
		{3,0},
		{0,4},
		{1,5},
		{2,6},
		{3,7},
		{4,5},
		{5,6},
		{6,7},
		{7,4}
	};
	const YsVec3 boxNom[6]=
	{
		-YsYVec(),
		-YsZVec(),
		 YsXVec(),
		 YsZVec(),
		-YsXVec(),
		 YsYVec()
	};

	for(int i=0; i<6; ++i)
	{
		const YsShellVertexHandle quadVtHd[4]=
		{
			boxVtHd[boxVtIdx[i][0]],
			boxVtHd[boxVtIdx[i][1]],
			boxVtHd[boxVtIdx[i][2]],
			boxVtHd[boxVtIdx[i][3]]
		};
		YsShellPolygonHandle plHd[1]={tmpShl.AddPolygon(4,quadVtHd)};
		tmpShl.SetPolygonNormal(plHd[0],boxNom[i]);
		tmpShl.AddFaceGroup(1,plHd);
	}

	for(int i=0; i<12; ++i)
	{
		const YsShellVertexHandle edVtHd[2]=
		{
			boxVtHd[boxEdVtIdx[i][0]],boxVtHd[boxEdVtIdx[i][1]]
		};
		tmpShl.AddConstEdge(2,edVtHd,YSFALSE);
	}
}

void PolyCreCreatePrimitiveDialog::PrepareCylinder(void)
{
	YsVec3 cen,u,v,w;
	GetTransformation(cen,u,v,w);

	YsArray <YsShellVertexHandle> vtHdArray[2];
	for(int i=0; i<division; ++i)
	{
		const double angle=YsPi*2.0*(double)i/(double)division;
		const double c=cos(angle);
		const double s=sin(angle);

		const YsVec3 pos[2]=
		{
			cen+u*c+v*s+w,
			cen+u*c+v*s-w,
		};

		vtHdArray[0].Append(tmpShl.AddVertex(pos[0]));
		vtHdArray[1].Append(tmpShl.AddVertex(pos[1]));
	}

	for(int i=0; i<division; ++i)
	{
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[0][i]));
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[1][i]));
	}
	for(int i=0; i<division; ++i)
	{
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[0][i]));
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[0].GetCyclic(i+1)));
	}
	for(int i=0; i<division; ++i)
	{
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[1][i]));
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[1].GetCyclic(i+1)));
	}

	YsShellPolygonHandle plHd=tmpShl.AddPolygon(vtHdArray[0]);
	tmpShl.AddFaceGroup(1,&plHd);

	plHd=tmpShl.AddPolygon(vtHdArray[1]);
	tmpShl.AddFaceGroup(1,&plHd);

	tmpShl.AddConstEdge(vtHdArray[0],YSTRUE);
	tmpShl.AddConstEdge(vtHdArray[1],YSTRUE);

	YsArray <YsShellPolygonHandle> sidePlHd;
	for(int i=0; i<division; ++i)
	{
		YsShellVertexHandle vtHd[4]=
		{
			vtHdArray[0][i],
			vtHdArray[0].GetCyclic(i+1),
			vtHdArray[1].GetCyclic(i+1),
			vtHdArray[1][i]
		};
		sidePlHd.Append(tmpShl.AddPolygon(4,vtHd));
	}

	tmpShl.AddFaceGroup(sidePlHd);

	for(int i=0; i<4; ++i)
	{
		const int n=i*division/4;
		const YsShellVertexHandle vtHd[2]={vtHdArray[0][n],vtHdArray[1][n]};
		tmpShl.AddConstEdge(2,vtHd,YSFALSE);
	}
}

void PolyCreCreatePrimitiveDialog::PrepareEllipsoid(void)
{
	YsVec3 cen,U,V,W;
	GetTransformation(cen,U,V,W);

	YsArray <YsShellVertexHandle> vtHdMatrix;

	const int nx=division*2,ny=division;

	for(int y=0; y<=ny; ++y)
	{
		const double lat=YsPi*(double)y/(double)ny-YsPi/2.0;
		for(int x=0; x<nx; ++x)
		{
			if((0==y || ny==y) && 0<x)
			{
				YsShellVertexHandle vtHd0=vtHdMatrix.Last();
				vtHdMatrix.Append(vtHd0);
			}
			else
			{
				const double lng=YsPi*2.0*(double)x/(double)nx;

				const double u=cos(lng)*cos(lat);
				const double v=sin(lng)*cos(lat);
				const double w=sin(lat);

				const YsVec3 pos=cen+U*u+V*v+W*w;
				vtHdMatrix.Append(tmpShl.AddVertex(pos));
			}
		}
	}

	for(int y=0; y<ny; ++y)
	{
		for(int x=0; x<nx; ++x)
		{
			const YsVec3 pos[3]=
			{
				tmpShl.GetVertexPosition(vtHdMatrix[ y   *nx+x]),
				tmpShl.GetVertexPosition(vtHdMatrix[(y+1)*nx+x]),
				tmpShl.GetVertexPosition(vtHdMatrix[ y   *nx+(x+1)%nx]),
			};

			vtxBuf.AddVertex(pos[0]);
			vtxBuf.AddVertex(pos[1]);
			vtxBuf.AddVertex(pos[0]);
			vtxBuf.AddVertex(pos[2]);
		}
	}

	YsArray <YsShellPolygonHandle> plHdMatrix;
	for(int y=0; y<ny; ++y)
	{
		for(int x=0; x<nx; ++x)
		{
			if(0==y)
			{
				const YsShellVertexHandle vtHd[3]=
				{
					vtHdMatrix[ y   *nx+x],
					vtHdMatrix[(y+1)*nx+x],
					vtHdMatrix[(y+1)*nx+(x+1)%nx]
				};
				plHdMatrix.Append(tmpShl.AddPolygon(3,vtHd));
			}
			else if(ny-1==y)
			{
				const YsShellVertexHandle vtHd[3]=
				{
					vtHdMatrix[ y   *nx+x],
					vtHdMatrix[(y+1)*nx+x],
					vtHdMatrix[ y   *nx+(x+1)%nx]
				};
				plHdMatrix.Append(tmpShl.AddPolygon(3,vtHd));
			}
			else
			{
				const YsShellVertexHandle vtHd[4]=
				{
					vtHdMatrix[ y   *nx+x],
					vtHdMatrix[(y+1)*nx+x],
					vtHdMatrix[(y+1)*nx+(x+1)%nx],
					vtHdMatrix[ y   *nx+(x+1)%nx]
				};
				plHdMatrix.Append(tmpShl.AddPolygon(4,vtHd));
			}
		}
	}

	tmpShl.AddFaceGroup(plHdMatrix);

	tmpShl.AddConstEdge(nx,((const YsShellVertexHandle *)vtHdMatrix)+(ny/2)*nx,YSTRUE);

	for(int i=0; i<4; ++i)
	{
		YsArray <YsShellVertexHandle> vtHdArray;
		const int x=nx*i/4;

		for(int y=0; y<=ny; ++y)
		{
			vtHdArray.Append(vtHdMatrix[y*nx+x]);
		}

		tmpShl.AddConstEdge(vtHdArray,YSFALSE);
	}
}

void PolyCreCreatePrimitiveDialog::PrepareCone(void)
{
	YsVec3 cen,u,v,w;
	GetTransformation(cen,u,v,w);

	YsShellVertexHandle peakVtHd=tmpShl.AddVertex(cen+w);

	YsArray <YsShellVertexHandle> vtHdArray;
	for(int i=0; i<division; ++i)
	{
		const double angle=YsPi*2.0*(double)i/(double)division;
		const double c=cos(angle);
		const double s=sin(angle);

		vtHdArray.Append(tmpShl.AddVertex(cen+u*c+v*s-w));
	}

	for(int i=0; i<division; ++i)
	{
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(peakVtHd));
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[i]));
	}
	for(int i=0; i<division; ++i)
	{
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray[i]));
		vtxBuf.AddVertex(tmpShl.GetVertexPosition(vtHdArray.GetCyclic(i+1)));
	}


	YsShellPolygonHandle plHd=tmpShl.AddPolygon(vtHdArray);
	tmpShl.AddFaceGroup(1,&plHd);

	tmpShl.AddConstEdge(vtHdArray,YSTRUE);

	YsArray <YsShellPolygonHandle> sidePlHd;
	for(int i=0; i<division; ++i)
	{
		YsShellVertexHandle vtHd[3]=
		{
			vtHdArray[i],
			vtHdArray.GetCyclic(i+1),
			peakVtHd
		};
		sidePlHd.Append(tmpShl.AddPolygon(3,vtHd));
	}

	tmpShl.AddFaceGroup(sidePlHd);

	for(int i=0; i<4; ++i)
	{
		const int n=i*division/4;
		const YsShellVertexHandle vtHd[2]={vtHdArray[n],peakVtHd};
		tmpShl.AddConstEdge(2,vtHd,YSFALSE);
	}
}

void PolyCreCreatePrimitiveDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		canvas->Edit_CreatePrimitive_SpaceKeyCallBack(*canvas);
	}
	else if(btn==cancelBtn)
	{
		canvas->Edit_ClearUIMode();
	}
	else if(btn==orientationBtn)
	{
		orientation=(orientation+1)%6;
		PrepareShape();
	}
}

void PolyCreCreatePrimitiveDialog::OnDropListSelChange(FsGuiDropList *drp,int)
{
	if(drp==primTypeDrp)
	{
		const int sel=primTypeDrp->GetSelection();
		if(0==sel)
		{
			orientationBtn->Disable();
			numDivTxt->Disable();
		}
		else
		{
			orientationBtn->Enable();
			numDivTxt->Enable();
		}
		PrepareShape();
	}
}

void PolyCreCreatePrimitiveDialog::OnTextBoxChange(FsGuiTextBox *txt)
{
	if(txt==numDivTxt)
	{
		PrepareShape();
	}
}

void GeblGuiEditorBase::Edit_CreatePrimitiveShape(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase *canvas=(GeblGuiEditorBase *)appPtr;

	if(NULL!=canvas->slHd)
	{
		Edit_ClearUIMode(appPtr,NULL,NULL);

		YsVec3 defBox[2]=
		{
			-YsXYZ()*5.0,YsXYZ()*5.0
		};

		canvas->threeDInterface.BeginInputBox(defBox[0],defBox[1],/*allowResize=*/YSTRUE,/*maintainAspect=*/YSFALSE);
		canvas->SetNeedRedraw(YSTRUE);

		canvas->SpaceKeyCallBack=Edit_CreatePrimitive_SpaceKeyCallBack;
		canvas->UIDrawCallBack2D=NULL;
		canvas->UIDrawCallBack3D=Edit_CreatePrimitive_UIDrawCallBack3D;
		canvas->threeDInterface.SetCallBack(Edit_CreatePrimitive_CursorMoved,canvas);

		canvas->createPrimitiveDlg->Make();
		canvas->AddDialog(canvas->createPrimitiveDlg);
		canvas->ArrangeDialog();

		canvas->createPrimitiveDlg->box[0]=defBox[0];
		canvas->createPrimitiveDlg->box[1]=defBox[1];
		canvas->createPrimitiveDlg->PrepareShape();

		canvas->undoRedoRequireToClearUIMode=YSFALSE;
		canvas->deletionRequireToClearUIMode=YSFALSE;
	}
}



YSRESULT GeblGuiEditorBase::Edit_CreatePrimitive_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	if(NULL!=canvas.slHd)
	{
		const YsVec3 boxPos[2]=
		{
			canvas.threeDInterface.box_pos[0],canvas.threeDInterface.box_pos[1]
		};
		canvas.createPrimitiveDlg->box[0]=boxPos[0];
		canvas.createPrimitiveDlg->box[1]=boxPos[1];
		canvas.createPrimitiveDlg->PrepareShape();


		{
			YsShellExt_OrientationUtil orientationUtil;
			orientationUtil.CleanUp();
			orientationUtil.FixOrientationOfClosedSolid(canvas.createPrimitiveDlg->tmpShl);
			auto plHdNeedFip=orientationUtil.GetPolygonNeedFlip();

			for(auto plHd : plHdNeedFip)
			{
				YsArray <YsShellVertexHandle,4> plVtHd;
				canvas.createPrimitiveDlg->tmpShl.GetPolygon(plVtHd,plHd);
				plVtHd.Invert();
				canvas.createPrimitiveDlg->tmpShl.SetPolygonVertex(plHd,plVtHd);
			}

			orientationUtil.RecalculateNormalFromCurrentOrientation((const YsShell &)canvas.createPrimitiveDlg->tmpShl);
			auto plNomPairNeedNormalReset=orientationUtil.GetPolygonNormalPair();
			for(auto plNomPair : plNomPairNeedNormalReset)
			{
				canvas.createPrimitiveDlg->tmpShl.SetPolygonNormal(plNomPair.plHd,plNomPair.nom);
			}
		}


		YsShellExtEdit_DuplicateUtil dupUtil;
		dupUtil.SetShell(canvas.createPrimitiveDlg->tmpShl);
		dupUtil.AddVertex(canvas.createPrimitiveDlg->tmpShl.AllVertex().Array());
		dupUtil.AddPolygon(canvas.createPrimitiveDlg->tmpShl.AllPolygon().Array());
		dupUtil.AddConstEdge(canvas.createPrimitiveDlg->tmpShl.AllConstEdge().Array());
		dupUtil.AddFaceGroup(canvas.createPrimitiveDlg->tmpShl.AllFaceGroup().Array());
		dupUtil.AddVolume(canvas.createPrimitiveDlg->tmpShl.AllVolume().Array());


		YsShellExtEdit::StopIncUndo incUndo(canvas.slHd);
		dupUtil.Paste(*(YsShellExtEdit *)canvas.slHd);

		canvas.Edit_ClearUIMode();
		canvas.needRemakeDrawingBuffer=NEED_REMAKE_DRAWING_VERTEX|NEED_REMAKE_DRAWING_POLYGON|NEED_REMAKE_DRAWING_CONSTEDGE;
		canvas.SetNeedRedraw(YSTRUE);

		return YSOK;
	}
	return YSERR;
}

/*static*/ void GeblGuiEditorBase::Edit_CreatePrimitive_CursorMoved(void *appPtr,FsGui3DInterface &itfc)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;

	itfc.GetBox(canvas.createPrimitiveDlg->box);
	canvas.createPrimitiveDlg->PrepareShape();

printf("%s %d\n",__FUNCTION__,__LINE__);
}

/*static*/ void GeblGuiEditorBase::Edit_CreatePrimitive_UIDrawCallBack3D(GeblGuiEditorBase &canvas)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,0.5f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINES,(int)canvas.createPrimitiveDlg->vtxBuf.GetNumVertex(),canvas.createPrimitiveDlg->vtxBuf);

	YsGLSLEndUse3DRenderer(renderer);
}



////////////////////////////////////////////////////////////

PolyCreEditCreateMirrorImageDialog::PolyCreEditCreateMirrorImageDialog()
{
}

PolyCreEditCreateMirrorImageDialog::~PolyCreEditCreateMirrorImageDialog()
{
}

/*static*/ PolyCreEditCreateMirrorImageDialog *PolyCreEditCreateMirrorImageDialog::Create(void)
{
	return new PolyCreEditCreateMirrorImageDialog;
}

/*static*/ void PolyCreEditCreateMirrorImageDialog::Delete(PolyCreEditCreateMirrorImageDialog *ptr)
{
	delete ptr;
}

void PolyCreEditCreateMirrorImageDialog::Make(class GeblGuiEditorBase *canvas)
{
	FsGuiDialog::Initialize();

	this->canvas=canvas;

	const char * const symPlaneType[5]=
	{
		"XY-Plane",
		"XZ-Plane",
		"YZ-Plane",
		"Selected Polygon",
		"Three Vertices"
	};

	symPlnDrp=AddDropList(0,FSKEY_NULL,"Plane of Symmetry",5,symPlaneType,5,40,40,YSTRUE);
	symPlnDrp->Select(2);
	okBtn=AddTextButton(0,FSKEY_SPACE,FSGUI_PUSHBUTTON,L"Create",YSTRUE);
	cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,L"Cancel",YSFALSE);

	Fit();
	SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
	SetBackgroundAlpha(0.3);
}

void PolyCreEditCreateMirrorImageDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		canvas->Edit_CreateMirrorImage_Create();
		canvas->Edit_ClearUIMode();
	}
	else if(btn==cancelBtn)
	{
		canvas->Edit_ClearUIMode();
	}
}

void PolyCreEditCreateMirrorImageDialog::OnDropListSelChange(FsGuiDropList *drp,int)
{
	if(drp==symPlnDrp)
	{
	}
}

YSSHELLEXT_PLANE_TYPE PolyCreEditCreateMirrorImageDialog::GetSymmPlaneType(void) const
{
	switch(symPlnDrp->GetSelection())
	{
	case 0: // "XY-Plane",
		return YSSHELLEXT_PLANE_XY;
	case 1: // "XZ-Plane",
		return YSSHELLEXT_PLANE_XZ;
	case 2: // "YZ-Plane",
		return YSSHELLEXT_PLANE_YZ;
	case 3: // "Selected Polygon",
		return YSSHELLEXT_PLANE_POLYGON;
	case 4: // "Three Vertices"
		return YSSHELLEXT_PLANE_3VERTEX;
	}
	return YSSHELLEXT_PLANE_NULL;
}

/*static*/ void GeblGuiEditorBase::Edit_CreateMirrorImage(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	auto canvas=(GeblGuiEditorBase *)appPtr;
	if(NULL!=canvas->slHd)
	{
		canvas->Edit_ClearUIMode();

		canvas->mirrorImageDlg->Make(canvas);
		canvas->AddDialog(canvas->mirrorImageDlg);
		canvas->ArrangeDialog();

		canvas->SetNeedRedraw(YSTRUE);
	}
}

void GeblGuiEditorBase::Edit_CreateMirrorImage_Create(void)
{
	if(NULL!=slHd)
	{
		auto allVtHd=slHd->AllVertex().Array();
		auto allPlHd=slHd->AllPolygon().Array();
		auto allCeHd=slHd->AllConstEdge().Array();
		auto allFgHd=slHd->AllFaceGroup().Array();
		auto allVlHd=slHd->AllVolume().Array();

		YsShellExtEdit_DuplicateUtil dupUtil;
		dupUtil.CleanUp();
		dupUtil.SetShell(*(YsShellExt *)(slHd));
		dupUtil.AddVertex(allVtHd);
		dupUtil.AddPolygon(allPlHd);
		dupUtil.AddConstEdge(allCeHd);
		dupUtil.AddFaceGroup(allFgHd);
		dupUtil.AddVolume(allVlHd);

		const YSBOOL dontCopyOnSymmPlane=YSTRUE;
		const YSBOOL deletePolygonOnSymmPlane=YSTRUE;
		switch(mirrorImageDlg->GetSymmPlaneType())
		{
		case YSSHELLEXT_PLANE_XY:
			dupUtil.MakeAxisymmetric(*slHd,2,dontCopyOnSymmPlane,deletePolygonOnSymmPlane);
			break;
		case YSSHELLEXT_PLANE_XZ:
			dupUtil.MakeAxisymmetric(*slHd,1,dontCopyOnSymmPlane,deletePolygonOnSymmPlane);
			break;
		case YSSHELLEXT_PLANE_YZ:
			dupUtil.MakeAxisymmetric(*slHd,0,dontCopyOnSymmPlane,deletePolygonOnSymmPlane);
			break;
		case YSSHELLEXT_PLANE_3VERTEX:
			{
				YsArray <YsShellVertexHandle> selVtHd;
				slHd->GetSelectedVertex(selVtHd);
				if(3==selVtHd.GetN())
				{
					YsVec3 pos[3]=
					{
						slHd->GetVertexPosition(selVtHd[0]),
						slHd->GetVertexPosition(selVtHd[1]),
						slHd->GetVertexPosition(selVtHd[2])
					};
					YsPlane pln;
					pln.MakePlaneFromTriangle(pos[0],pos[1],pos[2]);
					dupUtil.MakeBySymmetryPlane(*slHd,pln,dontCopyOnSymmPlane,deletePolygonOnSymmPlane);
				}
				else
				{
					MessageDialog(L"Error",L"3 vertices need to be selected.");
				}
			}
			break;
		case YSSHELLEXT_PLANE_POLYGON:
			{
				YsArray <YsShellPolygonHandle> selPlHd;
				slHd->GetSelectedPolygon(selPlHd);
				if(1==selPlHd.GetN())
				{
					YsArray <YsVec3> plVtPos;
					slHd->GetPolygon(plVtPos,selPlHd[0]);

					YsPlane pln;
					pln.MakeBestFitPlane(plVtPos.GetN(),plVtPos);
					dupUtil.MakeBySymmetryPlane(*slHd,pln,dontCopyOnSymmPlane,deletePolygonOnSymmPlane);
				}
				else
				{
					MessageDialog(L"Error",L"One polygon needs to be selected.");
				}
			}
			break;
		default:
			break;
		}

		needRemakeDrawingBuffer=(unsigned int)NEED_REMAKE_DRAWING_ALL;
		SetNeedRedraw(YSTRUE);
	}
}

////////////////////////////////////////////////////////////

PolyCreParallelogramDialog::PolyCreParallelogramDialog()
{
	canvas=NULL;
	okBtn=NULL;
	cancelBtn=NULL;
	closeLoopBtn=NULL;
}

PolyCreParallelogramDialog::~PolyCreParallelogramDialog()
{
}

/*static*/ PolyCreParallelogramDialog *PolyCreParallelogramDialog::Create(class GeblGuiEditorBase *canvas)
{
	PolyCreParallelogramDialog *dlg=new PolyCreParallelogramDialog;
	dlg->canvas=canvas;
	return dlg;
}

/*static*/ void PolyCreParallelogramDialog::Delete(PolyCreParallelogramDialog *ptr)
{
	delete ptr;
}

YSRESULT PolyCreParallelogramDialog::Make(void)
{
	if(NULL==okBtn)
	{
		FsGuiDialog::Initialize();
		okBtn=AddTextButton(0,FSKEY_ENTER,FSGUI_PUSHBUTTON,FSGUI_COMMON_OK,YSTRUE);
		cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);
		changePointSchemeBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_DLG_INSERTVERTEX_CHANGESCHEME,YSFALSE);
		cancelLastBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_DLG_PARALLELOGRAM_CANCELLAST,YSFALSE);
		closeLoopBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_DLG_PARALLELOGRAM_CLOSELOOP,YSFALSE);
		closeLoopFloorCeilingBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_DLG_PARALLELOGRAM_CLOSELOOPFLOORCEILING,YSFALSE);

		SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
		SetBackgroundAlpha(0.3);
		Fit();
	}

	plgChain.CleanUp();
	topRow.CleanUp();
	bottomRow.CleanUp();

	closeLoopBtn->Disable();
	closeLoopFloorCeilingBtn->Disable();
	cancelLastBtn->Disable();
	lineBuf.CleanUp();
	return YSOK;
}

void PolyCreParallelogramDialog::RemakeDrawingBuffer(void)
{
	// As required
}

void PolyCreParallelogramDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		canvas->Edit_CreateParallelogramFromTwoVertex_SpaceKeyCallBack();
	}
	else if(btn==cancelBtn)
	{
		canvas->Edit_ClearUIMode();
	}
	else if(btn==closeLoopBtn && NULL!=canvas->Slhd() && 3<=topRow.GetN())
	{
		const YsShellVertexHandle quadVtHd[4]=
		{
			topRow.Last(),
			topRow[0],
			bottomRow[0],
			bottomRow.Last()
		};
		canvas->Slhd()->AddPolygon(4,quadVtHd);
		canvas->Edit_ClearUIMode();

		canvas->needRemakeDrawingBuffer=GeblGuiEditorBase::NEED_REMAKE_DRAWING_SELECTED_VERTEX|GeblGuiEditorBase::NEED_REMAKE_DRAWING_POLYGON|GeblGuiEditorBase::NEED_REMAKE_DRAWING_VERTEX;
		canvas->SetNeedRedraw(YSTRUE);
	}
	else if(btn==closeLoopFloorCeilingBtn)
	{
		const YsShellVertexHandle quadVtHd[4]=
		{
			topRow.Last(),
			topRow[0],
			bottomRow[0],
			bottomRow.Last()
		};
		YsShellExtEdit::StopIncUndo incUndo(*(canvas->Slhd()));

		canvas->Slhd()->AddPolygon(4,quadVtHd);
		canvas->Slhd()->AddPolygon(topRow);
		canvas->Slhd()->AddPolygon(bottomRow);
		canvas->Edit_ClearUIMode();

		canvas->needRemakeDrawingBuffer=GeblGuiEditorBase::NEED_REMAKE_DRAWING_SELECTED_VERTEX|GeblGuiEditorBase::NEED_REMAKE_DRAWING_POLYGON|GeblGuiEditorBase::NEED_REMAKE_DRAWING_VERTEX;
		canvas->SetNeedRedraw(YSTRUE);
	}
	else if(btn==changePointSchemeBtn)
	{
		if(FsGui3DInterface::IFTYPE_POINT==canvas->threeDInterface.GetInterfaceType())
		{
			canvas->threeDInterface.ChangeInputPoint1to2(NULL,NULL);
			canvas->SetNeedRedraw(YSTRUE);
		}
		else
		{
			canvas->threeDInterface.ChangeInputPoint2to1();
			canvas->SetNeedRedraw(YSTRUE);
		}
	}
	else if(btn==cancelLastBtn)
	{
		if(0<plgChain.GetN())
		{
			YsShellExtEdit::StopIncUndo incUndo(*(canvas->Slhd()));

			canvas->Slhd()->DeletePolygon(plgChain.Last());
			canvas->Slhd()->DeleteVertex(topRow.Last());
			canvas->Slhd()->DeleteVertex(bottomRow.Last());
			canvas->needRemakeDrawingBuffer=GeblGuiEditorBase::NEED_REMAKE_DRAWING_SELECTED_VERTEX|GeblGuiEditorBase::NEED_REMAKE_DRAWING_POLYGON|GeblGuiEditorBase::NEED_REMAKE_DRAWING_VERTEX;

			plgChain.DeleteLast();
			topRow.DeleteLast();
			bottomRow.DeleteLast();

			if(0==plgChain.GetN())
			{
				cancelLastBtn->Disable();
			}
			if(topRow.GetN()<3)
			{
				closeLoopBtn->Disable();
				closeLoopFloorCeilingBtn->Disable();
			}
		}
	}
}

void PolyCreParallelogramDialog::OnDropListSelChange(FsGuiDropList *,int)
{
}

void PolyCreParallelogramDialog::OnTextBoxChange(FsGuiTextBox *)
{
}

void PolyCreParallelogramDialog::OnSliderPositionChange(FsGuiSlider *,const double &,const double &)
{
}

/*static*/ void GeblGuiEditorBase::Edit_CreateParallelogramFromTwoVertex(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL==canvas.Slhd())
	{
		return;
	}

	YsShellExtEdit &shl=*(canvas.Slhd());
	auto selVtHd=shl.GetSelectedVertex();
	if(2==selVtHd.GetN())
	{
		auto edgeCen=shl.GetCenter(selVtHd);

		canvas.Edit_ClearUIMode();

		if(YSOK==canvas.parallelogramDlg->Make())
		{
			canvas.parallelogramDlg->topRow.Append(selVtHd[0]);
			canvas.parallelogramDlg->bottomRow.Append(selVtHd[1]);

			canvas.AddDialog(canvas.parallelogramDlg);
			canvas.ArrangeDialog();

			canvas.UIDrawCallBack3D=Edit_CreateParallelogramFromTwoVertex_DrawCallBack3D;
			canvas.SpaceKeyCallBack=Edit_CreateParallelogramFromTwoVertex_SpaceKeyCallBack;

			canvas.threeDInterface.BeginInputPoint1(edgeCen);
			canvas.threeDInterface.SetCallBack(Edit_CreateParallelogramFromTwoVertex_3DInterfaceCallBack,&canvas);

			canvas.undoRedoRequireToClearUIMode=YSTRUE;
			canvas.deletionRequireToClearUIMode=YSTRUE;

			canvas.SetNeedRedraw(YSTRUE);
		}
	}
	else
	{
		canvas.MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_NEEDSELECT2VERTEX);
	}
}

/*static*/ void GeblGuiEditorBase::Edit_CreateParallelogramFromTwoVertex_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,0.5f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINE_LOOP,(int)canvas.parallelogramDlg->lineBuf.GetNumVertex(),canvas.parallelogramDlg->lineBuf);

	YsGLSLEndUse3DRenderer(renderer);
}

/*static*/ void GeblGuiEditorBase::Edit_CreateParallelogramFromTwoVertex_3DInterfaceCallBack(void *appPtr,class FsGui3DInterface &)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL==canvas.Slhd())
	{
		return;
	}

	YsShellExtEdit &shl=*(canvas.Slhd());

	canvas.parallelogramDlg->lineBuf.CleanUp();

	const YsVec3 refVtPos[2]=
	{
		shl.GetVertexPosition(canvas.parallelogramDlg->topRow.Last()),
		shl.GetVertexPosition(canvas.parallelogramDlg->bottomRow.Last())
	};
	const YsVec3 refCen=(refVtPos[0]+refVtPos[1])/2.0;
	const YsVec3 mov=canvas.threeDInterface.point_pos-refCen;
	const YsVec3 newVtPos[2]=
	{
		refVtPos[0]+mov,
		refVtPos[1]+mov
	};

	canvas.parallelogramDlg->lineBuf.AddVertex(refVtPos[0]);
	canvas.parallelogramDlg->lineBuf.AddVertex(newVtPos[0]);
	canvas.parallelogramDlg->lineBuf.AddVertex(newVtPos[1]);
	canvas.parallelogramDlg->lineBuf.AddVertex(refVtPos[1]);
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_CreateParallelogramFromTwoVertex_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_CreateParallelogramFromTwoVertex_SpaceKeyCallBack();
}

YSRESULT GeblGuiEditorBase::Edit_CreateParallelogramFromTwoVertex_SpaceKeyCallBack(void)
{
	if(NULL==Slhd())
	{
		return YSERR;
	}
	YsShellExtEdit &shl=*Slhd();

	const YsVec3 refVtPos[2]=
	{
		shl.GetVertexPosition(parallelogramDlg->topRow.Last()),
		shl.GetVertexPosition(parallelogramDlg->bottomRow.Last())
	};
	const YsVec3 refCen=(refVtPos[0]+refVtPos[1])/2.0;
	const YsVec3 mov=threeDInterface.point_pos-refCen;
	if(YsOrigin()==mov)
	{
		return YSERR;
	}

	YsShellExtEdit::StopIncUndo incUndo(shl);

	const YsVec3 newVtPos[2]=
	{
		refVtPos[0]+mov,
		refVtPos[1]+mov
	};

	const YsShellVertexHandle newVtHd[2]=
	{
		shl.AddVertex(newVtPos[0]),
		shl.AddVertex(newVtPos[1])
	};

	parallelogramDlg->topRow.Append(newVtHd[0]);
	parallelogramDlg->bottomRow.Append(newVtHd[1]);

	const YsShellVertexHandle quadVtHd[4]=
	{
		parallelogramDlg->topRow.Last(),
		parallelogramDlg->topRow.GetCyclic(-2),
		parallelogramDlg->bottomRow.GetCyclic(-2),
		parallelogramDlg->bottomRow.Last()
	};

	auto plHd=shl.AddPolygon(4,quadVtHd);
	parallelogramDlg->plgChain.Append(plHd);
	parallelogramDlg->cancelLastBtn->Enable();

	if(3<=parallelogramDlg->topRow.GetN())
	{
		parallelogramDlg->closeLoopBtn->Enable();
		parallelogramDlg->closeLoopFloorCeilingBtn->Enable();
	}

	needRemakeDrawingBuffer|=NEED_REMAKE_DRAWING_VERTEX|NEED_REMAKE_DRAWING_POLYGON|NEED_REMAKE_DRAWING_SELECTED_VERTEX;
	SetNeedRedraw(YSTRUE);
	return YSOK;
}


////////////////////////////////////////////////////////////

/*static*/ void GeblGuiEditorBase::Edit_CreateParallelogramFromThreeVertex(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL!=canvas.Slhd())
	{
		auto selVtHd=canvas.Slhd()->GetSelectedVertex();
		if(3==selVtHd.GetN())
		{
			const YsVec3 vtPos[3]=
			{
				canvas.Slhd()->GetVertexPosition(selVtHd[0]),
				canvas.Slhd()->GetVertexPosition(selVtHd[1]),
				canvas.Slhd()->GetVertexPosition(selVtHd[2])
			};
			const YsVec3 o=vtPos[1],v1=vtPos[0]-vtPos[1],v2=vtPos[2]-vtPos[1];
			const YsVec3 newVtPos=o+v1+v2;

			YsShellExtEdit::StopIncUndo incUndo(canvas.Slhd());
			YsShellVertexHandle newPlVtHd[4]=
			{
				selVtHd[0],
				selVtHd[1],
				selVtHd[2],
				canvas.Slhd()->AddVertex(newVtPos)
			};
			auto newPlHd=canvas.Slhd()->AddPolygon(4,newPlVtHd);
			canvas.Slhd()->SetPolygonColor(newPlHd,canvas.colorPaletteDlg->GetColor());

			canvas.Slhd()->SelectVertex(0,NULL);

			canvas.needRemakeDrawingBuffer=NEED_REMAKE_DRAWING_SELECTED_VERTEX|NEED_REMAKE_DRAWING_POLYGON|NEED_REMAKE_DRAWING_VERTEX;
			canvas.SetNeedRedraw(YSTRUE);
			
		}
		else
		{
			canvas.MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_NEEDSELECT3VERTEX);
		}
	}
}

////////////////////////////////////////////////////////////

PolyCreEquilateralDialog::PolyCreEquilateralDialog()
{
	canvas=NULL;
	okBtn=NULL;
	cancelBtn=NULL;
}

PolyCreEquilateralDialog::~PolyCreEquilateralDialog()
{
}

/*static*/ PolyCreEquilateralDialog *PolyCreEquilateralDialog::Create(class GeblGuiEditorBase *canvas)
{
	PolyCreEquilateralDialog *dlg=new PolyCreEquilateralDialog;
	dlg->canvas=canvas;
	return dlg;
}

/*static*/ void PolyCreEquilateralDialog::Delete(PolyCreEquilateralDialog *ptr)
{
	delete ptr;
}

YSRESULT PolyCreEquilateralDialog::Make(void)
{
	if(NULL==okBtn)
	{
		FsGuiDialog::Initialize();

		AddStaticText(0,FSKEY_NULL,FSGUI_DLG_EQUILATERAL_CURSOR_AS,YSTRUE);
		const wchar_t * const cursorAsMsg[3]=
		{
			FSGUI_DLG_EQUILATERAL_CURSOR_AS_TANGENT,
			FSGUI_DLG_EQUILATERAL_CURSOR_AS_NORMAL,
			FSGUI_DLG_EQUILATERAL_CURSOR_AS_PASSINGPOINT
		};
		takeCursorPosAsDrp=AddDropList(0,FSKEY_NULL,"Cursor As:",3,cursorAsMsg,5,16,16,YSFALSE);
		takeCursorPosAsDrp->Select(0);

		const wchar_t * const whichHalfMsg[3]=
		{
			FSGUI_DLG_EQUILATERAL_WHOLE,
			FSGUI_DLG_EQUILATERAL_FIRSTHALF,
			FSGUI_DLG_EQUILATERAL_LASTHALF
		};
		whichHalfDrp=AddDropList(0,FSKEY_NULL,"",3,whichHalfMsg,5,16,16,YSFALSE);
		whichHalfDrp->Select(0);

		AddStaticText(0,FSKEY_NULL,FSGUI_DLG_EQUILATERAL_1STVERTEX,YSTRUE);
		const wchar_t * const firstVertexIsMsg[3]=
		{
			FSGUI_DLG_EQUILATERAL_BORDER,
			FSGUI_DLG_EQUILATERAL_CENTER
		};
		firstVertexIsDrp=AddDropList(0,FSKEY_NULL,FSGUI_DLG_EQUILATERAL_1STVERTEX,2,firstVertexIsMsg,5,16,16,YSFALSE);
		firstVertexIsDrp->Select(0);

		include2ndVertexBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_DLG_EQUILATERAL_INCLUDE2ND,YSTRUE);
		include2ndVertexBtn->SetCheck(YSTRUE);
		includeCursorPosBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_DLG_EQUILATERAL_INCLUDECURSOR,YSTRUE);
		includeCursorPosBtn->SetCheck(YSFALSE);

		numDivTxt=AddTextBox(0,FSKEY_NULL,FSGUI_DLG_COMMON_DIVCOUNT,L"16",5,YSTRUE);
		numDivTxt->SetTextType(FSGUI_INTEGER);

		okBtn=AddTextButton(0,FSKEY_ENTER,FSGUI_PUSHBUTTON,FSGUI_COMMON_OK,YSTRUE);
		cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);

		RemakeDrawingBuffer();

		SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
		SetBackgroundAlpha(0.3);
		Fit();
	}

	return YSOK;
}

void PolyCreEquilateralDialog::RemakeDrawingBuffer(void)
{
	lineBuf.CleanUp();
	circleBuf.CleanUp();

	YsVec3 cen,uVec,vVec;
	double rad;
	if(YSOK==GetCircumCircle(cen,rad,uVec,vVec))
	{
		for(int i=0; i<360; ++i)
		{
			const double ang=(double)i*YsPi/180.0;
			const double c=cos(ang);
			const double s=sin(ang);

			circleBuf.AddVertex(cen+rad*(uVec*c+vVec*s));
		}

		YsArray <YsPair <YsVec3,YsShellVertexHandle> > plg=GetPolygon();
		for(auto vtx : plg)
		{
			lineBuf.AddVertex(vtx.a);
		}
	}
}

YSRESULT PolyCreEquilateralDialog::GetCircumCircle(YsVec3 &cen,double &rad,YsVec3 &uVec,YsVec3 &vVec) const
{
	const YsVec3 cursorPos=canvas->threeDInterface.point_pos;
	switch(takeCursorPosAsDrp->GetSelection())
	{
	case 0: // tangent
		{
			YsVec3 tri[3]=
			{
				srcVtCache[0].vtPos,
				srcVtCache[1].vtPos,
				cursorPos
			};
			YsVec3 nom;
			uVec=srcVtCache[0].vtPos-srcVtCache[1].vtPos;
			if(YSOK==YsGetAverageNormalVector(nom,3,tri) &&
			   YSOK==uVec.Normalize())
			{
				vVec=uVec^nom;
				if(0==firstVertexIsDrp->GetSelection())
				{
					cen=(srcVtCache[0].vtPos+srcVtCache[1].vtPos)/2.0;
					rad=(srcVtCache[0].vtPos-srcVtCache[1].vtPos).GetLength()/2.0;
				}
				else
				{
					cen=srcVtCache[0].vtPos;
					rad=(srcVtCache[0].vtPos-srcVtCache[1].vtPos).GetLength();
				}
				return YSOK;
			}
		}
		break;
	case 1: // normal
		{
			uVec=srcVtCache[0].vtPos-srcVtCache[1].vtPos;
			if(YSOK==uVec.Normalize())
			{
				if(0==firstVertexIsDrp->GetSelection())
				{
					cen=(srcVtCache[0].vtPos+srcVtCache[1].vtPos)/2.0;
					rad=(srcVtCache[0].vtPos-srcVtCache[1].vtPos).GetLength()/2.0;
				}
				else
				{
					cen=srcVtCache[0].vtPos;
					rad=(srcVtCache[0].vtPos-srcVtCache[1].vtPos).GetLength();
				}

				vVec=(cursorPos-srcVtCache[0].vtPos)^uVec;
				if(YSOK==vVec.Normalize())
				{
					return YSOK;
				}
			}
		}
		break;
	case 2: // Passing Point
		{
			if(YSOK==YsGetCircumSphereOfTriangle(cen,rad,srcVtCache[0].vtPos,srcVtCache[1].vtPos,cursorPos))
			{
				uVec=srcVtCache[0].vtPos-cen;
				vVec=srcVtCache[1].vtPos-cen;
				if(YSOK==uVec.Normalize() && YSOK==vVec.Normalize())
				{
					YsVec3 nom=uVec^vVec;
					if(YSOK==nom.Normalize())
					{
						vVec=nom^uVec;
						return YSOK;
					}
				}
			}
		}
		break;
	}
	return YSERR;
}

YsArray <YsPair <YsVec3,YsShellVertexHandle> > PolyCreEquilateralDialog::GetPolygon(void) const
{
	auto shl=canvas->Slhd();

	YsArray <YsPair <YsVec3,YsShellVertexHandle> > ary;

	YsVec3 cen,uVec,vVec;
	double rad;
	if(YSOK==PolyCreEquilateralDialog::GetCircumCircle(cen,rad,uVec,vVec))
	{
		const YSBOOL include1stVertex=(0==firstVertexIsDrp->GetSelection() ? YSTRUE : YSFALSE);
		const YSBOOL include2ndVertex=include2ndVertexBtn->GetCheck();
		const YSBOOL includeCursorPos=(1!=takeCursorPosAsDrp->GetSelection() ? includeCursorPosBtn->GetCheck() : YSFALSE);

		int nDiv=numDivTxt->GetInteger();
		if(3>nDiv)
		{
			nDiv=3;
		}

		if(0==whichHalfDrp->GetSelection() || 1==whichHalfDrp->GetSelection())
		{
			if(YSTRUE==include1stVertex)
			{
				ary.Increment();
				ary.Last().a=srcVtCache[0].vtPos;
				ary.Last().b=(NULL!=shl ? shl->FindVertex(srcVtCache[0].vtKey) : NULL);
			}

			YsArray <double> srcVtAngle(nSrcVt,NULL);
			const YsVec3 nom=uVec^vVec;
			for(int src=0; src<nSrcVt; ++src)
			{
				const YsVec3 tstVec=YsUnitVector(srcVtCache[src].vtPos-cen);
				const YsVec3 tstNom=uVec^tstVec;
				if(0.0<tstNom*nom)
				{
					srcVtAngle[src]=acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
				else
				{
					srcVtAngle[src]=YsPi*2.0-acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
			}

			double cursorAngle=YsInfinity;
			{
				const YsVec3 cursorPos=canvas->threeDInterface.point_pos;
				const YsVec3 tstVec=YsUnitVector(cursorPos-cen);
				const YsVec3 tstNom=uVec^tstVec;
				if(0.0<tstNom*nom)
				{
					cursorAngle=acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
				else
				{
					cursorAngle=YsPi*2.0-acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
			}

			double prevAngle=0;
			for(auto i=ary.GetN(); i<=nDiv; ++i)
			{
				const double angle=YsPi*2.0*(double)i/(double)nDiv;

				if((1==whichHalfDrp->GetSelection() || YSTRUE==include2ndVertex) &&
				   prevAngle<srcVtAngle[1] && srcVtAngle[1]<=angle+YsTolerance)
				{
					ary.Increment();
					ary.Last().a=srcVtCache[1].vtPos;
					ary.Last().b=(NULL!=shl ? shl->FindVertex(srcVtCache[1].vtKey) : NULL);

					const YSBOOL exact=YsEqual(angle,srcVtAngle[1]);

					srcVtAngle[1]=YsInfinity;  // Prevent it from being used again.

					if(1==whichHalfDrp->GetSelection())
					{
						i=nDiv+1; // Terminate
					}

					if(YSTRUE==exact)
					{
						prevAngle=angle;
						continue;
					}
				}

				if(YSTRUE==includeCursorPos &&
				   prevAngle<cursorAngle && cursorAngle<=angle+YsTolerance)
				{
					const double c=cos(cursorAngle);
					const double s=sin(cursorAngle);

					ary.Increment();
					ary.Last().a=cen+rad*(uVec*c+vVec*s);
					if(ary.Last().a==srcVtCache[2].vtPos && nullptr!=shl && nullptr!=shl->FindVertex(srcVtCache[2].vtKey))
					{
						ary.Last().b=shl->FindVertex(srcVtCache[2].vtKey);
					}
					else
					{
						ary.Last().b=NULL;
					}

					const YSBOOL exact=YsEqual(angle,cursorAngle);

					cursorAngle=YsInfinity;  // Prevent it from being used again.

					if(YSTRUE==exact)
					{
						prevAngle=angle;
						continue;
					}
				}

				if(i<nDiv)
				{
					const double c=cos(angle);
					const double s=sin(angle);
					ary.Increment();
					ary.Last().a=cen+rad*(uVec*c+vVec*s);
					ary.Last().b=NULL;
				}

				prevAngle=angle;
			}
		}
		else // if(2==whichHalfDrp->GetSelection())
		{
			YsArray <double> srcVtAngle(nSrcVt,NULL);
			const YsVec3 nom=uVec^vVec;
			for(int src=0; src<nSrcVt; ++src)
			{
				const YsVec3 tstVec=YsUnitVector(srcVtCache[src].vtPos-cen);
				const YsVec3 tstNom=uVec^tstVec;
				if(0.0<tstNom*nom)
				{
					srcVtAngle[src]=acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
				else
				{
					srcVtAngle[src]=YsPi*2.0-acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
			}

			double cursorAngle=YsInfinity;
			{
				const YsVec3 cursorPos=canvas->threeDInterface.point_pos;
				const YsVec3 tstVec=YsUnitVector(cursorPos-cen);
				const YsVec3 tstNom=uVec^tstVec;
				if(0.0<tstNom*nom)
				{
					cursorAngle=acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
				else
				{
					cursorAngle=YsPi*2.0-acos(YsBound(uVec*tstVec,-1.0,1.0));
				}
			}

			YSBOOL wait=YSTRUE;
			double prevAngle=0;
			for(int i=0; i<=nDiv; ++i)
			{
				const double angle=YsPi*2.0*(double)i/(double)nDiv;

				if(prevAngle<srcVtAngle[1] && srcVtAngle[1]<=angle+YsTolerance)
				{
					ary.Increment();
					ary.Last().a=srcVtCache[1].vtPos;
					ary.Last().b=(NULL!=shl ? shl->FindVertex(srcVtCache[1].vtKey) : NULL);

					const YSBOOL exact=YsEqual(angle,srcVtAngle[1]);

					srcVtAngle[1]=YsInfinity;  // Prevent it from being used again.

					wait=YSFALSE;

					if(YSTRUE==exact)
					{
						prevAngle=angle;
						continue;
					}
				}

				if(YSTRUE!=wait &&
				   YSTRUE==includeCursorPos &&
				   prevAngle<cursorAngle && cursorAngle<=angle+YsTolerance)
				{
					const double c=cos(cursorAngle);
					const double s=sin(cursorAngle);

					ary.Increment();
					ary.Last().a=cen+rad*(uVec*c+vVec*s);
					if(ary.Last().a==srcVtCache[2].vtPos && nullptr!=shl && nullptr!=shl->FindVertex(srcVtCache[2].vtKey))
					{
						ary.Last().b=shl->FindVertex(srcVtCache[2].vtKey);
					}
					else
					{
						ary.Last().b=NULL;
					}

					const YSBOOL exact=YsEqual(angle,cursorAngle);

					cursorAngle=YsInfinity;  // Prevent it from being used again.

					if(YSTRUE==exact)
					{
						prevAngle=angle;
						continue;
					}
				}

				if(YSTRUE!=wait)
				{
					const double c=cos(angle);
					const double s=sin(angle);
					ary.Increment();
					ary.Last().a=cen+rad*(uVec*c+vVec*s);
					ary.Last().b=NULL;
				}

				prevAngle=angle;
			}
		}
	}
	return ary;
}

void PolyCreEquilateralDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		canvas->Edit_CreateEquilateral_SpaceKeyCallBack();
	}
	else if(btn==cancelBtn)
	{
		canvas->Edit_ClearUIMode();
	}
	else if(btn==includeCursorPosBtn || btn==include2ndVertexBtn)
	{
		RemakeDrawingBuffer();
		canvas->SetNeedRedraw(YSTRUE);
	}
}

void PolyCreEquilateralDialog::OnDropListSelChange(FsGuiDropList *drp,int)
{
	if(drp==takeCursorPosAsDrp)
	{
		RemakeDrawingBuffer();
		if(1==drp->GetSelection())
		{
			includeCursorPosBtn->Disable();
		}
		else
		{
			includeCursorPosBtn->Enable();
		}
	}
	else if(drp==whichHalfDrp)
	{
		RemakeDrawingBuffer();
	}
}

void PolyCreEquilateralDialog::OnTextBoxChange(FsGuiTextBox *txt)
{
	if(txt==numDivTxt)
	{
		RemakeDrawingBuffer();
	}
}

/*static*/ void GeblGuiEditorBase::Edit_CreateEquilateral(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL==canvas.Slhd())
	{
		return;
	}

	YsShellExtEdit &shl=*(canvas.Slhd());

	auto selVtHd=shl.GetSelectedVertex();

	if(2<=selVtHd.GetN())
	{
		if(YSOK==canvas.equilateralDlg->Make())
		{
			// equilateralDlg must cache selected vertex positions and keys.

			canvas.Edit_ClearUIMode();

			canvas.AddDialog(canvas.equilateralDlg);
			canvas.ArrangeDialog();

			canvas.UIDrawCallBack3D=Edit_CreateEquilateral_DrawCallBack3D;
			canvas.SpaceKeyCallBack=Edit_CreateEquilateral_SpaceKeyCallBack;

			canvas.equilateralDlg->nSrcVt=0;
			for(YSSIZE_T vtIdx=0; vtIdx<selVtHd.GetN() && vtIdx<3; ++vtIdx)
			{
				canvas.equilateralDlg->srcVtCache[canvas.equilateralDlg->nSrcVt].vtKey=shl.GetSearchKey(selVtHd[vtIdx]);
				canvas.equilateralDlg->srcVtCache[canvas.equilateralDlg->nSrcVt].vtPos=shl.GetVertexPosition(selVtHd[vtIdx]);
				++canvas.equilateralDlg->nSrcVt;
			}

			YsVec3 cursorPos;
			if(3<=selVtHd.GetN())
			{
				cursorPos=shl.GetVertexPosition(selVtHd[2]);
			}
			else
			{
				cursorPos=(shl.GetVertexPosition(selVtHd[0])+shl.GetVertexPosition(selVtHd[1]))/2.0;
			}
			canvas.threeDInterface.BeginInputPoint2(cursorPos);
			canvas.threeDInterface.SetCallBack(Edit_CreateEquilateral_3DInterfaceCallBack,&canvas);

			canvas.equilateralDlg->RemakeDrawingBuffer();

			canvas.undoRedoRequireToClearUIMode=YSFALSE;
			canvas.deletionRequireToClearUIMode=YSFALSE;

			canvas.SetNeedRedraw(YSTRUE);
		}
	}
	else
	{
		canvas.MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_NEEDSELECTATLEAST2VERTEX);
	}
}

/*static*/ void GeblGuiEditorBase::Edit_CreateEquilateral_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,0.5f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINE_LOOP,(int)canvas.equilateralDlg->circleBuf.GetNumVertex(),canvas.equilateralDlg->circleBuf);

	const GLfloat plgCol[4]={0,0,1.0f,.5f};
	YsGLSLSet3DRendererUniformColorfv(renderer,plgCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINE_LOOP,(int)canvas.equilateralDlg->lineBuf.GetNumVertex(),canvas.equilateralDlg->lineBuf);

	YsGLSLEndUse3DRenderer(renderer);
}

/*static*/ void GeblGuiEditorBase::Edit_CreateEquilateral_3DInterfaceCallBack(void *appPtr,class FsGui3DInterface &)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL==canvas.Slhd())
	{
		return;
	}

	canvas.equilateralDlg->RemakeDrawingBuffer();
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_CreateEquilateral_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_CreateEquilateral_SpaceKeyCallBack();
}

YSRESULT GeblGuiEditorBase::Edit_CreateEquilateral_SpaceKeyCallBack(void)
{
	if(NULL==Slhd())
	{
		return YSERR;
	}
	YsShellExtEdit &shl=*Slhd();

	auto plg=equilateralDlg->GetPolygon();
	if(3<=plg.GetN())
	{
		YsShellExtEdit::StopIncUndo incUndo(&shl);

		YsArray <YsShellVertexHandle> plVtHd;
		plVtHd.Set(plg.GetN(),NULL);
		for(auto idx : plg.AllIndex())
		{
			if(NULL!=plg[idx].b)
			{
				plVtHd[idx]=plg[idx].b;
			}
			else
			{
				plVtHd[idx]=shl.AddVertex(plg[idx].a);
			}
		}

		YsShellPolygonHandle plHd=shl.AddPolygon(plVtHd);
		shl.SetPolygonColor(plHd,colorPaletteDlg->GetColor());
		shl.SelectVertex(0,NULL);

		Edit_ClearUIMode();

		needRemakeDrawingBuffer|=NEED_REMAKE_DRAWING_POLYGON|NEED_REMAKE_DRAWING_VERTEX|NEED_REMAKE_DRAWING_SELECTED_VERTEX;
		SetNeedRedraw(YSTRUE);
	}
	return YSERR;
}



////////////////////////////////////////////////////////////



PolyCreSolidOfRevolutionDialog::PolyCreSolidOfRevolutionDialog()
{
	canvas=NULL;
	okBtn=NULL;
	cancelBtn=NULL;
}

PolyCreSolidOfRevolutionDialog::~PolyCreSolidOfRevolutionDialog()
{
}

/*static*/ PolyCreSolidOfRevolutionDialog *PolyCreSolidOfRevolutionDialog::Create(class GeblGuiEditorBase *canvas)
{
	PolyCreSolidOfRevolutionDialog *dlg=new PolyCreSolidOfRevolutionDialog;
	dlg->canvas=canvas;
	return dlg;
}

/*static*/ void PolyCreSolidOfRevolutionDialog::Delete(PolyCreSolidOfRevolutionDialog *ptr)
{
	delete ptr;
}

YSRESULT PolyCreSolidOfRevolutionDialog::Make(void)
{
	if(NULL==okBtn)
	{
		FsGuiDialog::Initialize();

		divCountBtn=AddTextButton(0,FSKEY_NULL,FSGUI_RADIOBUTTON,FSGUI_DLG_COMMON_DIVCOUNT,YSTRUE);
		stepAngleBtn=AddTextButton(0,FSKEY_NULL,FSGUI_RADIOBUTTON,FSGUI_DLG_SOLREV_STEPANGLE,YSFALSE);
		FsGuiButton *divCountOrStepAngle[2]=
		{
			divCountBtn,
			stepAngleBtn
		};
		SetRadioButtonGroup(2,divCountOrStepAngle);
		stepAngleBtn->SetCheck(YSTRUE);

		divCountTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,FSGUI_DLG_COMMON_DIVCOUNT,4,YSTRUE);
		divCountTxt->SetInteger(16);
		divCountTxt->Disable();

		stepAngleTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,FSGUI_DLG_SOLREV_STEPANGLE,4,YSFALSE);
		stepAngleTxt->SetRealNumber(22.5,4);


		closedBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_DLG_SOLREV_CLOSED,YSTRUE);
		closedBtn->SetCheck(YSTRUE);

		deleteSourcePolygonBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_DLG_SWEEP_DELSRCPLG,YSTRUE);
		deleteSourcePolygonBtn->SetCheck(YSTRUE);
		deleteSourcePolygonBtn->Disable();

		reverseBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_COMMON_REVERSE,YSTRUE);

		fanAngleTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,FSGUI_DLG_SOLREV_ANGLE,4,YSFALSE);
		fanAngleTxt->SetInteger(360);
		fanAngleTxt->Disable();

		displacementTxt=AddTextBox(0,FSKEY_NULL,FsGuiTextBox::HORIZONTAL,FSGUI_DLG_SOLREV_DISPLACEMENT,4,YSFALSE);
		displacementTxt->SetInteger(0);
		displacementTxt->Disable();

		smoothnessBtn[0]=AddTextButton(0,FSKEY_NULL,FSGUI_RADIOBUTTON,FSGUI_COMMON_G0CONTINUOUS,YSTRUE);
		smoothnessBtn[1]=AddTextButton(0,FSKEY_NULL,FSGUI_RADIOBUTTON,FSGUI_COMMON_G1CONTINUOUS,YSFALSE);
		smoothnessBtn[2]=AddTextButton(0,FSKEY_NULL,FSGUI_RADIOBUTTON,FSGUI_COMMON_G2CONTINUOUS,YSFALSE);
		SetRadioButtonGroup(3,smoothnessBtn);

		smoothnessBtn[0]->SetCheck(YSTRUE);

		smoothnessBtn[0]->Disable();
		smoothnessBtn[1]->Disable();
		smoothnessBtn[2]->Disable();


		okBtn=AddTextButton(0,FSKEY_ENTER,FSGUI_PUSHBUTTON,FSGUI_COMMON_OK,YSTRUE);
		cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);

		SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
		SetBackgroundAlpha(0.3);
		Fit();
	}

	return YSOK;
}

void PolyCreSolidOfRevolutionDialog::RemakeSrcVtKeyToPathPosHash(const YsShellExt &shl)
{
	const YsVec3 axisPnt[2]=
	{
		shl.GetVertexPosition(axisVtHd[0]),
		shl.GetVertexPosition(axisVtHd[1])
	};

	const YsVec3 axisVec=YsUnitVector(axisPnt[1]-axisPnt[0]);

	const int nDiv=GetDivisionCount();

	srcVtKeyToPathPosHash.CleanUp();



	const YSBOOL closed=closedBtn->GetCheck();
	const double displacement=(YSTRUE==closed ? 0.0 : displacementTxt->GetRealNumber());
	const double dir=(YSTRUE==reverseBtn->GetCheck() ? -1.0 : 1.0);

	for(auto vtHd : allVtHd)
	{
		const double fanAngleDeg=(YSTRUE==closed ? 360.0 : fanAngleTxt->GetRealNumber());
		const double fanAngle=YsDegToRad(fanAngleDeg);

		const int lastDiv=(closed==YSTRUE ? nDiv-1 : nDiv);

		YsArray <YsVec3> path;

		for(YSSIZE_T div=0; div<=lastDiv; ++div)
		{
			const double t=dir*(double)div/(double)nDiv;

			const double angle=fanAngle*t;

			YsRotation rot(axisVec,angle);
			YsVec3 pos=shl.GetVertexPosition(vtHd);

			pos-=axisPnt[0];
			rot.RotatePositive(pos,pos);
			pos+=axisPnt[0];

			if(YSTRUE!=closed)
			{
				double disp=t;
				if(YSTRUE==smoothnessBtn[1]->GetCheck())
				{
					disp=YsG1Continuous(t);
				}
				else if(YSTRUE==smoothnessBtn[2]->GetCheck())
				{
					disp=YsG2Continuous(t);
				}
				YsVec3 offset=axisVec*displacement*disp;
				pos+=offset;
			}

			path.Append(pos);
		}

		if(YSTRUE==closed)
		{
			path.Append(shl.GetVertexPosition(vtHd));
		}

		srcVtKeyToPathPosHash.AddElement(shl.GetSearchKey(vtHd),path);
	}
}

void PolyCreSolidOfRevolutionDialog::RemakeDrawingBuffer(void)
{
	// As required
	RemakeSrcVtKeyToPathPosHash(*shl);
	lineBuf.CleanUp();

	for(auto &srcBoundary : srcBoundaryArray)
	{
		YsArray <YsArray <YsVec3> > matrix;

		for(auto vtHd : srcBoundary)
		{
			YsArray <YsVec3> path;
			if(YSOK==srcVtKeyToPathPosHash.FindElement(path,shl->GetSearchKey(vtHd)))
			{
				YsVec3 prevPos=shl->GetVertexPosition(vtHd);
				for(auto &pos : path)
				{
					lineBuf.AddVertex(prevPos);
					lineBuf.AddVertex(pos);
					prevPos=pos;
				}
			}
			matrix.Increment();
			matrix.Last().MoveFrom(path);
		}

		for(YSSIZE_T idx=0; idx<matrix[0].GetN(); ++idx)
		{
			for(YSSIZE_T lattitude=0; lattitude<matrix.GetN()-1; ++lattitude)
			{
				lineBuf.AddVertex(matrix[lattitude][idx]);
				lineBuf.AddVertex(matrix[lattitude+1][idx]);
			}
		}
	}
}

void PolyCreSolidOfRevolutionDialog::CleanUp(const YsShellExt &shl)
{
	srcPlHdArray.CleanUp();
	srcCeHdArray.CleanUp();
	allVtHd.SetShell(shl.Conv());
	allVtHd.CleanUp();
	srcBoundaryArray.CleanUp();
	axisVtHd[0]=NULL;
	axisVtHd[1]=NULL;
	this->shl=&shl;

	axisSelVtHd[0]=NULL;
	axisSelVtHd[1]=NULL;
	axisSelCeHd=NULL;

	srcVtKeyToPathPosHash.CleanUp();
	lineBuf.CleanUp();
}

int PolyCreSolidOfRevolutionDialog::GetDivisionCount(void) const
{
	int n=0;
	if(YSTRUE==divCountBtn->GetCheck())
	{
		n=divCountTxt->GetInteger();
	}
	else if(YSTRUE==stepAngleBtn->GetCheck())
	{
		const double stepAngle=stepAngleTxt->GetRealNumber();
		if(YsTolerance<stepAngle)
		{
			double totalAngle;
			if(YSTRUE==CloseLoop())
			{
				totalAngle=360.0;
			}
			else
			{
				totalAngle=fanAngleTxt->GetRealNumber();
			}
			n=(int)(0.01+totalAngle/stepAngle);
		}
	}
	if(YSTRUE==CloseLoop() && 3>n)
	{
		n=3;
	}
	else if(YSTRUE!=CloseLoop() && 2>n)
	{
		n=2;
	}

	return n;
}

YSBOOL PolyCreSolidOfRevolutionDialog::CloseLoop(void) const
{
	return closedBtn->GetCheck();
}

YSBOOL PolyCreSolidOfRevolutionDialog::DeleteSourcePolygon(void) const
{
	return deleteSourcePolygonBtn->GetCheck();
}

void PolyCreSolidOfRevolutionDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		canvas->Edit_CreateSolidOfRevolution_SpaceKeyCallBack();
	}
	else if(btn==cancelBtn)
	{
		canvas->Edit_ClearUIMode();
	}
	else if(btn==reverseBtn || btn==smoothnessBtn[0] || btn==smoothnessBtn[1] || btn==smoothnessBtn[2])
	{
		RemakeDrawingBuffer();
	}
	else if(btn==divCountBtn || btn==stepAngleBtn)
	{
		if(YSTRUE==divCountBtn->GetCheck())
		{
			divCountTxt->Enable();
			stepAngleTxt->Disable();
		}
		else
		{
			divCountTxt->Disable();
			stepAngleTxt->Enable();
		}
		RemakeDrawingBuffer();
	}
	else if(btn==closedBtn)
	{
		YSBOOL checked=closedBtn->GetCheck();
		YsFlip(checked);
		fanAngleTxt->SetEnabled(checked);
		displacementTxt->SetEnabled(checked);

		smoothnessBtn[0]->SetEnabled(checked);
		smoothnessBtn[1]->SetEnabled(checked);
		smoothnessBtn[2]->SetEnabled(checked);

		if(YSTRUE==closedBtn->GetCheck())
		{
			deleteSourcePolygonBtn->SetCheck(YSTRUE);
			deleteSourcePolygonBtn->Disable();
		}
		else
		{
			deleteSourcePolygonBtn->SetCheck(YSFALSE);
			deleteSourcePolygonBtn->Enable();
		}

		RemakeDrawingBuffer();
	}
}

void PolyCreSolidOfRevolutionDialog::OnDropListSelChange(FsGuiDropList *,int)
{
}

void PolyCreSolidOfRevolutionDialog::OnTextBoxChange(FsGuiTextBox *)
{
	RemakeDrawingBuffer();
}

void PolyCreSolidOfRevolutionDialog::OnSliderPositionChange(FsGuiSlider *,const double &,const double &)
{
}

/*static*/ void GeblGuiEditorBase::Edit_CreateSolidOfRevolution(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL==canvas.Slhd())
	{
		return;
	}

	YsShellExtEdit &shl=*(canvas.Slhd());

	canvas.solidOfRevolutionDlg->CleanUp(shl.Conv());

	auto selPlHd=shl.GetSelectedPolygon();
	auto selCeHd=shl.GetSelectedConstEdge();
	auto selVtHd=shl.GetSelectedVertex();

	YsArray <YsArray <YsShellVertexHandle> > srcBoundaryArray;
	YsShellVertexHandle axisVtHd[2];

	YSRESULT selectionError=YSOK;
	if(0<selPlHd.GetN())
	{
		YsShellExt_BoundaryInfo bdryInfo;
		bdryInfo.MakeInfo(shl.Conv(),selPlHd);
		bdryInfo.CacheContour(shl.Conv());
		srcBoundaryArray=bdryInfo.GetContourAll();
		canvas.solidOfRevolutionDlg->srcPlHdArray=selPlHd;
		canvas.solidOfRevolutionDlg->srcCeHdArray.CleanUp();
		selPlHd.CleanUp();
	}
	else if(0<selCeHd.GetN())
	{
		const YSSIZE_T last=(2<=selVtHd.GetN() ? selCeHd.GetN() : selCeHd.GetN()-1);

		canvas.solidOfRevolutionDlg->srcPlHdArray.CleanUp();
		canvas.solidOfRevolutionDlg->srcCeHdArray.CleanUp();

		for(YSSIZE_T idx=last-1; 0<=idx; --idx)
		{
			YsArray <YsShellVertexHandle> ceVtHd;
			YSBOOL isLoop;

			canvas.solidOfRevolutionDlg->srcCeHdArray.Append(selCeHd[idx]);

			shl.GetConstEdge(ceVtHd,isLoop,selCeHd[idx]);
			if(YSTRUE==isLoop)
			{
				ceVtHd.Append(ceVtHd[0]);
			}
			srcBoundaryArray.Increment();
			srcBoundaryArray.Last().MoveFrom(ceVtHd);
			selCeHd.Delete(idx);
		}
	}
	else
	{
		selectionError=YSERR;
	}

	if(0<selCeHd.GetN())
	{
		YsArray <YsShellVertexHandle> ceVtHd;
		YSBOOL isLoop;
		shl.GetConstEdge(ceVtHd,isLoop,selCeHd[0]);
		if(2<=ceVtHd.GetN())
		{
			axisVtHd[0]=ceVtHd[0];
			axisVtHd[1]=ceVtHd[1];

			canvas.solidOfRevolutionDlg->axisSelCeHd=selCeHd[0];
			canvas.solidOfRevolutionDlg->axisSelVtHd[0]=NULL;
			canvas.solidOfRevolutionDlg->axisSelVtHd[1]=NULL;
		}
		else
		{
			selectionError=YSERR;
		}
	}
	else if(2<=selVtHd.GetN())
	{
		axisVtHd[0]=selVtHd[0];
		axisVtHd[1]=selVtHd[1];

		canvas.solidOfRevolutionDlg->axisSelCeHd=NULL;
		canvas.solidOfRevolutionDlg->axisSelVtHd[0]=selVtHd[0];
		canvas.solidOfRevolutionDlg->axisSelVtHd[1]=selVtHd[0];
	}
	else
	{
		selectionError=YSERR;
	}

	if(0<srcBoundaryArray.GetN() && YSOK==selectionError)
	{
		if(YSOK==canvas.solidOfRevolutionDlg->Make())
		{
			canvas.solidOfRevolutionDlg->allVtHd.SetShell(shl.Conv());
			for(auto plHd : canvas.solidOfRevolutionDlg->srcPlHdArray)
			{
				auto plVtHd=shl.GetPolygonVertex(plHd);
				canvas.solidOfRevolutionDlg->allVtHd.AddVertex(plVtHd);
			}
			for(auto ceHd : canvas.solidOfRevolutionDlg->srcCeHdArray)
			{
				YSBOOL isLoop;
				YsArray <YsShellVertexHandle> ceVtHd;
				shl.GetConstEdge(ceVtHd,isLoop,ceHd);
				canvas.solidOfRevolutionDlg->allVtHd.AddVertex(ceVtHd);
			}

			canvas.solidOfRevolutionDlg->srcBoundaryArray=srcBoundaryArray;
			canvas.solidOfRevolutionDlg->axisVtHd[0]=axisVtHd[0];
			canvas.solidOfRevolutionDlg->axisVtHd[1]=axisVtHd[1];
			canvas.solidOfRevolutionDlg->shl=&(const YsShellExt &)shl.Conv();

			canvas.Edit_ClearUIMode();

			canvas.AddDialog(canvas.solidOfRevolutionDlg);
			canvas.ArrangeDialog();

			canvas.UIDrawCallBack3D=Edit_CreateSolidOfRevolution_DrawCallBack3D;
			canvas.SpaceKeyCallBack=Edit_CreateSolidOfRevolution_SpaceKeyCallBack;

			// Set up 3D interface if necessary
			canvas.threeDInterface.SetCallBack(Edit_CreateSolidOfRevolution_3DInterfaceCallBack,&canvas);

			canvas.undoRedoRequireToClearUIMode=YSTRUE;
			canvas.deletionRequireToClearUIMode=YSTRUE;

			canvas.solidOfRevolutionDlg->RemakeDrawingBuffer();

			canvas.SetNeedRedraw(YSTRUE);
		}
	}
	else
	{
		canvas.MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_SOLREV_SELECTION);
	}
}

/*static*/ void GeblGuiEditorBase::Edit_CreateSolidOfRevolution_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	if(NULL!=canvas.Slhd())
	{
		struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
		YsGLSLUse3DRenderer(renderer);

		const GLfloat edgeCol[4]={0,1.0f,0,0.5f};
		YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
		YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINES,(int)canvas.solidOfRevolutionDlg->lineBuf.GetNumVertex(),canvas.solidOfRevolutionDlg->lineBuf);

		YsGLSLEndUse3DRenderer(renderer);
		return;
	}
}

/*static*/ void GeblGuiEditorBase::Edit_CreateSolidOfRevolution_3DInterfaceCallBack(void *appPtr,class FsGui3DInterface &)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL==canvas.Slhd())
	{
		return;
	}
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_CreateSolidOfRevolution_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_CreateSolidOfRevolution_SpaceKeyCallBack();
}

YSRESULT GeblGuiEditorBase::Edit_CreateSolidOfRevolution_SpaceKeyCallBack(void)
{
	if(NULL==Slhd())
	{
		return YSERR;
	}
	YsShellExtEdit &shl=*Slhd();

	{
		YsShellExtEdit::StopIncUndo incUndo(shl);
		YsHashTable <YsArray <YsShellVertexHandle> > srcVtKeyToPathVtHdHash;

		for(auto srcBoundary : solidOfRevolutionDlg->srcBoundaryArray)
		{
			YsArray <YsArray <YsShellVertexHandle> > vtHdMatrix;
			for(auto vtHd : srcBoundary)
			{
				YsArray <YsShellVertexHandle> vtHdArray;
				if(YSOK==srcVtKeyToPathVtHdHash.FindElement(vtHdArray,shl.GetSearchKey(vtHd)))
				{
					continue;  // Already created.
				}

				YsArray <YsVec3> path;
				if(YSOK!=solidOfRevolutionDlg->srcVtKeyToPathPosHash.FindElement(path,shl.GetSearchKey(vtHd)))
				{
					return YSERR;
				}

				vtHdArray.Append(vtHd);
				for(YSSIZE_T idx=1; idx<path.GetN()-1; ++idx)
				{
					auto newVtHd=shl.AddVertex(path[idx]);
					vtHdArray.Append(newVtHd);
				}

				if(YSTRUE==solidOfRevolutionDlg->CloseLoop())
				{
					vtHdArray.Append(vtHd);
				}
				else
				{
					vtHdArray.Append(shl.AddVertex(path.Last()));
				}

				srcVtKeyToPathVtHdHash.AddElement(shl.GetSearchKey(vtHd),vtHdArray);
			}
		}

		for(auto srcBoundary : solidOfRevolutionDlg->srcBoundaryArray)
		{
			YsArray <YsArray <YsShellVertexHandle> > vtHdMatrix;
			for(auto vtHd : srcBoundary)
			{
				YsArray <YsShellVertexHandle> path;
				if(YSOK!=srcVtKeyToPathVtHdHash.FindElement(path,shl.GetSearchKey(vtHd)))
				{
					return YSERR;
				}

				vtHdMatrix.Increment();
				vtHdMatrix.Last().MoveFrom(path);
			}

			for(YSSIZE_T i=0; i<vtHdMatrix.GetN()-1; ++i)
			{
				for(YSSIZE_T j=0; j<vtHdMatrix[i].GetN()-1; ++j)
				{
					YsShellVertexHandle quadVtHd[4]=
					{
						vtHdMatrix[i  ][j  ],
						vtHdMatrix[i+1][j  ],
						vtHdMatrix[i+1][j+1],
						vtHdMatrix[i  ][j+1],
					};
					shl.AddPolygon(4,quadVtHd);
				}
			}
		}

		YsArray <YsShellPolygonHandle> newSelPlHd;
		YsArray <YsShellExt::ConstEdgeHandle> newSelCeHd;

		if(YSTRUE!=solidOfRevolutionDlg->CloseLoop())
		{
			// Cap the open end.
			for(auto vtHd : solidOfRevolutionDlg->allVtHd)
			{
				YsArray <YsShellVertexHandle> pathVtHd;
				YsArray <YsVec3> pathPos;
				if(YSOK!=srcVtKeyToPathVtHdHash.FindElement(pathVtHd,shl.GetSearchKey(vtHd)) &&
				   YSOK==solidOfRevolutionDlg->srcVtKeyToPathPosHash.FindElement(pathPos,shl.GetSearchKey(vtHd)))
				{
					pathVtHd.MakeUnitLength(shl.AddVertex(pathPos.Last()));
					srcVtKeyToPathVtHdHash.AddElement(shl.GetSearchKey(vtHd),pathVtHd);
				}
			}

			for(auto plHd : solidOfRevolutionDlg->srcPlHdArray)
			{
				auto plVtHd=shl.GetPolygonVertex(plHd);
				YSRESULT res=YSOK;
				for(auto &vtHd : plVtHd)
				{
					YsArray <YsShellVertexHandle> pathVtHd;
					if(YSOK!=srcVtKeyToPathVtHdHash.FindElement(pathVtHd,shl.GetSearchKey(vtHd)))
					{
						res=YSERR;
						break;
					}
					vtHd=pathVtHd.Last();
				}
				if(YSOK==res)
				{
					auto newPlHd=shl.AddPolygon(plVtHd);
					shl.SetPolygonColor(newPlHd,colorPaletteDlg->GetColor());
					newSelPlHd.Append(newPlHd);
				}
			}
			for(auto ceHd : solidOfRevolutionDlg->srcCeHdArray)
			{
				YSBOOL isLoop;
				YsArray <YsShellVertexHandle> ceVtHd;
				shl.GetConstEdge(ceVtHd,isLoop,ceHd);
				YSRESULT res=YSOK;
				for(auto &vtHd : ceVtHd)
				{
					YsArray <YsShellVertexHandle> pathVtHd;
					if(YSOK!=srcVtKeyToPathVtHdHash.FindElement(pathVtHd,shl.GetSearchKey(vtHd)))
					{
						res=YSERR;
						break;
					}
					vtHd=pathVtHd.Last();
				}
				if(YSOK==res)
				{
					auto newCeHd=shl.AddConstEdge(ceVtHd,isLoop);
					newSelCeHd.Append(newCeHd);
				}
			}
			newSelCeHd.Append(solidOfRevolutionDlg->axisSelCeHd);
		}

		shl.SelectPolygon(newSelPlHd);
		shl.SelectConstEdge(newSelCeHd);
		if(NULL!=solidOfRevolutionDlg->axisSelVtHd[0])
		{
			shl.SelectVertex(2,solidOfRevolutionDlg->axisSelVtHd);
		}

		if(YSTRUE==solidOfRevolutionDlg->DeleteSourcePolygon())
		{
			shl.DeleteMultiPolygon(solidOfRevolutionDlg->srcPlHdArray);
		}

		needRemakeDrawingBuffer|=
		    NEED_REMAKE_DRAWING_POLYGON|
		    NEED_REMAKE_DRAWING_VERTEX|
		    NEED_REMAKE_DRAWING_CONSTEDGE|
		    NEED_REMAKE_DRAWING_SELECTED_POLYGON|
		    NEED_REMAKE_DRAWING_SELECTED_CONSTEDGE|
		    NEED_REMAKE_DRAWING_SELECTED_VERTEX;
		SetNeedRedraw(YSTRUE);

		Edit_ClearUIMode();
	}
	return YSOK;
}

////////////////////////////////////////////////////////////

class GeblGui_CreateConeFromPolygonAndVertexDialog : public FsGuiDialog
{
private:
	GeblGuiEditorBase *canvasPtr;

public:
	FsGuiDropList *srcPlgDrp;
	FsGuiButton *okBtn,*cancelBtn;

	void Make(GeblGuiEditorBase &canvas);
	void CreateCone(void);
	virtual void OnButtonClick(FsGuiButton *btn);
};

void GeblGui_CreateConeFromPolygonAndVertexDialog::Make(GeblGuiEditorBase &canvas)
{
	canvasPtr=&canvas;

	const wchar_t *srcPlgOption[]=
	{
		L"Automatic",
		L"Delete",
		L"Don't delete"
	};
	srcPlgDrp=AddDropList(0,FSKEY_NULL,"",3,srcPlgOption,5,16,16,YSFALSE);
	srcPlgDrp->Select(0);

	okBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_COMMON_OK,YSTRUE);
	cancelBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);

	SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
	SetBackgroundAlpha(0.3);
	Fit();
}

void GeblGui_CreateConeFromPolygonAndVertexDialog::CreateCone(void)
{
	auto slHdPtr=canvasPtr->Slhd();
	if(nullptr==slHdPtr)
	{
		return;
	}

	auto &shl=*slHdPtr;

	auto selVtHd=shl.GetSelectedVertex();
	auto selCeHd=shl.GetSelectedConstEdge();
	auto selPlHd=shl.GetSelectedPolygon();
	auto selFgHd=shl.GetSelectedFaceGroup();

	if(0<selVtHd.GetN() && (0<selCeHd.GetN() || 0<selPlHd.GetN() || 0<selFgHd.GetN()))
	{
		auto peakVtHd=selVtHd[0];

		YsShellPolygonStore srcPlg(shl.Conv());
		srcPlg.Add(selPlHd);
		for(auto fgHd : selFgHd)
		{
			auto fgPlHd=shl.GetFaceGroup(fgHd);
			srcPlg.Add(fgPlHd);
		}
		YsShellExt_BoundaryInfo boundaryUtil;
		boundaryUtil.MakeInfo(shl.Conv(),srcPlg);
		boundaryUtil.CacheContour(shl.Conv());

		YsShellEdgeStore edgeTable(shl.Conv());

		YsArray <YsShell::Edge> srcEdge;
		for(YSSIZE_T idx=0; idx<boundaryUtil.GetNumContour(); ++idx)
		{
			YsArray <YsShell::VertexHandle> contour;
			boundaryUtil.GetContour(contour,idx);
			for(auto edIdx : contour.AllIndex())
			{
				YsShell::Edge edge;
				edge[0]=contour[edIdx];
				edge[1]=contour.GetCyclic(edIdx+1);
				if(edge[0]!=edge[1] && YSTRUE!=edgeTable.IsIncluded(edge)) // Just in case
				{
					srcEdge.Add(edge);
					edgeTable.Add(edge);
				}
			}
		}

		for(auto ceHd : selCeHd)
		{
			YSBOOL isLoop;
			YsArray <YsShell::VertexHandle> ceVtHd;
			shl.GetConstEdge(ceVtHd,isLoop,ceHd);
			if(YSTRUE==isLoop)
			{
				ceVtHd.Add(ceVtHd[0]);
			}

			for(auto edIdx : ceVtHd.AllIndex())
			{
				YsShell::Edge edge;
				edge[0]=ceVtHd[edIdx];
				edge[1]=ceVtHd.GetCyclic(edIdx+1);
				if(edge[0]!=edge[1] && YSTRUE!=edgeTable.IsIncluded(edge)) // Just in case
				{
					srcEdge.Add(edge);
					edgeTable.Add(edge);
				}
			}
		}

		YSBOOL autoDelete=YSFALSE;
		for(auto edge : srcEdge)
		{
			if(2<=shl.GetNumPolygonUsingEdge(edge))
			{
				autoDelete=YSTRUE;
			}
		}

		YsShellExtEdit::StopIncUndo undoGuard(shl);
		for(auto edge : srcEdge)
		{
			YsShell::VertexHandle triVtHd[3]={edge[0],edge[1],peakVtHd};
			auto plHd=shl.AddPolygon(3,triVtHd);

			YsVec3 tri[3]=
			{
				shl.GetVertexPosition(triVtHd[0]),
				shl.GetVertexPosition(triVtHd[1]),
				shl.GetVertexPosition(triVtHd[2]),
			};
			auto nom=YsGetAverageNormalVector(3,tri);
			shl.SetPolygonNormal(plHd,nom);
		}

		switch(srcPlgDrp->GetSelection())
		{
		default:
		case 0:
			if(YSTRUE==autoDelete)
			{
				for(auto fgHd : selFgHd)
				{
					shl.DeleteFaceGroup(fgHd);
				}
				shl.DeleteFaceGroupPolygonMulti(selPlHd);
				for(auto plHd : srcPlg)
				{
					shl.DeletePolygon(plHd);
				}
			}
			break;
		case 1:
			for(auto fgHd : selFgHd)
			{
				shl.DeleteFaceGroup(fgHd);
			}
			for(auto plHd : srcPlg)
			{
				shl.DeletePolygon(plHd);
			}
			break;
		case 2:
			break;
		}
	}
	else
	{
		canvasPtr->MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_NEEDSELECTVTXANDPLGORFGORCE);
	}
}

/* virtual */ void GeblGui_CreateConeFromPolygonAndVertexDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		CreateCone();
		canvasPtr->needRemakeDrawingBuffer|=
		   GeblGuiEditorBase::NEED_REMAKE_DRAWING_POLYGON|
		   GeblGuiEditorBase::NEED_REMAKE_DRAWING_SELECTED_POLYGON|
		   GeblGuiEditorBase::NEED_REMAKE_DRAWING_SELECTED_FACEGROUP;
		canvasPtr->Edit_ClearUIMode();
	}
	else if(btn==cancelBtn)
	{
		canvasPtr->Edit_ClearUIMode();
	}
}
void GeblGuiEditorBase::Edit_CreateConeFromPolygonAndVertex(FsGuiPopUpMenuItem *)
{
printf("%s %d\n",__FUNCTION__,__LINE__);

	if(NULL==Slhd())
	{
		return;
	}

	YsShellExtEdit &shl=*Slhd();

	auto selVtHd=shl.GetSelectedVertex();
	auto selCeHd=shl.GetSelectedConstEdge();
	auto selPlHd=shl.GetSelectedPolygon();
	auto selFgHd=shl.GetSelectedFaceGroup();

	if(0<selVtHd.GetN() && (0<selCeHd.GetN() || 0<selPlHd.GetN() || 0<selFgHd.GetN()))
	{
		auto dlg=FsGuiDialog::CreateSelfDestructiveDialog<GeblGui_CreateConeFromPolygonAndVertexDialog>();
		dlg->Make(*this);
		AddDialog(dlg);
		ArrangeDialog();
	}
	else
	{
		MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_NEEDSELECTVTXANDPLGORFGORCE);
	}
}
