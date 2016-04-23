#include <stdio.h>
#include "ysglfontdata.h"

const unsigned char * const *YsGlSelectFontBitmapPointerByHeight(int *selectedWidth,int *selectedHeight,int fontHeight)
{
	if(7>=fontHeight)
	{
		*selectedWidth=6;
		*selectedHeight=7;
		return YsFont6x7;
	}
	else if(8>=fontHeight)
	{
		*selectedWidth=6;
		*selectedHeight=8;
		return YsFont6x8;
	}
	else if(10>=fontHeight)
	{
		*selectedWidth=7;
		*selectedHeight=10;
		return YsFont7x10;
	}
	else if(12>=fontHeight)
	{
		*selectedWidth=8;
		*selectedHeight=12;
		return YsFont8x12;
	}
	else if(14>=fontHeight)
	{
		*selectedWidth=10;
		*selectedHeight=14;
		return YsFont10x14;
	}
	else if(16>=fontHeight)
	{
		*selectedWidth=12;
		*selectedHeight=16;
		return YsFont12x16;
	}
	else if(20>=fontHeight)
	{
		*selectedWidth=16;
		*selectedHeight=20;
		return YsFont16x20;
	}
	else if(24>=fontHeight)
	{
		*selectedWidth=16;
		*selectedHeight=24;
		return YsFont16x24;
	}
	else if(28>=fontHeight)
	{
		*selectedWidth=20;
		*selectedHeight=28;
		return YsFont20x28;
	}
	else if(32>=fontHeight)
	{
		*selectedWidth=20;
		*selectedHeight=32;
		return YsFont20x32;
	}
	else if(40>=fontHeight)
	{
		*selectedWidth=24;
		*selectedHeight=40;
		return YsFont24x40;
	}
	else if(44>=fontHeight)
	{
		*selectedWidth=28;
		*selectedHeight=44;
		return YsFont28x44;
	}
	else
	{
		*selectedWidth=32;
		*selectedHeight=48;
		return YsFont32x48;
	}
	// *selectedWidth=0;
	// *selectedHeight=0;
	// return NULL;
}

