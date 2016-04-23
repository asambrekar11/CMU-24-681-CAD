#import <Cocoa/Cocoa.h>

double YsSubSecTimer_GetSecond(void)
{
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];

	NSTimeInterval curTime=[NSDate timeIntervalSinceReferenceDate]; // Reference Date is Jan 1 2001
	const double curTimeD=(double)curTime;

	[pool release];

	return curTimeD;
}

void YsSubSecTimer_SleepMilliSecond(unsigned int ms)
{
	if(ms>0)
	{
		double sec;
		sec=(double)ms/1000.0;
		[NSThread sleepForTimeInterval:sec];
	}
}

void YsSubSecTimer_SleepSecond(const double sec)
{
	if(0.0<sec)
	{
		[NSThread sleepForTimeInterval:sec];
	}
}

