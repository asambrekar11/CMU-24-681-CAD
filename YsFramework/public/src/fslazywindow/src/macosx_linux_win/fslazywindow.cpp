#include <fssimplewindow.h>

#include "../fslazywindow.h"


static void IntervalCallBack(void *)
{
	auto appPtr=FsLazyWindowApplicationBase::GetApplication();
	if(nullptr!=appPtr)
	{
		appPtr->Interval();
	}
}

static void NeedRedrawCallBack(void *)
{
	auto appPtr=FsLazyWindowApplicationBase::GetApplication();
	if(nullptr!=appPtr)
	{
		appPtr->Draw();
	}
}

static bool UserWantToCloseProgram(void *)
{
	auto appPtr=FsLazyWindowApplicationBase::GetApplication();
	if(nullptr!=appPtr)
	{
		return FsLazyWindowApplicationBase::GetApplication()->UserWantToCloseProgram();
	}
}

int main(int ac,char *av[])
{
	FsLazyWindowApplicationBase::GetApplication()->BeforeEverything(ac,av);

	FsOpenWindowOption owo;
	owo.useDoubleBuffer=true;
	FsLazyWindowApplicationBase::GetApplication()->GetOpenWindowOption(owo);

	FsOpenWindow(owo);
	FsLazyWindowApplicationBase::GetApplication()->Initialize(ac,av);


	// Memo: Don't register call backs before Initialize.
	//       Some of the call-back functions may be accidentally fired from inside FsOpenWindow.
	FsRegisterIntervalCallBack(IntervalCallBack,nullptr);
	FsRegisterOnPaintCallBack(NeedRedrawCallBack,nullptr);
	FsRegisterCloseWindowCallBack(UserWantToCloseProgram,nullptr);


	auto t0=FsSubSecondTimer();
	while(0!=FsCheckWindowOpen() &&
	      true!=FsLazyWindowApplicationBase::GetApplication()->MustTerminate())
	{
		auto t=FsSubSecondTimer();
		auto passed=t-t0;
		t0=t;

		FsPollDevice();
		FsLazyWindowApplicationBase::GetApplication()->Interval();

		if(0!=FsCheckWindowExposure() ||
		   true==FsLazyWindowApplicationBase::GetApplication()->NeedRedraw())
		{
			FsLazyWindowApplicationBase::GetApplication()->Draw();
		}

		auto sleepMS=FsLazyWindowApplicationBase::GetApplication()->GetMinimumSleepPerInterval();
		if(sleepMS>passed)
		{
			FsSleep(sleepMS-(int)passed);
		}
	}

	FsLazyWindowApplicationBase::GetApplication()->BeforeTerminate();

	FsCloseWindow();

	return 0;
}

#ifdef _WIN32
// Infamouse WinMain function.  This function will be used when SUBSYSTEM is set to WINDOWS.
#include <windows.h>
int PASCAL WinMain(HINSTANCE inst,HINSTANCE,LPSTR param,int)
{
	char *argv[]=
	{
		"something.exe",
		param
	};
	return main(2,argv);
}
#endif

