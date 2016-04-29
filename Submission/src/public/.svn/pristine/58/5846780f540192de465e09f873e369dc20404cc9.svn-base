/* ////////////////////////////////////////////////////////////

File Name: ysshellext_patchutil.h
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

#ifndef YSSHELLEXT_PATCHUTIL_IS_INCLUDED
#define YSSHELLEXT_PATCHUTIL_IS_INCLUDED
/* { */

#include <ysshellext.h>

class YsShellExt_PatchBetweenTwoConstEdge
{
public:
	enum PATCHSTRATEGY
	{
		UNIFORM_PARAMETER
	};

	YsShellExt shl;
	YsHashTable <YsShellVertexHandle> patchVtKeyToSrcVtHd;
	YsHashTable <int> patchVtKeyToSeqId;

	YsArray <YsShellVertexHandle> rawVtHdSeq[2],vtHdSeq[2];
	YsArray <double> paramArray[2];
	YSBOOL isLoop[2];

	YsShellExt::ConstEdgeHandle seqCeHd[2];

	double totalLen[2];


	YsShellExt_PatchBetweenTwoConstEdge();
	~YsShellExt_PatchBetweenTwoConstEdge();

	void CleanUp(void);
	YSRESULT SetVertexSequence(const YsShell &srcShl,
	                       YSSIZE_T nVt0,const YsShellVertexHandle vtHdArray0[],YSBOOL isLoop0,
	                       YSSIZE_T nVt1,const YsShellVertexHandle vtHdArray1[],YSBOOL isLoop1);

	template <const int N>
	YSRESULT SetVertexSequence(const YsShell &srcShl,
	                       const YsArray <YsShellVertexHandle,N> &vtHdArray0,YSBOOL isLoop0,
	                       const YsArray <YsShellVertexHandle,N> &vtHdArray1,YSBOOL isLoop1);

	template <const int N>
	YSRESULT SetVertexSequence(const YsShell &srcShl,
	                       const YsArray <YsShellVertexHandle,N> vtHdArray[2],const YSBOOL isLoop[2]);

	void CalculateParameter(void);
	void MakeInitialGuess(void);
	void MinimizeDihedralAngleSum(void);

	template <const int N>
	void GetPolygonVertexOfOriginalShell(YsArray <YsShellVertexHandle,N> &plVtHd,YsVec3 &nom,YsShellPolygonHandle patchPlHd);
};

template <const int N>
YSRESULT YsShellExt_PatchBetweenTwoConstEdge::SetVertexSequence(const YsShell &srcShl,
                       const YsArray <YsShellVertexHandle,N> &vtHdArray0,YSBOOL isLoop0,
                       const YsArray <YsShellVertexHandle,N> &vtHdArray1,YSBOOL isLoop1)
{
	return SetVertexSequence(srcShl,vtHdArray0.GetN(),vtHdArray0,isLoop0,vtHdArray1.GetN(),vtHdArray1,isLoop1);
}

template <const int N>
YSRESULT YsShellExt_PatchBetweenTwoConstEdge::SetVertexSequence(const YsShell &srcShl,
                       const YsArray <YsShellVertexHandle,N> vtHdArray[2],const YSBOOL isLoop[2])
{
	return SetVertexSequence(srcShl,vtHdArray[0],isLoop[0],vtHdArray[1],isLoop[1]);
}

template <const int N>
void YsShellExt_PatchBetweenTwoConstEdge::GetPolygonVertexOfOriginalShell(YsArray <YsShellVertexHandle,N> &plVtHd,YsVec3 &nom,YsShellPolygonHandle patchPlHd)
{
	nom=shl.GetNormal(patchPlHd);
	shl.GetPolygon(plVtHd,patchPlHd);
	for(auto &vtHd : plVtHd)
	{
		patchVtKeyToSrcVtHd.FindElement(vtHd,shl.GetSearchKey(vtHd));
	}
}


/* } */
#endif
