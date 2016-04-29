#import <Cocoa/Cocoa.h>

void FsOpenURLC(const char urlStr[])
{
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

	printf("Open Web Site> %s\n",urlStr);

	NSURL *url;
	NSString *urlString;

	urlString=[NSString stringWithUTF8String:urlStr];
	url=[NSURL URLWithString:urlString];

	[[NSWorkspace sharedWorkspace] openURL:url];

	[pool release];
}

