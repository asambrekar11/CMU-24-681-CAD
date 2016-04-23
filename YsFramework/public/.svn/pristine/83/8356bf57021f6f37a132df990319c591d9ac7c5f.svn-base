#include <Cocoa/Cocoa.h>

int FsGuiCopyStringToClipBoardC(const char cstr[])
{
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

	NSPasteboard *pasteboard=[NSPasteboard generalPasteboard];

	[pasteboard clearContents];


	NSString *str=[[NSString alloc] initWithUTF8String:cstr];

	NSMutableArray *toCopy=[[NSMutableArray alloc] init];
	[toCopy addObject:str];

	BOOL res=[pasteboard writeObjects:toCopy];

	[str release];

	[pool release];

	return 1;
}



int FsGuiGetStringLengthInClipBoardC(void)
{
	int n=0;

	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

	NSPasteboard *pasteboard=[NSPasteboard generalPasteboard];

	NSArray *classArray=[[NSMutableArray alloc] initWithObjects:[NSString class],nil];
	NSDictionary *options=[NSDictionary dictionary];

	NSArray *clipboardContent=[pasteboard readObjectsForClasses:classArray options:options];

	if(nil!=clipboardContent)
	{
		if(0<[clipboardContent count])
		{
			NSString *str=[clipboardContent objectAtIndex:0];
			if(NULL!=str)
			{
				n=strlen([str UTF8String]);
			}
		}
	}

	[classArray release];

	[pool release];

	return n;
}

int FsGuiCopyStringFromClipBoardC(char cstr[])
{
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

	NSPasteboard *pasteboard=[NSPasteboard generalPasteboard];

	NSArray *classArray=[[NSMutableArray alloc] initWithObjects:[NSString class],nil];
	NSDictionary *options=[NSDictionary dictionary];

	NSArray *clipboardContent=[pasteboard readObjectsForClasses:classArray options:options];

	if(nil!=clipboardContent)
	{
		if(0<[clipboardContent count])
		{
			NSString *str=[clipboardContent objectAtIndex:0];
			if(NULL!=str)
			{
				strcpy(cstr,[str UTF8String]);
			}
		}
	}

	[classArray release];

	[pool release];

	return 1;
}
