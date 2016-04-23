#import <Cocoa/Cocoa.h>

void YsLocale_GetLocale(unsigned int nBuf,char buf[])
{
	strncpy(buf,"en-US",nBuf);  // Default to en-US

	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

	NSArray *lang=[NSLocale preferredLanguages];
	if(NULL!=lang)
	{
		NSString *langId=[lang objectAtIndex:0];
		if(NULL!=langId)
		{
			strncpy(buf,[langId UTF8String],nBuf-1);
		}
	}

	[pool release];
}
