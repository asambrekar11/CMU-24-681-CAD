/* ////////////////////////////////////////////////////////////

File Name: ysshellext_triangulationutil.h
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

#ifndef YSSHELLEXT_TRIANGULATIONUTIL_IS_INCLUDED
#define YSSHELLEXT_TRIANGULATIONUTIL_IS_INCLUDED
/* { */

#include <ysshellext.h>

#include "ysshellext_localop.h"



class YsShellExt_TriangulationInfo
{
public:
	class Triangulation
	{
	public:
		YsShellPolygonHandle plHd;
		YsArray <YsShellVertexHandle> triVtHd;  // triangle[i]={triVtHd[i*3],triVtHd[i*3+1],triVtHd[i*3+2]}
	};
	YsArray <Triangulation> tri;
	YsArray <YsShellPolygonHandle> failedPlHd;

	class ThreadInfo;
	static void ThreadEntry(ThreadInfo *infoPtr);

	void CleanUp(void);
	void MakeInfo(const YsShellExt &shl,int nThread=8);
};


/* } */
#endif
