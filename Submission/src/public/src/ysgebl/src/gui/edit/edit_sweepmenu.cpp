/* ////////////////////////////////////////////////////////////

File Name: edit_sweepmenu.cpp
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
#include "edit_sweepdialog.h"
#include "../miscdlg.h"
#include <ysgl.h>

#include <ysshellextutil.h>
#include <ysshellext_sweeputil.h>
#include <ysshellext_geomcalc.h>
#include <ysshellextedit_orientationutil.h>

#include "../textresource.h"

////////////////////////////////////////////////////////////

/*static*/ PolyCreEditParallelSweepDialog *PolyCreEditParallelSweepDialog::Create(void)
{
	return new PolyCreEditParallelSweepDialog;
}

/*static*/ void PolyCreEditParallelSweepDialog::Delete(PolyCreEditParallelSweepDialog *dlg)
{
	delete dlg;
}

void PolyCreEditParallelSweepDialog::Make(class GeblGuiEditorBase *canvas)
{
	this->canvas=canvas;

	Initialize();

	okBtn=AddTextButton(0,FSKEY_SPACE,FSGUI_PUSHBUTTON,L"Sweep",YSTRUE);
	cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);

	scaleTxt=AddTextBox(0,FSKEY_NULL,FSGUI_DLG_COMMON_SCALINGFACTOR,L"1.0",5,YSTRUE);
	scaleTxt->SetTextType(FSGUI_REALNUMBER);

	deleteSourcePolygonBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_DLG_SWEEP_DELSRCPLG,YSTRUE);

	SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
	SetBackgroundAlpha(0.3);
	Fit();

}

double PolyCreEditParallelSweepDialog::GetScalingFactor(void) const
{
	return scaleTxt->GetRealNumber();
}

void PolyCreEditParallelSweepDialog::SetDeleteSourcePolygon(YSBOOL sw)
{
	deleteSourcePolygonBtn->SetCheck(sw);
}

YSBOOL PolyCreEditParallelSweepDialog::GetDeleteSourcePolygon(void) const
{
	return deleteSourcePolygonBtn->GetCheck();
}

void PolyCreEditParallelSweepDialog::OnButtonClick(FsGuiButton *btn)
{
	if(okBtn==btn)
	{
		canvas->Edit_Sweep_ParallelSweep_SpaceKeyCallBack();
	}
	else if(cancelBtn==btn)
	{
		canvas->Edit_ClearUIMode();
	}
}

////////////////////////////////////////////////////////////

/*static*/ void GeblGuiEditorBase::Edit_Sweep_ParallelSweep(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase *canvas=(GeblGuiEditorBase *)appPtr;

	YsArray <YsShellPolygonHandle> selPlHd;
	YsArray <YsShellExt::ConstEdgeHandle> selCeHd;

	if(NULL!=canvas->slHd)
	{
		canvas->slHd->GetSelectedConstEdge(selCeHd);
		canvas->slHd->GetSelectedPolygon(selPlHd);

		if(0<selCeHd.GetN() || 0<selPlHd.GetN())
		{
			YSBOOL dirSw[3]={YSTRUE,YSTRUE,YSTRUE};
			YsVec3 dir[3]={YsXVec(),YsYVec(),YsZVec()};

			YsVec3 nomSum=YsShell_CalculatePolygonAverageNormal(canvas->slHd->Conv(),selPlHd);
			if(YSOK==nomSum.Normalize())
			{
				dir[0]=nomSum;
				dir[1]=nomSum.GetArbitraryPerpendicularVector();
				dir[2]=dir[0]^dir[1];
			}

			canvas->Edit_ClearUIMode();

			canvas->sweepInfo->MakeInfo(*(const YsShellExt *)(canvas->slHd),selPlHd,selCeHd);
			canvas->sweepInfo->CacheNonDuplicateVtHdArray();

			const YsVec3 cen=canvas->sweepInfo->GetCenter(*(const YsShellExt *)(canvas->slHd));
			canvas->threeDInterface.BeginInputPoint2(cen,dirSw,dir);
			canvas->threeDInterface.SetInputPoint2UseCustomOrientation(YSTRUE);

			canvas->UIDrawCallBack3D=Edit_Sweep_ParallelSweep_DrawCallBack3D;
			canvas->SpaceKeyCallBack=Edit_Sweep_ParallelSweep_SpaceKeyCallBack;

			canvas->undoRedoRequireToClearUIMode=YSTRUE;
			canvas->deletionRequireToClearUIMode=YSTRUE;

			canvas->parallelSweepDlg->Make(canvas);
			int minUse,maxUse;
			canvas->sweepInfo->GetEdgeUsageMinMax(minUse,maxUse,*(const YsShellExt *)(canvas->slHd));
			canvas->parallelSweepDlg->SetDeleteSourcePolygon((1!=maxUse) ? YSTRUE : YSFALSE);

			canvas->AddDialog(canvas->parallelSweepDlg);
			canvas->ArrangeDialog();
		}
		else
		{
			canvas->MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_NEEDSELECTONEPLGORCONSTEDGE);
		}
	}
}

/*static*/ void GeblGuiEditorBase::Edit_Sweep_ParallelSweep_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	canvas.Edit_Sweep_ParallelSweep_DrawCallBack3D();
}

void GeblGuiEditorBase::Edit_Sweep_ParallelSweep_DrawCallBack3D(void)
{
	const double scaling=parallelSweepDlg->GetScalingFactor();
	const YsVec3 cen=sweepInfo->GetCenter(*(const YsShellExt *)slHd);
	const YsVec3 displace=threeDInterface.point_pos-threeDInterface.point_pos_org;

	YsArray <YsShellVertexHandle,16> vtHdArray;
	sweepInfo->GetVertexAll(vtHdArray);

	YsGLVertexBuffer vtxBuf;
	for(YSSIZE_T vtIdx=0; vtIdx<vtHdArray.GetN(); ++vtIdx)
	{
		YsVec3 pos;
		slHd->GetVertexPosition(pos,vtHdArray[vtIdx]);
		vtxBuf.AddVertex(pos);

		pos=cen+displace+(pos-cen)*scaling;
		vtxBuf.AddVertex(pos);
	}

	for(YSSIZE_T edIdx=0; edIdx<sweepInfo->GetNumEdgePiece(); ++edIdx)
	{
		YsShellVertexHandle edVtHd[2];
		if(YSOK==sweepInfo->GetEdge(edVtHd,edIdx))
		{
			YsVec3 edVtPos[2];
			slHd->GetVertexPosition(edVtPos[0],edVtHd[0]);
			slHd->GetVertexPosition(edVtPos[1],edVtHd[1]);

			edVtPos[0]=cen+displace+(edVtPos[0]-cen)*scaling;
			edVtPos[1]=cen+displace+(edVtPos[1]-cen)*scaling;

			vtxBuf.AddVertex(edVtPos[0]);
			vtxBuf.AddVertex(edVtPos[1]);
		}
	}

	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,1.0f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINES,(int)vtxBuf.GetNumVertex(),vtxBuf);

	YsGLSLEndUse3DRenderer(renderer);
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_Sweep_ParallelSweep_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_Sweep_ParallelSweep_SpaceKeyCallBack();
}

YSRESULT GeblGuiEditorBase::Edit_Sweep_ParallelSweep_SpaceKeyCallBack(void)
{
	if(NULL!=slHd)
	{
		YsArray <YsShellPolygonHandle> selPlHd;
		YsArray <YsShellExt::ConstEdgeHandle> selCeHd;

		slHd->GetSelectedConstEdge(selCeHd);
		slHd->GetSelectedPolygon(selPlHd);

		if(0<selCeHd.GetN() || 0<selPlHd.GetN())
		{
			const double scaling=parallelSweepDlg->GetScalingFactor();
			const YsVec3 cen=sweepInfo->GetCenter(*(const YsShellExt *)slHd);
			const YsVec3 displace=threeDInterface.point_pos-threeDInterface.point_pos_org;

			YsShellExtEdit::StopIncUndo incUndo(slHd);

			YsArray <YsShellVertexHandle,16> vtHdArray;
			sweepInfo->GetVertexAll(vtHdArray);

			for(YSSIZE_T vtIdx=0; vtIdx<vtHdArray.GetN(); ++vtIdx)
			{
				YsVec3 pos;
				slHd->GetVertexPosition(pos,vtHdArray[vtIdx]);
				pos=cen+displace+(pos-cen)*scaling;

				YsShellVertexHandle newVtHd=slHd->AddVertex(pos);
				sweepInfo->AddMapping(*(const YsShellExt *)slHd,vtHdArray[vtIdx],newVtHd);
			}

			YsArray <YsShellPolygonHandle> newPlHdArray;
			for(YSSIZE_T edIdx=0; edIdx<sweepInfo->GetNumEdgePiece(); ++edIdx)
			{
				YsShellVertexHandle edVtHd[2];
				YsArray <YsShellVertexHandle> mappedVtHd[2];
				if(YSOK==sweepInfo->GetEdge(edVtHd,edIdx)&&
				   YSOK==sweepInfo->FindMapping(mappedVtHd[0],*(const YsShellExt *)slHd,edVtHd[0]) &&
				   YSOK==sweepInfo->FindMapping(mappedVtHd[1],*(const YsShellExt *)slHd,edVtHd[1]) &&
				   1<=mappedVtHd[0].GetN() &&
				   1<=mappedVtHd[1].GetN())
				{
					YsShellVertexHandle quad[4]=
					{
						edVtHd[0],
						edVtHd[1],
						mappedVtHd[1][0],
						mappedVtHd[0][0]
					};
					YsShellPolygonHandle newPlHd=slHd->AddPolygon(4,quad);
					newPlHdArray.Append(newPlHd);
				}
			}
			YsShellExtEdit_OrientationUtil flipper;
			flipper.RecalculateNormal(*slHd,newPlHdArray);

			YsArray <YsShellVertexHandle,4> srcPlVtHd;
			YsArray <YsShellVertexHandle,4> newPlVtHd;
			YsArray <YsShellVertexHandle> mappedVtHd;
			for(auto plHd : selPlHd)
			{
				slHd->GetVertexListOfPolygon(srcPlVtHd,plHd);
				newPlVtHd=srcPlVtHd;
				for(auto &newVtHd : newPlVtHd)
				{
					if(YSOK!=sweepInfo->FindMapping(mappedVtHd,*(const YsShellExt *)slHd,newVtHd) ||
					   1!=mappedVtHd.GetN())
					{
						goto NEXTPOLYGON;
					}
					newVtHd=mappedVtHd[0];
				}
				slHd->SetPolygonVertex(plHd,newPlVtHd);
				if(YSTRUE!=parallelSweepDlg->GetDeleteSourcePolygon())
				{
					srcPlVtHd.Invert();
					YsShellPolygonHandle newPlHd=slHd->AddPolygon(srcPlVtHd);

					auto *attribPtr=slHd->GetPolygonAttrib(plHd);
					YsColor col;
					YsVec3 nom;
					slHd->GetColorOfPolygon(col,plHd);
					slHd->GetNormalOfPolygon(nom,plHd);

					slHd->SetPolygonAttrib(newPlHd,*attribPtr);
					slHd->SetPolygonNormal(newPlHd,-nom);
					slHd->SetPolygonColor(newPlHd,col);
				}

			NEXTPOLYGON:
				;
			}

			slHd->SelectPolygon(0,NULL);
			slHd->SelectConstEdge(0,NULL);

			needRemakeDrawingBuffer|=(NEED_REMAKE_DRAWING_VERTEX|NEED_REMAKE_DRAWING_POLYGON|NEED_REMAKE_DRAWING_CONSTEDGE|
			                          NEED_REMAKE_DRAWING_SELECTED_POLYGON|NEED_REMAKE_DRAWING_SELECTED_CONSTEDGE);
			SetNeedRedraw(YSTRUE);

			Edit_ClearUIMode();

			return YSOK;
		}
		else
		{
			MessageDialog(L"Error!",L"At least one polygon or a constraint edge must be selected.");
		}
	}
	return YSERR;
}

////////////////////////////////////////////////////////////

PolyCreEditParallelSweepWithPathDialog::PolyCreEditParallelSweepWithPathDialog()
{
	okBtn=NULL;
	cancelBtn=NULL;
	deleteSourcePolygonBtn=NULL;
}

PolyCreEditParallelSweepWithPathDialog::~PolyCreEditParallelSweepWithPathDialog()
{
}

/*static*/ PolyCreEditParallelSweepWithPathDialog *PolyCreEditParallelSweepWithPathDialog::Create(GeblGuiEditorBase *canvas)
{
	PolyCreEditParallelSweepWithPathDialog *dlg=new PolyCreEditParallelSweepWithPathDialog;
	dlg->canvas=canvas;
	return dlg;
}

/*static*/ void PolyCreEditParallelSweepWithPathDialog::Delete(PolyCreEditParallelSweepWithPathDialog *ptr)
{
	delete ptr;
}

YSRESULT PolyCreEditParallelSweepWithPathDialog::Make(void)
{
	if(NULL!=canvas->Slhd())
	{
		auto &shl=*(canvas->Slhd());
		auto pathCeHd=shl.GetSelectedConstEdge();
		auto srcPlHd=shl.GetSelectedPolygon();

		auto selFgHd=shl.GetSelectedFaceGroup();
		for(auto fgHd : selFgHd)
		{
			YsArray <YsShellPolygonHandle> fgPlHd;
			shl.GetFaceGroup(fgPlHd,fgHd);
			srcPlHd.Append(fgPlHd);
		}

		if(1>pathCeHd.GetN() || 2<pathCeHd.GetN() || 0==srcPlHd.GetN())
		{
			canvas->MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_SWEEP_NEEDPATHANDSRC);
			return YSERR;
		}

		srcPlHdCache=srcPlHd;

		YSBOOL isLoop;
		shl.GetConstEdge(pathVtHdCache,isLoop,pathCeHd[0]);

		scaling.CleanUp();
		if(2==pathCeHd.GetN())
		{
			YsArray <YsShellVertexHandle> guideVtHdArray;
			YSBOOL isLoop;
			shl.GetConstEdge(guideVtHdArray,isLoop,pathCeHd[1]);

			YsVec3 sweepDir=YsShell_CalculatePolygonAverageNormal((const YsShell &)shl,srcPlHdCache);
			scaling=YsShellExt_SweepInfoMultiStep::CalculateScalingForParallelSweepWithPathAndGuideLine(
				(const YsShellExt &)shl,sweepDir,pathVtHdCache,guideVtHdArray);
		}
		if(0==scaling.GetN())
		{
			scaling.Set(pathVtHdCache.GetN(),NULL);
			for(auto &s : scaling)
			{
				s=1.0;
			}
		}


		FsGuiDialog::Initialize();

		okBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_COMMON_OK,YSTRUE);
		cancelBtn=AddTextButton(0,FSKEY_NULL,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);
		deleteSourcePolygonBtn=AddTextButton(0,FSKEY_NULL,FSGUI_CHECKBOX,FSGUI_DLG_SWEEP_DELSRCPLG,YSTRUE);

		scaleTxt.CleanUp();
		for(YSSIZE_T idx=1; idx<20 && idx<pathVtHdCache.GetN(); ++idx)
		{
			scaleTxt.Append(AddTextBox(0,FSKEY_NULL,FSGUI_DLG_COMMON_SCALINGFACTOR,L"1.0",5,YSTRUE));
			scaleTxt.Last()->SetRealNumber(scaling[idx],2);
			scaleTxt.Last()->SetTextType(FSGUI_REALNUMBER);
		}

		SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
		SetBackgroundAlpha(0.3);
		Fit();

		SetUpParallelSweepWithPath();
		RemakeDrawingBuffer();

		return YSOK;
	}
	return YSERR;
}

YSBOOL PolyCreEditParallelSweepWithPathDialog::GetDeleteSourcePolygon(void) const
{
	return deleteSourcePolygonBtn->GetCheck();
}

void PolyCreEditParallelSweepWithPathDialog::OnButtonClick(FsGuiButton *btn)
{
	if(okBtn==btn)
	{
		canvas->SpaceKeyCallBack(*canvas);
	}
	else if(cancelBtn==btn)
	{
		canvas->Edit_ClearUIMode();
	}
}

void PolyCreEditParallelSweepWithPathDialog::SetUpParallelSweepWithPath(void)
{
	if(0<pathVtHdCache.GetN())
	{
		scaling.Set(pathVtHdCache.GetN(),NULL);
		scaling[0]=1.0;
		for(auto index : scaling.AllIndex())
		{
			if(0<index && index<=scaleTxt.GetN())
			{
				scaling[index]=scaleTxt[index-1]->GetRealNumber();
			}
			else
			{
				scaling[index]=scaleTxt.Last()->GetRealNumber();
			}
		}

		YsArray <YsShellExt::ConstEdgeHandle> empty;
		sweepUtil.CleanUp();
		sweepUtil.MakeInfo(*(const YsShellExt *)(canvas->Slhd()),srcPlHdCache,empty);
		sweepUtil.SetUpParallelSweepWithPath(*(const YsShellExt *)(canvas->Slhd()),pathVtHdCache,scaling);
	}
}

void PolyCreEditParallelSweepWithPathDialog::RemakeDrawingBuffer(void)
{
	const auto &shl=*(canvas->Slhd());

	lineBuf.CleanUp();
	for(auto &layer : sweepUtil.layerArray)
	{
		for(YSSIZE_T idx=0; idx<=sweepUtil.srcEdVtHd.GetN()-2; idx+=2)
		{
			YSSIZE_T matchingIndex[2];
			if(YSOK==sweepUtil.srcVtKeyToMidPointIndex.FindElement(matchingIndex[0],shl.GetSearchKey(sweepUtil.srcEdVtHd[idx])) &&
			   YSOK==sweepUtil.srcVtKeyToMidPointIndex.FindElement(matchingIndex[1],shl.GetSearchKey(sweepUtil.srcEdVtHd[idx+1])))
			{
				const YsVec3 pos[2]=
				{
					layer.pointArray[matchingIndex[0]].pos,
					layer.pointArray[matchingIndex[1]].pos
				};
				lineBuf.AddVertex(pos[0]);
				lineBuf.AddVertex(pos[1]);
			}
		}
	}
}

void PolyCreEditParallelSweepWithPathDialog::OnTextBoxChange(FsGuiTextBox *txt)
{
	if(YsTolerance<txt->GetRealNumber())
	{
		SetUpParallelSweepWithPath();
		RemakeDrawingBuffer();
		canvas->SetNeedRedraw(YSTRUE);
	}
}

/*static*/ void GeblGuiEditorBase::Edit_Sweep_ParallelSweepWithPath(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;
	if(YSOK==canvas.parallelSweepWithPathDlg->Make())
	{
		canvas.Edit_ClearUIMode();

		canvas.AddDialog(canvas.parallelSweepWithPathDlg);
		canvas.ArrangeDialog();

		canvas.UIDrawCallBack3D=Edit_Sweep_ParallelSweepWithPath_DrawCallBack3D;
		canvas.SpaceKeyCallBack=Edit_Sweep_ParallelSweepWithPath_SpaceKeyCallBack;

		canvas.undoRedoRequireToClearUIMode=YSTRUE;
		canvas.deletionRequireToClearUIMode=YSTRUE;
	}
}

/*static*/ void GeblGuiEditorBase::Edit_Sweep_ParallelSweepWithPath_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	canvas.Edit_Sweep_ParallelSweepWithPath_DrawCallBack3D();
}

void GeblGuiEditorBase::Edit_Sweep_ParallelSweepWithPath_DrawCallBack3D(void)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,1.0f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINES,(int)parallelSweepWithPathDlg->lineBuf.GetNumVertex(),parallelSweepWithPathDlg->lineBuf);

	YsGLSLEndUse3DRenderer(renderer);
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_Sweep_ParallelSweepWithPath_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_Sweep_ParallelSweepWithPath_SpaceKeyCallBack();
}

YSRESULT GeblGuiEditorBase::Edit_Sweep_ParallelSweepWithPath_SpaceKeyCallBack(void)
{
	if(NULL!=Slhd())
	{
		YsShellExtEdit::StopIncUndo incUndo(*Slhd());

		auto &sweepUtil=parallelSweepWithPathDlg->sweepUtil;
		for(auto &layer : sweepUtil.layerArray)
		{
			for(auto &point : layer.pointArray)
			{
				point.vtHd=Slhd()->AddVertex(point.pos);
			}
		}

		auto quadArray=sweepUtil.MakeSideFaceAndFirstToLastVertexMapping(*(const YsShellExt *)Slhd());
		for(auto &quad : quadArray)
		{
			auto plHd=Slhd()->AddPolygon(4,quad.quadVtHd);
			Slhd()->SetPolygonColor(plHd,colorPaletteDlg->GetColor());
		}

		for(auto plHd : parallelSweepWithPathDlg->srcPlHdCache)
		{
			YsArray <YsShellVertexHandle,4> plVtHd;
			Slhd()->GetPolygon(plVtHd,plHd);

			if(YSOK==sweepUtil.MapVertexArray(plVtHd,*(const YsShellExt *)Slhd()))
			{
				if(YSTRUE==parallelSweepWithPathDlg->deleteSourcePolygonBtn->GetCheck())
				{
					Slhd()->SetPolygonVertex(plHd,plVtHd);
				}
				else
				{
					auto newPlHd=Slhd()->AddPolygon(plVtHd);
					auto attrib=Slhd()->GetPolygonAttrib(plHd);
					auto nom=Slhd()->GetNormal(plHd);
					auto col=Slhd()->GetColor(plHd);

					Slhd()->SetPolygonAttrib(newPlHd,*attrib);
					Slhd()->SetPolygonNormal(newPlHd,nom);
					Slhd()->SetPolygonColor(newPlHd,col);
				}
			}
		}

		slHd->SelectPolygon(0,NULL);
		slHd->SelectConstEdge(0,NULL);

		needRemakeDrawingBuffer|=(NEED_REMAKE_DRAWING_VERTEX|NEED_REMAKE_DRAWING_POLYGON|
		                          NEED_REMAKE_DRAWING_SELECTED_POLYGON|NEED_REMAKE_DRAWING_SELECTED_CONSTEDGE);
		SetNeedRedraw(YSTRUE);

		Edit_ClearUIMode();

		return YSOK;
	}

	return YSERR;
}

////////////////////////////////////////////////////////////

/* static */ void GeblGuiEditorBase::Edit_Sweep_AlongSweepPath(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	printf("%s %d\n",__FUNCTION__,__LINE__);
	auto &canvas=*(GeblGuiEditorBase *)appPtr;
	if(NULL!=canvas.Slhd())
	{
		auto &shl=*(canvas.Slhd());
		auto pathCeHd=shl.GetSelectedConstEdge();
		auto srcPlHd=shl.GetSelectedPolygon();

		auto selFgHd=shl.GetSelectedFaceGroup();
		for(auto fgHd : selFgHd)
		{
			YsArray <YsShellPolygonHandle> fgPlHd;
			shl.GetFaceGroup(fgPlHd,fgHd);
			srcPlHd.Append(fgPlHd);
		}

		if(1>pathCeHd.GetN() || 2<pathCeHd.GetN() || 0==srcPlHd.GetN())
		{
			canvas.MessageDialog(FSGUI_COMMON_ERROR,FSGUI_ERROR_SWEEP_NEEDPATHANDSRC);
			return;
		}

		auto srcPlHdCache=srcPlHd;

		YsArray <YsShellVertexHandle> pathVtHdCache;
		YSBOOL isLoop;
		shl.GetConstEdge(pathVtHdCache,isLoop,pathCeHd[0]);

		YsArray <double> scaling(pathVtHdCache.GetN(),NULL);
		for(auto &s : scaling)
		{
			s=1.0;
		}


		YsShellExtEdit::StopIncUndo incUndo(shl);
		YsShellExt_SweepInfoMultiStep sweepUtil;

		YsArray <YsShellExt::ConstEdgeHandle> empty;
		sweepUtil.CleanUp();
		sweepUtil.MakeInfo(shl.Conv(),srcPlHdCache,empty);
		if(YSOK==sweepUtil.SetUpNonParallelSweepWithPath(shl.Conv(),pathVtHdCache,isLoop,scaling,YsShellExt_SweepInfoMultiStep::ORICON_AVERAGE_ANGLE,YsShellExt_SweepInfoMultiStep::ORICON_AVERAGE_ANGLE))
		{
			for(auto &layer : sweepUtil.layerArray)
			{
				for(auto &point : layer.pointArray)
				{
					if(NULL==point.vtHd)
					{
						point.vtHd=shl.AddVertex(point.pos);
					}
				}
			}
		}



		auto quadArray=sweepUtil.MakeSideFaceAndFirstToLastVertexMapping(shl.Conv());
		for(auto &quad : quadArray)
		{
			auto plHd=shl.AddPolygon(4,quad.quadVtHd);
			shl.SetPolygonColor(plHd,canvas.colorPaletteDlg->GetColor());
		}

		if(YSTRUE==isLoop)
		{
			for(auto plHd : srcPlHdCache)
			{
				shl.DeletePolygon(plHd);
			}
		}
		else
		{
			const YSBOOL deleteSrcPolygon=YSFALSE;
			for(auto plHd : srcPlHdCache)
			{
				YsArray <YsShellVertexHandle,4> plVtHd;
				shl.GetPolygon(plVtHd,plHd);

				if(YSOK==sweepUtil.MapVertexArray(plVtHd,shl.Conv()))
				{
					if(YSTRUE==deleteSrcPolygon)
					{
						shl.SetPolygonVertex(plHd,plVtHd);
					}
					else
					{
						auto newPlHd=shl.AddPolygon(plVtHd);
						auto attrib=shl.GetPolygonAttrib(plHd);
						auto nom=shl.GetNormal(plHd);
						auto col=shl.GetColor(plHd);

						shl.SetPolygonAttrib(newPlHd,*attrib);
						shl.SetPolygonNormal(newPlHd,nom);
						shl.SetPolygonColor(newPlHd,col);
					}
				}
			}
		}



		shl.SelectPolygon(0,NULL);
		shl.SelectConstEdge(0,NULL);

		canvas.needRemakeDrawingBuffer|=(NEED_REMAKE_DRAWING_VERTEX|NEED_REMAKE_DRAWING_POLYGON|
		                          NEED_REMAKE_DRAWING_SELECTED_POLYGON|NEED_REMAKE_DRAWING_SELECTED_CONSTEDGE);
		canvas.SetNeedRedraw(YSTRUE);

		canvas.Edit_ClearUIMode();
	}
}

////////////////////////////////////////////////////////////

PolyCreEditSweepConstEdgeDialog::PolyCreEditSweepConstEdgeDialog()
{
	okBtn=NULL;
	cancelBtn=NULL;
	invertBtn=NULL;
	canvas=NULL;
}

PolyCreEditSweepConstEdgeDialog::~PolyCreEditSweepConstEdgeDialog()
{
}

/*static*/ PolyCreEditSweepConstEdgeDialog *PolyCreEditSweepConstEdgeDialog::Create(GeblGuiEditorBase *canvas)
{
	PolyCreEditSweepConstEdgeDialog *dlg=new PolyCreEditSweepConstEdgeDialog;
	dlg->canvas=canvas;
	return dlg;
}

/*static*/ void PolyCreEditSweepConstEdgeDialog::Delete(PolyCreEditSweepConstEdgeDialog *ptr)
{
	delete ptr;
}

void PolyCreEditSweepConstEdgeDialog::CleanUp(void)
{
	ceHdArray.CleanUp();
	patchUtil.CleanUp();
}

void PolyCreEditSweepConstEdgeDialog::Make(void)
{
	if(NULL==okBtn)
	{
		FsGuiDialog::Initialize();

		okBtn=AddTextButton(0,FSKEY_SPACE,FSGUI_PUSHBUTTON,FSGUI_DLG_SWEEPCONSTEDGE_SWEEP,YSTRUE);
		cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);
		invertBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_DLG_SWEEPCONSTEDGE_INVERT,YSFALSE);
	}

	SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
	SetBackgroundAlpha(0.3);
	Fit();
}

YSRESULT PolyCreEditSweepConstEdgeDialog::SetUp(void)
{
	if(NULL!=canvas->slHd)
	{
		auto selCeHd=canvas->slHd->GetSelectedConstEdge();
		if(2<=selCeHd.GetN())
		{
			ceHdArray=selCeHd;
			patchUtil.Set(selCeHd.GetN()-1,NULL);

			for(YSSIZE_T idx=0; idx<ceHdArray.GetN()-1; ++idx)
			{
				YsArray <YsShellVertexHandle> ceVtHd[2];
				YSBOOL isLoop[2];

				canvas->slHd->GetConstEdge(ceVtHd[0],isLoop[0],ceHdArray[idx]);
				canvas->slHd->GetConstEdge(ceVtHd[1],isLoop[1],ceHdArray[idx+1]);

				patchUtil[idx].CleanUp();
				if(YSOK!=patchUtil[idx].SetVertexSequence(*(const YsShell *)(canvas->slHd),ceVtHd,isLoop))
				{
					return YSERR;
				}
				patchUtil[idx].CalculateParameter();
				patchUtil[idx].MakeInitialGuess();
				patchUtil[idx].MinimizeDihedralAngleSum();
			}

			return YSOK;
		}
	}
	return YSERR;
}

void PolyCreEditSweepConstEdgeDialog::OnButtonClick(FsGuiButton *btn)
{
	if(btn==okBtn)
	{
		canvas->Edit_Sweep_BetweenConstEdge_SpaceKeyCallBack(*canvas);
	}
	else if(btn==cancelBtn)
	{
		canvas->Edit_ClearUIMode();
	}
	else if(btn==invertBtn)
	{
		for(auto &patch : patchUtil)
		{
			YsShellExt_OrientationUtil::InvertPolygonAll(patch.shl);
			RemakeDrawingBuffer();
			canvas->SetNeedRedraw(YSTRUE);
		}
	}
}

void PolyCreEditSweepConstEdgeDialog::RecalculateNormal(void)
{
	for(auto &patch : patchUtil)
	{
		YsShellExt_OrientationUtil orientationUtil;
		orientationUtil.CleanUp();
		orientationUtil.FixOrientationOfClosedSolid(patch.shl);
		auto plHdNeedFip=orientationUtil.GetPolygonNeedFlip();

		for(auto plHd : plHdNeedFip)
		{
			YsArray <YsShellVertexHandle,4> plVtHd;
			patch.shl.GetPolygon(plVtHd,plHd);
			plVtHd.Invert();
			patch.shl.SetPolygonVertex(plHd,plVtHd);
		}

		orientationUtil.RecalculateNormalFromCurrentOrientation((const YsShell &)patch.shl);
		auto plNomPairNeedNormalReset=orientationUtil.GetPolygonNormalPair();
		for(auto plNomPair : plNomPairNeedNormalReset)
		{
			patch.shl.SetPolygonNormal(plNomPair.plHd,plNomPair.nom);
		}
	}
}

void PolyCreEditSweepConstEdgeDialog::RemakeDrawingBuffer(void)
{
	lineBuf.CleanUp();
	triBuf.CleanUp();

	for(auto &patch : patchUtil)
	{
		const auto &shl=patch.shl;
		YsShellEdgeEnumHandle edHd=NULL;
		while(YSOK==shl.MoveToNextEdge(edHd))
		{
			YsShellVertexHandle edVtHd[2];
			shl.GetEdge(edVtHd,edHd);

			lineBuf.AddVertex(shl.GetVertexPosition(edVtHd[0]));
			lineBuf.AddVertex(shl.GetVertexPosition(edVtHd[1]));
		}

		for(auto plHd : shl.AllPolygon())
		{
			YsArray <YsVec3,4> plVtPos;
			shl.GetPolygon(plVtPos,plHd);

			YsBoundingBoxMaker3 mkBbx(plVtPos);
			double l=mkBbx.GetDiagonalLength();

			const YsVec3 cen=shl.GetCenter(plHd);
			const YsVec3 nom=shl.GetNormal(plHd);
			const YsVec3 arrowHead=cen+nom*l/5.0;
			lineBuf.AddVertex(cen);
			lineBuf.AddVertex(arrowHead);

			triBuf.AddArrowHeadTriangle(cen,arrowHead,0.1);
		}
	}
}

/*static*/ void GeblGuiEditorBase::Edit_Sweep_BetweenConstEdge(void *appPtr,FsGuiPopUpMenu *,FsGuiPopUpMenuItem *)
{
	GeblGuiEditorBase &canvas=*(GeblGuiEditorBase *)appPtr;

	canvas.Edit_ClearUIMode();

	auto selCeHd=canvas.slHd->GetSelectedConstEdge();
	if(2<=selCeHd.GetN())
	{
		canvas.sweepConstEdgeDlg->Make();
		if(YSOK==canvas.sweepConstEdgeDlg->SetUp())
		{
			canvas.sweepConstEdgeDlg->RecalculateNormal();
			canvas.sweepConstEdgeDlg->RemakeDrawingBuffer();

			canvas.AddDialog(canvas.sweepConstEdgeDlg);
			canvas.ArrangeDialog();

			canvas.UIDrawCallBack3D=Edit_Sweep_BetweenConstEdge_DrawCallBack3D;
			canvas.SpaceKeyCallBack=Edit_Sweep_BetweenConstEdge_SpaceKeyCallBack;
		}
		else
		{
			canvas.MessageDialog(FSGUI_COMMON_ERROR,L"Cannot prepare sweeping calculation.");
		}
	}
}

/*static*/ void GeblGuiEditorBase::Edit_Sweep_BetweenConstEdge_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,1.0f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINES,(int)canvas.sweepConstEdgeDlg->lineBuf.GetNumVertex(),canvas.sweepConstEdgeDlg->lineBuf);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_TRIANGLES,(int)canvas.sweepConstEdgeDlg->triBuf.GetNumVertex(),canvas.sweepConstEdgeDlg->triBuf);

	YsGLSLEndUse3DRenderer(renderer);
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_Sweep_BetweenConstEdge_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_Sweep_BetweenConstEdge_Sweep();
}

YSRESULT GeblGuiEditorBase::Edit_Sweep_BetweenConstEdge_Sweep(void)
{
	if(NULL!=slHd)
	{
		YsShellExtEdit::StopIncUndo incUndo(slHd);

		const YsColor col=colorPaletteDlg->GetColor();

		for(auto &patch : sweepConstEdgeDlg->patchUtil)
		{
			for(auto patchPlHd : patch.shl.AllPolygon())
			{
				YsArray <YsShellVertexHandle,4> plVtHd;
				YsVec3 nom;

				patch.GetPolygonVertexOfOriginalShell(plVtHd,nom,patchPlHd);

				auto newPlHd=slHd->AddPolygon(plVtHd);
				slHd->SetPolygonNormal(newPlHd,nom);
				slHd->SetPolygonColor(newPlHd,col);
			}
		}

		slHd->SelectConstEdge(0,NULL);
	}

	needRemakeDrawingBuffer=NEED_REMAKE_DRAWING_POLYGON|NEED_REMAKE_DRAWING_SELECTED_CONSTEDGE;
	SetNeedRedraw(YSTRUE);

	Edit_ClearUIMode();
	return YSOK;
}

////////////////////////////////////////////////////////////

PolyCreExtendOrShrinkDialog::PolyCreExtendOrShrinkDialog()
{
	canvas=NULL;
	okBtn=NULL;
	cancelBtn=NULL;
}

PolyCreExtendOrShrinkDialog::~PolyCreExtendOrShrinkDialog()
{
}

/*static*/ PolyCreExtendOrShrinkDialog *PolyCreExtendOrShrinkDialog::Create(GeblGuiEditorBase *canvas)
{
	PolyCreExtendOrShrinkDialog *dlg=new PolyCreExtendOrShrinkDialog;
	dlg->canvas=canvas;
	return dlg;
}

/*static*/ void PolyCreExtendOrShrinkDialog::Delete(PolyCreExtendOrShrinkDialog *ptr)
{
	delete ptr;
}

YSRESULT PolyCreExtendOrShrinkDialog::Make(class YsShellExtEdit &shl,const YsShellPolygonStore &targetPlHdStore)
{
	this->shl=&shl;
	this->targetPlHdArray.CleanUp();
	for(auto plHd : targetPlHdStore)
	{
		this->targetPlHdArray.Append(plHd);
	}

	YsShellExt_BoundaryInfo boundaryInfo;
	boundaryInfo.MakeInfo((const YsShell &)shl,targetPlHdArray);
	boundaryInfo.CacheNonDuplicateVtHdArray();
	auto allVtHd=boundaryInfo.GetVertexAll();


	YsArray <YsVec3> allVtPos;
	for(auto vtHd : allVtHd)
	{
		allVtPos.Append(shl.GetVertexPosition(vtHd));
	}
	if(YSOK!=YsFindLeastSquareFittingPlaneNormal(moveDir,allVtPos))
	{
		return YSERR;
	}


	extensionTarget.CleanUp();

	for(auto vtHd : allVtHd)
	{
		YsShellVertexHandle hairVtHd=NULL;
		auto connVtHdArray=shl.GetConnectedVertex(vtHd);
		for(auto connVtHd : connVtHdArray)
		{
			YsArray <YsShellPolygonHandle> edPlHdAll;
			shl.FindPolygonFromEdgePiece(edPlHdAll,vtHd,connVtHd);

			YSBOOL isUsedByTargetPlHd=YSFALSE;
			for(auto edPlHd : edPlHdAll)
			{
				if(YSTRUE==targetPlHdStore.IsIncluded(edPlHd))
				{
					isUsedByTargetPlHd=YSTRUE;
					break;
				}
			}
			if(YSTRUE!=isUsedByTargetPlHd)
			{
				if(NULL!=hairVtHd)
				{
					return YSERR;  // Hair not unique
				}
				else
				{
					hairVtHd=connVtHd;
				}
			}
		}
		if(NULL==hairVtHd)
		{
			return YSERR; // Hiar not found
		}

		auto org=shl.GetVertexPosition(vtHd);
		auto from=shl.GetVertexPosition(hairVtHd);
		auto vec=org-from;
		if(YSOK!=vec.Normalize())
		{
			return YSERR;
		}

		extensionTarget.Increment();
		extensionTarget.Last().toVtHd=vtHd;
		extensionTarget.Last().fromVtHd=hairVtHd;
		extensionTarget.Last().org=org;
		extensionTarget.Last().vec=vec;
		extensionTarget.Last().newPos=org;
	}

	center=boundaryInfo.GetCenter((const YsShellExt &)shl);

	if(NULL==okBtn)
	{
		FsGuiDialog::Initialize();
		okBtn=AddTextButton(0,FSKEY_ENTER,FSGUI_PUSHBUTTON,FSGUI_COMMON_OK,YSTRUE);
		cancelBtn=AddTextButton(0,FSKEY_ESC,FSGUI_PUSHBUTTON,FSGUI_COMMON_CANCEL,YSFALSE);

		SetArrangeType(FSDIALOG_ARRANGE_TOP_LEFT);
		SetBackgroundAlpha(0.3);
		Fit();
	}

	RemakeDrawingBuffer();

	return YSOK;
}

void PolyCreExtendOrShrinkDialog::RemakeDrawingBuffer(void)
{
	lineBuf.CleanUp();
	triBuf.CleanUp();

	YsPlane prjPln(center,moveDir);
	for(auto &ext : extensionTarget)
	{
		if(YSOK==prjPln.GetIntersectionHighPrecision(ext.newPos,ext.org,ext.vec))
		{
			lineBuf.AddVertex(ext.org);
			lineBuf.AddVertex(ext.newPos);

			triBuf.AddArrowHeadTriangle(ext.org,ext.newPos,0.1);
		}
	}
}

void PolyCreExtendOrShrinkDialog::OnButtonClick(FsGuiButton *btn)
{
	if(okBtn==btn)
	{
		canvas->SpaceKeyCallBack(*canvas);
	}
	else if(cancelBtn==btn)
	{
		canvas->Edit_ClearUIMode();
	}
}



void GeblGuiEditorBase::Edit_Sweep_ExtendOrShrink(FsGuiPopUpMenuItem *)
{
	if(NULL==Slhd())
	{
		return;
	}

	YsShellExtEdit &shl=*Slhd();
	auto selPlHd=shl.GetSelectedPolygon();
	auto selFgHd=shl.GetSelectedFaceGroup();

	YsShellPolygonStore targetPlHd((const YsShell &)shl);
	targetPlHd.AddPolygon(selPlHd);
	for(auto fgHd : selFgHd)
	{
		YsArray <YsShellPolygonHandle> fgPlHd;
		shl.GetFaceGroup(fgPlHd,fgHd);
		targetPlHd.AddPolygon(fgPlHd);
	}

	if(0<targetPlHd.GetN())
	{
		if(YSOK==extendOrShrinkDlg->Make(shl,targetPlHd))
		{
			Edit_ClearUIMode();

			AddDialog(extendOrShrinkDlg);
			ArrangeDialog();

			UIDrawCallBack3D=Edit_Sweep_ExtendOrShrink_DrawCallBack3D;
			SpaceKeyCallBack=Edit_Sweep_ExtendOrShrink_SpaceKeyCallBack;

			// Set up 3D interface if necessary  Yes, I do need 3D itfc
			threeDInterface.BeginInputPoint2(extendOrShrinkDlg->center);
			threeDInterface.BindCallBack(&THISCLASS::Edit_Sweep_ExtendOrShrink_3DInterfaceCallBack,this);

			undoRedoRequireToClearUIMode=YSTRUE;
			deletionRequireToClearUIMode=YSTRUE;
		}
	}
	else
	{
		// Tell the user something is missing.
	}
}

/*static*/ void GeblGuiEditorBase::Edit_Sweep_ExtendOrShrink_DrawCallBack3D(GeblGuiEditorBase &canvas)
{
	struct YsGLSL3DRenderer *renderer=YsGLSLSharedFlat3DRenderer();
	YsGLSLUse3DRenderer(renderer);

	const GLfloat edgeCol[4]={0,1.0f,0,1.0f};
	YsGLSLSet3DRendererUniformColorfv(renderer,edgeCol);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_LINES,(int)canvas.extendOrShrinkDlg->lineBuf.GetNumVertex(),canvas.extendOrShrinkDlg->lineBuf);
	YsGLSLDrawPrimitiveVtxfv(renderer,GL_TRIANGLES,(int)canvas.extendOrShrinkDlg->triBuf.GetNumVertex(),canvas.extendOrShrinkDlg->triBuf);

	YsGLSLEndUse3DRenderer(renderer);
}

void GeblGuiEditorBase::Edit_Sweep_ExtendOrShrink_3DInterfaceCallBack(class FsGui3DInterface *itfc)
{
	extendOrShrinkDlg->center=itfc->point_pos;
	extendOrShrinkDlg->RemakeDrawingBuffer();
	SetNeedRedraw(YSTRUE);
}

/*static*/ YSRESULT GeblGuiEditorBase::Edit_Sweep_ExtendOrShrink_SpaceKeyCallBack(GeblGuiEditorBase &canvas)
{
	return canvas.Edit_Sweep_ExtendOrShrink_SpaceKeyCallBack();
}

YSRESULT GeblGuiEditorBase::Edit_Sweep_ExtendOrShrink_SpaceKeyCallBack(void)
{
	if(NULL==Slhd())
	{
		return YSERR;
	}
	YsShellExtEdit &shl=*Slhd();

	YsShellExtEdit::StopIncUndo incUndo(shl);
	for(auto &target : extendOrShrinkDlg->extensionTarget)
	{
		shl.SetVertexPosition(target.toVtHd,target.newPos);
	}

	shl.SelectConstEdge(0,NULL);
	shl.SelectPolygon(0,NULL);

	needRemakeDrawingBuffer=(unsigned int)NEED_REMAKE_DRAWING_ALL;
	SetNeedRedraw(YSTRUE);

	Edit_ClearUIMode();

	return YSOK;
}

////////////////////////////////////////////////////////////

