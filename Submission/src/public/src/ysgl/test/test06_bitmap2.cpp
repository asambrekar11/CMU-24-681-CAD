/* ////////////////////////////////////////////////////////////

File Name: test06_bitmap2.cpp
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
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <fssimplewindow.h>
#include <ysgl.h>
#include <ysbitmap.h>


#ifndef _WIN32
extern "C" void InspectOpenGLVersionAndExtension(void)
{
	const char *openGlVersion=(const char *)glGetString(GL_VERSION);
	if(NULL!=openGlVersion)
	{
		printf("OpenGL Version=[%s]\n",openGlVersion);
	}
	else
	{
		printf("Cannot retrieve OpenGL version.\n");
	}
}
#endif

int main(void)
{
	int key;
	struct YsGLSLBitmapRenderer *bitmapRenderer;
	YsBitmap bmp;

	FsChangeToProgramDir();

	bmp.LoadPng("flash01.png");
	printf("Bitmap dimension %dx%d\n",bmp.GetWidth(),bmp.GetHeight());

	FsOpenWindow(32,32,640,480,1);

	bitmapRenderer=YsGLSLCreateBitmapRenderer();

	key=FSKEY_NULL;
	while(FSKEY_ESC!=key)
	{
		FsPollDevice();

		key=FsInkey();

		int mx,my,lb,mb,rb;
		FsGetMouseState(lb,mb,rb,mx,my);

		int wid,hei;
		FsGetWindowSize(wid,hei);

		glViewport(0,0,wid,hei);

		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glColor3ub(255,255,255);
		glBegin(GL_LINES);
		glVertex2i(270,240);
		glVertex2i(370,240);
		glVertex2i(320,190);
		glVertex2i(320,290);
		glEnd();

		YsGLSLUseBitmapRenderer(bitmapRenderer);
		YsGLSLRenderRGBABitmap2D(bitmapRenderer,320,240,YSGLSL_HALIGN_RIGHT,YSGLSL_VALIGN_BOTTOM,bmp.GetWidth(),bmp.GetHeight(),bmp.GetRGBABitmapPointer());
		YsGLSLEndUseBitmapRenderer(bitmapRenderer);

		FsSwapBuffers();

		FsSleep(10);
	}

	YsGLSLDeleteBitmapRenderer(bitmapRenderer);

	return 0;
}
