/* ////////////////////////////////////////////////////////////

File Name: fssimplenowindow.cpp
Copyright (c) 2015 Soji Yamakawa.  All rights reserved.
http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>
#include <string.h>

#ifdef _WIN32
	#include <windows.h>
	#include <direct.h>
#elif defined(__APPLE__)
	#include <unistd.h>
#endif

#include "fssimplewindow.h"





class FsMouseEventLog
{
public:
	int eventType;
	int lb,mb,rb;
	int mx,my;
	unsigned int shift,ctrl;
};


#define NKEYBUF 256
static int keyBuffer[NKEYBUF];
static int nKeyBufUsed=0;
static int charBuffer[NKEYBUF];
static int nCharBufUsed=0;
static int nMosBufUsed=0;
static FsMouseEventLog mosBuffer[NKEYBUF];
static int exposure=0;



void FsOpenWindow(const FsOpenWindowOption &)
{
	if(NULL!=fsAfterWindowCreationCallBack)
	{
		(*fsAfterWindowCreationCallBack)(fsAfterWindowCreationCallBackParam);
	}
}

void FsCloseWindow(void)
{
}

void FsMaximizeWindow(void)
{
}

void FsUnmaximizeWindow(void)
{
}
void FsMakeFullScreen(void)
{
}

void FsGetWindowSize(int &wid,int &hei)
{
	wid=16;
	hei=16;
}

void FsGetWindowPosition(int &x0,int &y0)
{
	x0=0;
	y0=0;
}

void FsSetWindowTitle(const char windowTitle[])
{
	printf("%s\n",windowTitle);
}

void FsPollDevice(void)
{
	static bool busy=false;
	if(true!=busy)
	{
		busy=true;
		if(NULL!=fsPollDeviceHook)
		{
			(*fsPollDeviceHook)(fsPollDeviceHookParam);
		}
		busy=false;
	}
}

void FsPushOnPaintEvent(void)
{
}

void FsSleep(int ms)
{
#ifdef WIN32
	Sleep(ms);
#else
	fd_set set;
	struct timeval wait;
	wait.tv_sec=ms/1000;
	wait.tv_usec=(ms%1000)*1000;
	FD_ZERO(&set);
	select(0,&set,NULL,NULL,&wait);
#endif
}

long long int FsPassedTime(void)
{
	return 10;
}

long long int FsSubSecondTimer(void)
{
	return time(NULL)*1000;
}

int FsInkey(void)
{
	if(nKeyBufUsed>0)
	{
		int i,keyCode;
		keyCode=keyBuffer[0];
		nKeyBufUsed--;
		for(i=0; i<nKeyBufUsed; i++)
		{
			keyBuffer[i]=keyBuffer[i+1];
		}
		return keyCode;
	}
	return 0;
}

int FsInkeyChar(void)
{
	if(nCharBufUsed>0)
	{
		int i,asciiCode;
		asciiCode=charBuffer[0];
		nCharBufUsed--;
		for(i=0; i<nCharBufUsed; i++)
		{
			charBuffer[i]=charBuffer[i+1];
		}
		return asciiCode;
	}
	return 0;
}

void FsPushKey(int fskey)
{
	if(nKeyBufUsed<NKEYBUF)
	{
		keyBuffer[nKeyBufUsed++]=fskey;
	}
}

void FsPushChar(int c)
{
	if(nCharBufUsed<NKEYBUF)
	{
		charBuffer[nCharBufUsed++]=c;
	}
}

int FsGetKeyState(int fsKeyCode)
{
	return 0;
}

int FsCheckWindowExposure(void)
{
	const int ret=exposure;
	exposure=0;
	return ret;
}

void FsGetMouseState(int &lb,int &mb,int &rb,int &mx,int &my)
{
	lb=0;
	mb=0;
	rb=0;
	mx=0;
	my=0;
}

int FsGetMouseEvent(int &lb,int &mb,int &rb,int &mx,int &my)
{
	lb=0;
	mb=0;
	rb=0;
	mx=0;
	my=0;
	return FSMOUSEEVENT_NONE;
}

void FsSwapBuffers(void)
{
}

void FsChangeToProgramDir(void)
{
}

int FsGetNumCurrentTouch(void)
{
	return 0;
}

const FsVec2i *FsGetCurrentTouch(void)
{
	return nullptr;
}


// The following two dummy functions are for loop-holing fsguilib to fssimplewindow for
// managing clipboard.  There's got to be a better way, but I cannot find so far.
void FsX11GetClipBoardString(long long int &,char *&)
{
}
void FsX11SetClipBoardString(long long int,const char [])
{
}
