// Compiled as :
// cl /FIwindows.h auxsample.c ysglusefontbitmap.c ysglfontdata.c kernel32.lib user32.lib gdi32.lib advapi32.lib opengl32.lib glu32.lib glaux.lib

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>

#include "ysglfontdata.h"

int main(int ac,char *av[])
{
	int viewport[4],wid,hei;
	const char *str;


	auxInitDisplayMode(AUX_SINGLE|AUX_RGB);
	auxInitPosition(0,0,640,480);
	auxInitWindow(NULL);


	YsGlUseFontBitmap6x7(0x400);
	YsGlUseFontBitmap6x8(0x500);
	YsGlUseFontBitmap8x8(0x600);
	YsGlUseFontBitmap8x12(0x700);
	YsGlUseFontBitmap12x16(0x800);
	YsGlUseFontBitmap16x20(0x900);
	YsGlUseFontBitmap16x24(0xA00);
	YsGlUseFontBitmap20x28(0xB00);
	YsGlUseFontBitmap20x32(0xC00);


	glViewport(0,0,640,480);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


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

	Sleep(15000);


	return 0;
}
