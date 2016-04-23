/* ////////////////////////////////////////////////////////////

File Name: test01_PlainDrawing.cpp
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
#include <fssimplewindow.h>
#include <ysgl.h>

int main(void)
{
	FsOpenWindow(16,16,800,600,1);

	const GLubyte *verStr=glGetString(GL_VERSION);
	if(NULL!=verStr)
	{
		printf("OpenGL Version %s\n",verStr);
	}
	else
	{
		printf("Unknown OpenGL version\n");
	}

	struct YsGLSLPlain2DRenderer *plain2DDrawingPtr=YsGLSLCreatePlain2DRenderer();

	for(int key=FSKEY_NULL; FSKEY_ESC!=key; FsSleep(10))
	{
		FsPollDevice();
		key=FsInkey();

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glViewport(0,0,400,300);

		YsGLSLUsePlain2DRenderer(plain2DDrawingPtr);

		YsGLSLDontUseWindowCoordinateInPlain2DDrawing(plain2DDrawingPtr);
		YsGLSLDrawPlain2DLined(
		    plain2DDrawingPtr,
		    -1.0, 1.0,  // x0,y0
		     1.0,-1.0,  // x1,y1
		     1.0, 0.0, 0.0, 1.0);  // r,g,b,a



		int wid,hei;
		FsGetWindowSize(wid,hei);

		GLfloat triVtx[6]={100,100,200,100,150,200};
		GLfloat triColor[12]={1,0,0,1,  0,0,1,1,  0,1,0,1};


		glViewport(0,0,wid,hei);
		YsGLSLUseWindowCoordinateInPlain2DDrawing(plain2DDrawingPtr,1);

		YsGLSLDrawPlain2DLinef(
		    plain2DDrawingPtr,
		    0,0,300,300,0,0,1,1);

		YsGLSLDrawPlain2DPrimitivefv(plain2DDrawingPtr,GL_TRIANGLES,3,triVtx,triColor);


		YsGLSLUseWindowCoordinateInPlain2DDrawing(plain2DDrawingPtr,0);

		YsGLSLDrawPlain2DLinef(
		    plain2DDrawingPtr,
		    0,0,300,300,0,0,1,1);


		YsGLSLDrawPlain2DPrimitivefv(plain2DDrawingPtr,GL_TRIANGLES,3,triVtx,triColor);

		YsGLSLEndUsePlain2DRenderer(plain2DDrawingPtr);

		FsSwapBuffers();
	}

	YsGLSLDeletePlain2DRenderer(plain2DDrawingPtr);

	return 0;
}
