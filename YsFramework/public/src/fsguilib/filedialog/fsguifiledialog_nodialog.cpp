#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>


#include <ysclass.h>
#include <ysbitmap.h>
#include <fsgui.h>
#include <fssimplewindow.h>

#include "fsguifiledialog.h"



class FsGuiFileDialog_PlatformSpecificData : public FsGuiDialog
{
public:
	FsGuiButton *okBtn;

	void Make(void);
	virtual void OnButtonClick(FsGuiButton *btn);
};

void FsGuiFileDialog_PlatformSpecificData::Make(void)
{
	FsGuiDialog::Initialize();
	AddStaticText(0,FSKEY_NULL,L"This platform does not support a file dialog.",YSTRUE);
	okBtn=AddTextButton(0,FSKEY_ENTER,FSGUI_PUSHBUTTON,L"OK",YSTRUE);
	Fit();
}

/* virtual */ void FsGuiFileDialog_PlatformSpecificData::OnButtonClick(FsGuiButton *btn)
{
	CloseModalDialog((int)YSOK);
}

////////////////////////////////////////////////////////////

void FsGuiFileDialog::CloseModalCallBackFunc(class FsGuiDialog *,int returnCode)
{
	this->busy=YSFALSE;
	this->CloseModalDialog(returnCode);
}

void FsGuiFileDialog::Show(const FsGuiDialog *excluded) const
{
	FsGuiDialog::Show(excluded);

	if(YSTRUE!=busy)
	{
		busy=YSTRUE;

		FsGuiFileDialog *nonConst=(FsGuiFileDialog *)this;

		param->Make();
		param->BindCloseModalCallBack(&THISCLASS::CloseModalCallBackFunc,nonConst);

		nonConst->SetBackgroundAlpha(0.1);

		nonConst->AttachModalDialog(this->param);
	}
}

void FsGuiFileDialog::AllocatePlatformSpecificData(void)
{
	param=new FsGuiFileDialog_PlatformSpecificData;
}

void FsGuiFileDialog::DeletePlatformSpecificData(void)
{
	delete param;
}

/* virtual */ void FsGuiFileDialog::OnAttach(void)
{
}
