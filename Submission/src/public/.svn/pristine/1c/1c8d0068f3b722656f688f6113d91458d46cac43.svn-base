/* ////////////////////////////////////////////////////////////

File Name: fsguibase.h
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

#ifndef FSGUIBASE_IS_INCLUDED
#define FSGUIBASE_IS_INCLUDED
/* { */



#include <ysclass.h>
#include <ysbitmap.h>
#include <ysbitmaparray.h>


class FsGuiBitmapType : public YsBitmapArray
{
public:
	YSBOOL IsEmpty(void) const;
	YSBOOL IsLoaded(void) const;
	int GetWidth(void) const;
	int GetHeight(void) const;
	void MakeSmallEmptyBitmap(void);
	void SetBitmap(const YsBitmap &bmpIn);
};



class FsGuiMouseButtonSet
{
public:
	YSBOOL lb,mb,rb;
	FsGuiMouseButtonSet(){}
	FsGuiMouseButtonSet(YSBOOL l,YSBOOL m,YSBOOL r)
	{
		lb=l;
		mb=m;
		rb=r;
	}
};



////////////////////////////////////////////////////////////

class FsGuiObject
{
public:
	enum SCHEME
	{
		PLAIN,
		MODERN
	};

public:
	enum FSGUI_DRAWINGTYPE
	{
		FSGUI_DRAWINGTYPE_UNKNOWN,
		FSGUI_DRAWINGTYPE_NORMAL,
		FSGUI_DRAWINGTYPE_ACTIVE,
		FSGUI_DRAWINGTYPE_INACTIVE
	};

	static SCHEME scheme;

	static YsColor defBgCol,defFgCol;
	static YsColor defActiveBgCol,defActiveFgCol;
	static YsColor defFrameCol;

	static YsColor defDialogBgCol;

	static YsColor defTabBgCol;
	static YsColor defTabClosedBgCol;
	static YsColor defTabClosedFgCol;

	// used in MODERN scheme
	static double defRoundRadius;

	// for ListBox and DropList
	static YsColor defListBgCol,defListFgCol;
	static YsColor defListActiveBgCol,defListActiveFgCol;

	static class YsFontRenderer *defUnicodeRenderer;
	static class YsFontRenderer *defAsciiRenderer;

	static int defHSpaceUnit,defVSpaceUnit,defHAnnotation,defHScrollBar;

	static int defBitmapWidthLimit,defBitmapHeightLimit;

	static YSRESULT GetUnicodeCharacterBitmapSize(int &wid,int &hei);  // Based on the size of L"X"
	static YSRESULT RenderUnicodeString(YsBitmap &bmp,const char str[],const YsColor &fgCol,const YsColor &bgCol);
	static YSRESULT RenderUnicodeString(YsBitmap &bmp,const wchar_t wStr[],const YsColor &fgCol,const YsColor &bgCol);
	static YSRESULT GetTightUnicodeRenderSize(int &wid,int &hei,const wchar_t wStr[]);

	static YSRESULT GetAsciiCharacterBitmapSize(int &wid,int &hei);  // Based on the size of "X"
	static YSRESULT GetAsciiStringBitmapSize(int &wid,int &hei,const char *str);
	static YSRESULT RenderAsciiString(YsBitmap &bmp,const char str[],const YsColor &fgCol,const YsColor &bgCol);
	static YSRESULT RenderAsciiString(YsBitmap &bmp,const wchar_t str[],const YsColor &fgCol,const YsColor &bgCol);
	static YSRESULT GetTightAsciiRenderSize(int &wid,int &hei,const char str[]);

	class TouchMovement
	{
	public:
		YsVec2 from,to;
	};
	class TouchState
	{
	public:
		YsArray <TouchMovement> movement;
		YsArray <YsVec2> newTouch;
		YsArray <YsVec2> endedTouch;

		YSSIZE_T GetNumCurrentTouch(void) const
		{
			return movement.GetN()+newTouch.GetN();
		}
	};
	static TouchState MatchTouch(const YsArray <YsVec2> &newTouch,const YsArray <YsVec2> &prevTouch);
	static YsArray <YsVec2> ConvertTouch(YSSIZE_T nTouch,const class FsVec2i touch[]);
};

////////////////////////////////////////////////////////////

class FsGuiFontRenderer
{
public:
	virtual YSRESULT RenderString(YsBitmap &bmp,const wchar_t wStr[],const YsColor &fgCol,const YsColor &bgCol) const=0;
	YSRESULT RenderString(YsBitmap &bmp,const char str[],const YsColor &fgCol,const YsColor &bgCol) const;
	virtual YSRESULT GetTightRenderSize(int &wid,int &hei,const wchar_t wStr[]) const=0;
};

class FsGuiAsciiFixedFontRenderer : public FsGuiFontRenderer
{
public:
	virtual int GetFontWidth(void) const=0;
	virtual int GetFontHeight(void) const=0;
	virtual YSRESULT RenderString(YsBitmap &bmp,const wchar_t wStr[],const YsColor &fgCol,const YsColor &bgCol) const=0;
	virtual YSRESULT GetTightRenderSize(int &wid,int &hei,const wchar_t wStr[]) const=0;
};

////////////////////////////////////////////////////////////

/*! This class is a base class for FsGuiCanvas, FsGuiPopUpmenu, and FsGuiDialog.
    */
class FsGuiControlBase : public FsGuiObject
{
private:
	FsGuiControlBase *parent;
	class FsGuiDialog *modalDlg;

protected:
	YsArray <FsGuiControlBase *> scheduledForDeletion;

public:
	/*! Default constructor. */
	FsGuiControlBase();

	/*! Default destructor.  This needs to be virtual becasue some sub-classes may be deleted from the FsGuiControlBase pointer. */ 
	virtual ~FsGuiControlBase();

	/*! This function sets a pointer to the parent control-base object. */
	void SetParent(FsGuiControlBase *parent);

	/*! This function returns a pointer to the parent control-base object. */
	FsGuiControlBase *GetParent(void) const;

	/*! This function attaches a pointer to the modal dialog. Giving NULL to modalDlg will detach the dialog. */
	void AttachModalDialog(class FsGuiDialog *modalDlg);

	/*! This function detaches the modal dialog.  Same effect as AttachModalDialog(NULL);. */
	void DetachModalDialog(void);

	/*! This function returns a pointer to the modal dialog attached to this control-base object. */
	class FsGuiDialog *GetModalDialog(void) const;

	/*! This function is called when a modal dialog that belongs to this object has been closed. */
    virtual void OnModalDialogClosed(int dialogIdent,class FsGuiDialog *closedModalDialog,int modalDialogEndCode);

	/*! Override this function to implement behavior of the Save button of the View-control dialog. 
	    (A tunnel for 3D-enabled applications.) */
	virtual YSRESULT OnSaveView(const class FsGui3DViewControlDialog *dlg);

	/*! Override this function to implement behavior of the Loae button of the View-control dialog.
	    (A tunnel for 3D-enabled applications.) */
	virtual YSRESULT OnLoadView(class FsGui3DViewControlDialog *dlg);

	/*! The framework (main loop) must call this function regularly. */
	virtual void Interval(void);

	/*! This function must return YSTRUE when a content of the object needs to be re-drawn. */
	virtual YSBOOL NeedRedraw(void) const=0;
	// Memo to myself: The condition for NeedRedraw may also depend on the child objects.  Therefore, it needs to be implemented per class.

	/*! This function sets need-redraw flag. */
	virtual void SetNeedRedraw(YSBOOL needRedraw)=0;

	/*! This function delets FsGuiControlBase sub-class objects that are scheduled for deletion. */
	void PerformScheduledDeletion(void);
};


/* } */
#endif
