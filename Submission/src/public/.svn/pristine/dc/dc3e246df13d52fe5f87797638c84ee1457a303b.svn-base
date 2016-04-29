/* ////////////////////////////////////////////////////////////

File Name: ysbitmap.h
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

#ifndef YSBITMAP_IS_INCLUDED
#define YSBITMAP_IS_INCLUDED
/* { */

#include <stdio.h>

#ifndef YSRESULT_IS_DEFINED
#define YSRESULT_IS_DEFINED
typedef enum
{
	YSERR,
	YSOK
} YSRESULT;
#endif

#ifndef YSBOOL_IS_DEFINED
#define YSBOOL_IS_DEFINED
/*! Enum for boolearn. */
typedef enum
{
	YSFALSE,     /*!< False */
	YSTRUE,      /*!< True */
	YSTFUNKNOWN  /*!< Unable to tell true or false. */
} YSBOOL;
#endif

class YsBitmap
{
private:
	int nx,ny;
	unsigned char *dat;
	unsigned char **lineTop;

public:
	YsBitmap();
	~YsBitmap();

	/*! Default copy constructor. */
	YsBitmap(const YsBitmap &incoming);

	/*! Clears the bitmap.  This bitmap will become empty. */
	void CleanUp(void);

	/*! Default copy operator. */
	YsBitmap &operator=(const YsBitmap &from);

	/*! Copies the incoming bitmap to this bitmap. */
	YsBitmap &CopyFrom(const YsBitmap &incoming);

	/*! Move-assignment constructor. */
	YsBitmap(YsBitmap &&incoming);

	/*! Move-assignment operator. */
	YsBitmap &operator=(YsBitmap &&incoming);

	/*! Move incoming bitmap to this bitmap.  Incoming bitmap will be cleared. */
	YsBitmap &MoveFrom(YsBitmap &incoming);

	YSRESULT LoadBmp(const char fn[]);
	/*! fp must be opened with "rb". */
	YSRESULT LoadBmp(FILE *fp);
	YSRESULT LoadPng(const char fn[]);
	/*! fp must be opened with "rb". */
	YSRESULT LoadPng(FILE *fp);

	/*! Load PNG-format data from memory. */
	YSRESULT LoadPng(size_t nByte,const unsigned char binaryData[]);

	/*! Move-assignment operator from a YsRawPngDecoder object. */
	YsBitmap &operator=(class YsRawPngDecoder &&pngDecoder);

	/*! Move bitmap from YsRawPngDecoder. */
	YsBitmap &MoveFrom(class YsRawPngDecoder &pngDecoder);

public:
	YSRESULT PrepareBitmap(int wid,int hei);
	void Clear(unsigned char r,unsigned char g,unsigned char b,unsigned char a);
	YSRESULT ScaleCopy(int wid,int hei,const YsBitmap &from);
	YSRESULT ScaleCopyRaw(int wid,int hei,const YsBitmap &from);
	YSRESULT Copy(const YsBitmap &from,int x0,int y0);

	/*! Copy from the bitmap incomingBmp starting from (inX0,inY0 to this bitmap from (toX0,toY0) 
	    for the wid pixels horizontally and hei pixels vertically. */
	YSRESULT Copy(const YsBitmap &incomingBmp,int inX0,int inY0,int toX0,int toY0,int wid,int hei);
	void Invert(void);
	void NegateRGB(void);
	void NegateRGBA(void);

	void SetDirect(int wid,int hei,unsigned char *rgba);  // Buffer rgba will be managed by this object.

	YSRESULT InsecureTransfer(const YsBitmap &from,int x0,int y0);  // Incoming bmp must fit in this bmp.
	YSRESULT Transfer(const YsBitmap &from,int x0,int y0,unsigned int flags);
	// Flags:
	//    1: Horizontal Mirror
	//    2: Vertical Mirror
	//    4: Transpose
	//  128: Use Alpha
	YSRESULT VerticalScroll(int dy,unsigned char r,unsigned char g,unsigned char b,unsigned char a);

	YSRESULT SetR(int x,int y,unsigned char r);
	YSRESULT SetG(int x,int y,unsigned char g);
	YSRESULT SetB(int x,int y,unsigned char b);
	YSRESULT SetA(int x,int y,unsigned char a);
	YSRESULT SetRGBA(int x,int y,unsigned char r,unsigned char g,unsigned char b,unsigned char a);

	int GetWidth(void) const;
	int GetHeight(void) const;
	const unsigned char *GetRGBABitmapPointer(void) const;
	const unsigned char *GetRGBAPixelPointer(int x,int y) const;
	unsigned char *GetEditableRGBABitmapPointer(void);
	unsigned char *GetEditableRGBAPixelPointer(int x,int y);

	YSRESULT SaveBmp(const char fn[]) const;
	YSRESULT SaveBlackAndWhiteBmp(const char fn[],const int thr,YSBOOL invert) const;

	YSBOOL IsInRange(int x,int y) const;
	YSBOOL IsXInRange(int x) const;
	YSBOOL IsYInRange(int y) const;

protected:
	unsigned GetIntelUInt(const unsigned char dat[]);
	unsigned GetIntelUShort(const unsigned char dat[]);
	unsigned char *GetTopOfLine(int y) const;
};


/* } */
#endif
