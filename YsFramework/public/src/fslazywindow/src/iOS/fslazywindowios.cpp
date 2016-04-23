#include <fssimplewindow.h>
#include "fslazywindowios.h"


static decltype(FsSubSecondTimer()) prevT=0;


void FsIOSBeforeEverything(void)
{
	char progStr[]="program.exe";
	char *dummyArgs[]=
	{
		progStr
	};
	FsLazyWindowApplicationBase::GetApplication()->BeforeEverything(1,dummyArgs);
}


void FsIOSGetWindowSizeAndPreference(int *x0,int *y0,int *wid,int *hei)
{
    FsOpenWindowOption opt;
    FsLazyWindowApplicationBase::GetApplication()->GetOpenWindowOption(opt);
    *x0=opt.x0;
    *y0=opt.y0;
    *wid=opt.wid;
    *hei=opt.hei;
}


void FsIOSInitialize(void)
{
	char progStr[]="program.exe";
	char *dummyArgs[]=
	{
		progStr
	};
	FsLazyWindowApplicationBase::GetApplication()->Initialize(1,dummyArgs);
}


void FsIOSCallInterval(void)
{
	FsLazyWindowApplicationBase::GetApplication()->Interval();

	auto t=FsSubSecondTimer();
	auto passed=t-prevT;

	auto sleepMS=FsLazyWindowApplicationBase::GetApplication()->GetMinimumSleepPerInterval();
	if(sleepMS>passed)
	{
		FsSleep((int)sleepMS-(int)passed);
	}

	prevT=t;
}
void FsIOSDraw(void)
{
	FsLazyWindowApplicationBase::GetApplication()->Draw();
}


int FsIOSNeedRedraw(void)
{
	if(0!=FsCheckWindowExposure() ||
	   true==FsLazyWindowApplicationBase::GetApplication()->NeedRedraw())
	{
		return 1;
	}
	return 0;
}


void FsIOSBeforeTerminate(void)
{
	FsLazyWindowApplicationBase::GetApplication()->BeforeTerminate();
}



