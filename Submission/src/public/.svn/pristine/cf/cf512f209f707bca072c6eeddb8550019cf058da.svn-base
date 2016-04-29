#import <Cocoa/Cocoa.h>
#import <mach-o/dyld.h>

bool YsSpecialPath_GetProgramFileName(size_t lBuf,char buf[])
{
	if(nil!=[NSBundle mainBundle])
	{
	 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
		strncpy(buf,[[[NSBundle mainBundle] executablePath] UTF8String],lBuf);
		[pool release];
		buf[lBuf-1]=0;
	}
	else
	{
		_NSGetExecutablePath(buf,lBuf);
	}
	return buf;
}

bool YsSpecialPath_GetProgramBaseDir(size_t lBuf,char buf[])
{
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	strncpy(buf,[[[NSBundle mainBundle] resourcePath] UTF8String],lBuf); // 2015/11/30 Changed from bundlePath to resourcePath
	[pool release];
	buf[lBuf-1]=0;
	return buf;
}

bool YsSpecialPath_GetUserDir(size_t lBuf,char buf[])
{
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	strncpy(buf,[NSHomeDirectory() UTF8String],lBuf);
	[pool release];

	buf[lBuf-1]=0;
	return true;
}

bool YsSpecialPath_GetAllUsersDir(size_t lBuf,char buf[])
{
	return false;
}

