#include <Cocoa/Cocoa.h>

extern void YsMacOSXGetFileListCallBack(void *objPointer,const char fn[],int isDirectory,unsigned long long fileSize);

int YsMacOSXGetFileList(void *objPointer,const char dir[],int errCode,int okCode)
{
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	NSFileManager *fileManager=[[NSFileManager alloc] init];
	NSString *atPath=[[NSString alloc] initWithUTF8String:dir];
	NSArray *fileList=[fileManager contentsOfDirectoryAtPath:atPath error:nil];

	int ret=okCode;

	if(nil!=fileList)
	{
		int i;
		for(i=0; i<[fileList count]; i++)
		{
			NSString *str=[fileList objectAtIndex:i];

			NSString *fulPath=[atPath stringByAppendingPathComponent:str];
			BOOL isDir;
			[fileManager fileExistsAtPath:fulPath isDirectory:&isDir];

			NSDictionary *attrib=[fileManager attributesOfItemAtPath:fulPath error:nil];
			unsigned long long fileSize=(TRUE==isDir ? 0 : [attrib fileSize]);

			YsMacOSXGetFileListCallBack(objPointer,[str UTF8String],(TRUE==isDir ? 1 : 0),fileSize);
		}
	}
	else
	{
		ret=errCode;
	}

	[atPath release];
	[fileManager release];
	[pool release];

	return ret;
}


