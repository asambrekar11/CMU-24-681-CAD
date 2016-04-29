/* gcc glxsample.c ysglfontdata.c ysglusefontbitmap.c /usr/lib/libX11.so /usr/lib/libXext.so.6 -lGLU -lGL -lm */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/Xatom.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>


#include "ysglfontdata.h"


static Display *ysXDsp;
static Window ysXWnd;
static Colormap ysXCMap;
static XVisualInfo *ysXVis;
static const int ysXEventMask=(KeyPress|KeyRelease|ButtonPress|ButtonRelease|ExposureMask|StructureNotifyMask);

static GLXContext ysGlRC;
static int ysGlxCfg[]={/* GLX_DOUBLEBUFFER, */GLX_RGBA,GLX_DEPTH_SIZE,16,None};

int YsOpenWindow(int lupX,int lupY,int sizX,int sizY)
{
	const char *title="GLX Sample";

	int n;
	char **m,*def;
	XSetWindowAttributes swa;
	Font font;

	ysXDsp=XOpenDisplay(NULL);

	if(ysXDsp!=NULL)
	{
		if(glXQueryExtension(ysXDsp,NULL,NULL)!=0)
		{
			ysXVis=glXChooseVisual(ysXDsp,DefaultScreen(ysXDsp),ysGlxCfg);
			if(ysXVis!=NULL)
			{
				ysXCMap=XCreateColormap(ysXDsp,RootWindow(ysXDsp,ysXVis->screen),ysXVis->visual,AllocNone);

				ysGlRC=glXCreateContext(ysXDsp,ysXVis,None,GL_TRUE);
				if(ysGlRC!=NULL)
				{
					swa.colormap=ysXCMap;
					swa.border_pixel=0;
					swa.event_mask=ysXEventMask;

					ysXWnd=XCreateWindow(ysXDsp,RootWindow(ysXDsp,ysXVis->screen),
							  lupX,lupY,sizX,sizY,
					                  0,
							  ysXVis->depth,
					                  InputOutput,
							  ysXVis->visual,
					                  CWEventMask|CWBorderPixel|CWColormap,&swa);


					XStoreName(ysXDsp,ysXWnd,title);

					XWMHints wmHints;
					wmHints.flags=0;
					wmHints.initial_state=NormalState;
					XSetWMHints(ysXDsp,ysXWnd,&wmHints);


					XSetIconName(ysXDsp,ysXWnd,title);
					XMapWindow(ysXDsp,ysXWnd);

					glXMakeCurrent(ysXDsp,ysXWnd,ysGlRC);

					glClearColor(0.0,0.0,0.0,0.0);
					glViewport(0,0,sizX,sizY);
					glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
					glFlush();
					// glXSwapBuffers(ysXDsp,ysXWnd);
				}
				else
				{
					fprintf(stderr,"Cannot create OpenGL context.\n");
					exit(1);
				}
			}
			else
			{
				fprintf(stderr,"Double buffer not supported?\n");
				exit(1);
			}
		}
		else
		{
			fprintf(stderr,"This system doesn't support OpenGL.\n");
			exit(1);
		}
	}
	else
	{
		fprintf(stderr,"Cannot Open Display.\n");
		exit(1);
	}

	return 0;
}

int YsCloseWindow(void)
{
	XCloseDisplay(ysXDsp);
	return 0;
}

void YsSleep(int ms)
{
	fd_set set;
	struct timeval wait;
	wait.tv_sec=ms/1000;
	wait.tv_usec=(ms%1000)*1000;
	FD_ZERO(&set);
	select(0,&set,NULL,NULL,&wait);
}

int main(void)
{
	int viewport[4],wid,hei;
	const char *str;

	YsOpenWindow(32,32,800,600);

	YsGlUseFontBitmap6x7(0x400);
	YsGlUseFontBitmap6x8(0x500);
	YsGlUseFontBitmap8x8(0x600);
	YsGlUseFontBitmap8x12(0x700);
	YsGlUseFontBitmap12x16(0x800);
	YsGlUseFontBitmap16x20(0x900);
	YsGlUseFontBitmap16x24(0xA00);
	YsGlUseFontBitmap20x28(0xB00);
	YsGlUseFontBitmap20x32(0xC00);

	glGetIntegerv(GL_VIEWPORT,viewport);
	wid=viewport[2];
	hei=viewport[3];

	glDepthFunc(GL_ALWAYS);
	glDepthMask(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5,(GLdouble)wid-0.5,(GLdouble)hei-0.5,-0.5,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);


	glListBase(0x400);
	glRasterPos2i(0,7);
	str="6x7 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0x500);
	glRasterPos2i(0,15);
	str="6x8 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0x600);
	glRasterPos2i(0,23);
	str="8x8 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0x700);
	glRasterPos2i(0,35);
	str="8x12 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0x800);
	glRasterPos2i(0,51);
	str="12x16 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0x900);
	glRasterPos2i(0,71);
	str="16x20 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0xA00);
	glRasterPos2i(0,95);
	str="16x24 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0xB00);
	glRasterPos2i(0,123);
	str="20x28 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glListBase(0xC00);
	glRasterPos2i(0,155);
	str="20x32 Font ABCDEFG012345";
	glCallLists(strlen(str),GL_UNSIGNED_BYTE,str);

	glFlush();

	YsSleep(10000);
	YsCloseWindow();

	return 0;
}

