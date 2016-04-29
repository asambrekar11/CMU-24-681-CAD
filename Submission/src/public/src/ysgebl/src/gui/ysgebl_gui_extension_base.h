/* ////////////////////////////////////////////////////////////

File Name: ysgebl_gui_extension_base.h
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

#ifndef POLYCRE_GUI_EXTENSIONBASE_IS_INCLUDED
#define POLYCRE_GUI_EXTENSIONBASE_IS_INCLUDED
/* { */

#include <ysclass.h>

class GeblGuiExtensionBase
{
protected:
	class GeblGuiEditorBase *canvasPtr;

public:
	GeblGuiExtensionBase(class GeblGuiEditorBase &canvas)
	{
		canvasPtr=&canvas;
	}
	virtual ~GeblGuiExtensionBase()
	{
	};

	/*! This function is called when view-control dialog is created.  Return NULL if the standard view-control dialog is needed. */
	virtual class FsGui3DViewControlDialogBase *CreateCustomViewControlDialog(class FsGui3DViewControl &viewCtrl)=0;

	/*! This function is called during creation of default modeless dialogs. */
	virtual void CreateModelessDialog(void)=0;

	/*! This function is called when dialogs under control of GeblGuiExtension needs to be deleted. */
	virtual void DeleteModelessDialog(void)=0;

	/*! This function is called when the main canvas is adding menu items. */
	virtual void AddMenu(class GeblGuiEditorMainMenu *mainMenu)=0;

	virtual YSBOOL DrawBackground(void);

	/*! This function is called when the user presses the ESC key or selected Edit->Clear UI Mode. */
	virtual void OnClearUIMode(void)=0;

	/*! This function is called after the application is set up and just before entering the run loop. */
	virtual void FinalSetUp(void)=0;

	/*! This function is forwarded from OnSelectMenu of GeblGuiEditorBase */
	virtual void OnSelectMenuItem(class FsGuiPopUpMenuItem *){};

	/*! This function is forwarded from OnModalDialogClosed of GeblGuiEditorBase */
	virtual void OnModalDialogClosed(int dialogIdent,class FsGuiDialog *closedModalDialog,int modalDialogEndCode){};
};

/*! Must be implemented in the specific derivative of Polygon Crest. */
GeblGuiExtensionBase *PolyCreCreateGuiExtension(class GeblGuiEditorBase &canvas);

/*! Must be implemented in the specific derivative of Polygon Crest. */
void PolyCreDeleteGuiExtension(GeblGuiExtensionBase *ptr);

/*! This function must return the custom font size.  Return 0 to use the default size. 
    This function is outside GeblGuiExtensionBase since this function is needed before
    GeblGuiEditorBase is created. */
int PolyCreGetCustomFontHeight(void);

/* } */
#endif
