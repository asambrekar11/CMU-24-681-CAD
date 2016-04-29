#import <UIKit/UIKit.h>

double YsSubSecTimer_GetSecond(void)
{
	NSTimeInterval curTime=[NSDate timeIntervalSinceReferenceDate]; // Reference Date is Jan 1 2001
	const double curTimeD=(double)curTime;

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

