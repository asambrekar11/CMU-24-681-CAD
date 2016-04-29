/* ////////////////////////////////////////////////////////////

File Name: edit_sweepdialog.h
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

#ifndef EDIT_SWEEPDIALOG_IS_INCLUDED
#define EDIT_SWEEPDIALOG_IS_INCLUDED
/* { */

#include <fsgui.h>
#include <ysglbuffer.h>

#include <ysshellext_sweeputil.h>

class PolyCreEditParallelSweepDialog : public FsGuiDialog
{
private:
	PolyCreEditParallelSweepDialog(){};
	~PolyCreEditParallelSweepDialog(){};

	class GeblGuiEditorBase *canvas;
	FsGuiButton *okBtn,*cancelBtn;
	FsGuiTextBox *scaleTxt;
	FsGuiButton *deleteSourcePolygonBtn;

public:
	static PolyCreEditParallelSweepDialog *Create(void);
	static void Delete(PolyCreEditParallelSweepDialog *dlg);

	void Make(class GeblGuiEditorBase *canvas);
	double GetScalingFactor(void) const;

	void SetDeleteSourcePolygon(YSBOOL sw);
	YSBOOL GetDeleteSourcePolygon(void) const;

	void OnButtonClick(FsGuiButton *btn);
};

////////////////////////////////////////////////////////////

class PolyCreEditParallelSweepWithPathDialog : public FsGuiDialog
{
private:
	PolyCreEditParallelSweepWithPathDialog();
	~PolyCreEditParallelSweepWithPathDialog();

public:
	class GeblGuiEditorBase *canvas;
	FsGuiButton *okBtn,*cancelBtn;
	FsGuiButton *deleteSourcePolygonBtn;
	YsArray <FsGuiTextBox *> scaleTxt;

	YsArray <YsShellPolygonHandle> srcPlHdCache;
	YsArray <YsShellVertexHandle> pathVtHdCache;
	YsArray <double> scaling;

	YsShellExt_SweepInfoMultiStep sweepUtil;
	YsGLVertexBuffer lineBuf;

public:
	static PolyCreEditParallelSweepWithPathDialog *Create(GeblGuiEditorBase *canvas);
	static void Delete(PolyCreEditParallelSweepWithPathDialog *ptr);

	YSRESULT Make(void);
	YSBOOL GetDeleteSourcePolygon(void) const;

	void SetUpParallelSweepWithPath(void);
	void RemakeDrawingBuffer(void);

	void OnButtonClick(FsGuiButton *btn);
	void OnTextBoxChange(FsGuiTextBox *txt);
};

#include <ysshellext_patchutil.h>

class PolyCreEditSweepConstEdgeDialog : public FsGuiDialog
{
private:
	class GeblGuiEditorBase *canvas;
	FsGuiButton *okBtn,*cancelBtn,*invertBtn;

	PolyCreEditSweepConstEdgeDialog();
	~PolyCreEditSweepConstEdgeDialog();
public:
	YsArray <YsShellExt::ConstEdgeHandle> ceHdArray;
	YsArray <YsShellExt_PatchBetweenTwoConstEdge> patchUtil;

	YsGLVertexBuffer lineBuf;
	YsGLVertexBuffer triBuf;

	static PolyCreEditSweepConstEdgeDialog *Create(GeblGuiEditorBase *canvas);
	static void Delete(PolyCreEditSweepConstEdgeDialog *ptr);

	void CleanUp(void);
	void Make(void);
	YSRESULT SetUp(void);

	void OnButtonClick(FsGuiButton *btn);

	void RecalculateNormal(void);
	void RemakeDrawingBuffer(void);
};

////////////////////////////////////////////////////////////

class PolyCreExtendOrShrinkDialog : public FsGuiDialog
{
private:
	FsGuiButton *okBtn,*cancelBtn;
	GeblGuiEditorBase *canvas;

public:
	class ToExtend
	{
	public:
		YsShellVertexHandle fromVtHd,toVtHd;
		YsVec3 org,vec,newPos;
	};
	YsArray <ToExtend> extensionTarget;
	YsGLVertexBuffer lineBuf,triBuf;

	YsVec3 moveDir,center;
	YsShellExtEdit *shl;
	YsArray <YsShellPolygonHandle> targetPlHdArray;

private:
	PolyCreExtendOrShrinkDialog();
	~PolyCreExtendOrShrinkDialog();

public:
	static PolyCreExtendOrShrinkDialog *Create(GeblGuiEditorBase *canvas);
	static void Delete(PolyCreExtendOrShrinkDialog *ptr);

	YSRESULT Make(class YsShellExtEdit &shl,const YsShellPolygonStore &targetPlHdStore);

	void RemakeDrawingBuffer(void);

	void OnButtonClick(FsGuiButton *btn);
};

/* } */
#endif
