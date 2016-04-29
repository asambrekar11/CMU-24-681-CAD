#import <UIKit/UIKit.h>
#import <mach-o/dyld.h>

bool YsSpecialPath_GetProgramFileName(size_t lBuf,char buf[])
{
	if(nil!=[NSBundle mainBundle])
	{
		strncpy(buf,[[[NSBundle mainBundle] executablePath] UTF8String],lBuf);
		buf[lBuf-1]=0;
	}
	else
	{
		_NSGetExecutablePath(buf,(int)lBuf);
	}
	return buf;
}

bool YsSpecialPath_GetProgramBaseDir(size_t lBuf,char buf[])
{
	strncpy(buf,[[[NSBundle mainBundle] bundlePath] UTF8String],lBuf);
	buf[lBuf-1]=0;
	return buf;
}

bool YsSpecialPath_GetUserDir(size_t lBuf,char buf[])
{
	strncpy(buf,[NSHomeDirectory() UTF8String],lBuf);

	buf[lBuf-1]=0;
	return true;
}

bool YsSpecialPath_GetAllUsersDir(size_t lBuf,char buf[])
{
	return false;
}

