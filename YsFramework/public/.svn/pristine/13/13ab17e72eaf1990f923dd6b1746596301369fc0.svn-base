#include "../fssimplewindow.h"

#import <UIKit/UIKit.h>
#import <UIKit/UIView.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>


static UIView *viewCache=NULL;

void FsIOSReportUIViewPointer(UIView *view)
{
	viewCache=view;
}

unsigned int FsIOSCreateDepthBuffer(int backingWidth,int backingHeight)
{
	GLuint depthRenderbuffer;
	
	glGenRenderbuffers(1,&depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return 0;
    }

	return depthRenderbuffer;
}

void FsIOSDeleteDepthBuffer(unsigned int depthBuffer)
{
	if (depthBuffer)
	{
		GLuint bufferIdArray[1]={(GLuint)depthBuffer};
		glDeleteRenderbuffers(1,bufferIdArray);
	}
}



void FsChangeToProgramDirC(void)
{
	NSString *path;
	path=[[NSBundle mainBundle] bundlePath];
	printf("BundlePath:%s\n",[path UTF8String]);
	
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:path];
}

void FsSleepC(int ms)
{
	if(ms>0)
	{
		double sec;
		sec=(double)ms/1000.0;
		[NSThread sleepForTimeInterval:sec];
	}
}

long long int FsSubSecondTimerC(void)
{
	static int first=1;
	static NSTimeInterval t0=0.0;
	if(0!=first)
	{
		t0=[[NSDate date] timeIntervalSince1970];
		first=0;
	}


	long long int ms;

#if !__has_feature(objc_arc)
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
#endif

	NSTimeInterval now;
	now=[[NSDate date] timeIntervalSince1970];

	NSTimeInterval passed;
	passed=now-t0;
	ms=(long long int)(1000.0*passed);

#if !__has_feature(objc_arc)
	[pool release];	
#endif

	return ms;
}

long long int FsPassedTimeC(void)
{
	int ms;
	
#if !__has_feature(objc_arc)
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
#endif
	
	static NSTimeInterval last=0.0;
	NSTimeInterval now;
	
	now=[[NSDate date] timeIntervalSince1970];
	
	NSTimeInterval passed;
	passed=now-last;
	ms=(int)(1000.0*passed);
	
	if(ms<0)
	{
		ms=1;
	}
	last=now;
	
#if !__has_feature(objc_arc)
	[pool release];	
#endif

	static bool first=true;
	if(true==first)
	{
		first=false;
		ms=1;
	}

	return ms;
}

void FsSwapBuffersC(void)
{
	if(NULL!=viewCache)
	{
		// [[viewCache view] presentFramebuffer];
	}
}

#define FS_MAX_NUM_TOUCH 16
static int nTouchCache=0;
static int touchCache[FS_MAX_NUM_TOUCH];

void FsIOSReportCurrentTouch(int nTouch,const int touchCoord[])
{
	int i;
	nTouchCache=0;
	for(i=0; i<FS_MAX_NUM_TOUCH && i<nTouch; ++i)
	{
		touchCache[i*2  ]=touchCoord[i*2];
		touchCache[i*2+1]=touchCoord[i*2+1];
		++nTouchCache;
	}
}

int FsGetNumCurrentTouchC(void)
{
	return nTouchCache;
}

const int *FsGetCurrentTouchC(void)
{
	return touchCache;
}

