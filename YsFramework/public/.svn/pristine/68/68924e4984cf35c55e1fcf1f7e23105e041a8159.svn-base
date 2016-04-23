#include "fslazywindow.h"

FsLazyWindowApplicationBase::FsLazyWindowApplicationBase()
{
	terminateFlag=false;
}

/* virtual */ bool FsLazyWindowApplicationBase::MustTerminate(void) const
{
	return terminateFlag;
}

/* virtual */ long long int FsLazyWindowApplicationBase::GetMinimumSleepPerInterval(void) const
{
	return 10;
}

/* virtual */ bool FsLazyWindowApplicationBase::UserWantToCloseProgram(void)
{
	return true;
}

void FsLazyWindowApplicationBase::SetMustTerminate(bool MustTerminate)
{
	terminateFlag=MustTerminate;
}
