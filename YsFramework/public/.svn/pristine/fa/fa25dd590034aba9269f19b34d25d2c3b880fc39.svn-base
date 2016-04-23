/* ////////////////////////////////////////////////////////////

File Name: ystextstream.h
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

#ifndef YSTEXTSTREAM_IS_INCLUDED
#define YSTEXTSTREAM_IS_INCLUDED
/* { */

#include "ysstring.h"
#include "yslist2.h"

class YsTextStream
{
};

class YsTextInputStream : public YsTextStream
{
public:
	virtual const char *Gets(YsString &str)=0;
};

class YsTextOutputStream : public YsTextStream
{
public:
	virtual YSRESULT Puts(const char str[])=0;
	virtual YSRESULT Puts(const YsString &str)=0;
	virtual YSRESULT Printf(const char fmt[],...)=0;
};



class YsTextFileInputStream : public YsTextInputStream
{
private:
	FILE *fp;
public:
	YsTextFileInputStream(FILE *fp);
	const char *Gets(YsString &str);
};

class YsTextFileOutputStream : public YsTextOutputStream
{
private:
	FILE *fp;
public:
	YsTextFileOutputStream(FILE *fp);
	YSRESULT Puts(const char str[]);
	YSRESULT Puts(const YsString &str);
	YSRESULT Printf(const char fmt[],...);
};




class YsTextMemoryOutputStream : public YsTextOutputStream, public YsSegmentedArray <YsString,8>
{
private:
	YSSIZE_T curLineIdx;

	YsString &GetCurrentLine(void);
	YsString &NewLine(void);

public:
	YsTextMemoryOutputStream(void);
	void CleanUp(void);
	YSRESULT Puts(const char str[]);
	YSRESULT Puts(const YsString &str);
	YSRESULT Printf(const char fmt[],...);

	const YSSIZE_T GetNumLine(void) const;
};



class YsTextMemoryInputStream : public YsTextInputStream
{
private:
	enum MODE
	{
		MODE_NONE,
		MODE_CSTRING_ARRAY
	};

	MODE mode;
	YSSIZE_T ptr;
	const char *const *inStr;

public:
	/*! Create from a NULL-terminated array of C-strings. */
	YsTextMemoryInputStream(const char *const inStr[]);

	virtual const char *Gets(YsString &str);
};


/* } */
#endif
