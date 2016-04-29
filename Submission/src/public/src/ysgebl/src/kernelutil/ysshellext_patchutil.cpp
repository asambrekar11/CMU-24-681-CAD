/* ////////////////////////////////////////////////////////////

File Name: ysshellext_patchutil.cpp
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

#include "ysshellext_patchutil.h"
#include "ysshellext_localop.h"
#include "ysshellext_geomcalc.h"

YsShellExt_PatchBetweenTwoConstEdge::YsShellExt_PatchBetweenTwoConstEdge()
{
	shl.EnableSearch();
}

YsShellExt_PatchBetweenTwoConstEdge::~YsShellExt_PatchBetweenTwoConstEdge()
{
}

void YsShellExt_PatchBetweenTwoConstEdge::CleanUp(void)
{
	shl.CleanUp();
	patchVtKeyToSrcVtHd.CleanUp();
	patchVtKeyToSeqId.CleanUp();
	seqCeHd[0]=NULL;
	seqCeHd[1]=NULL;

	rawVtHdSeq[0].CleanUp();
	rawVtHdSeq[1].CleanUp();
	vtHdSeq[0].CleanUp();
	vtHdSeq[1].CleanUp();
	totalLen[0]=0.0;
	totalLen[1]=0.0;
}

YSRESULT YsShellExt_PatchBetweenTwoConstEdge::SetVertexSequence(const YsShell &srcShl,
                       YSSIZE_T nVt0,const YsShellVertexHandle vtHdArray0[],YSBOOL isLoop0,
                       YSSIZE_T nVt1,const YsShellVertexHandle vtHdArray1[],YSBOOL isLoop1)
{
	if((YSTRUE!=isLoop0 && 2>nVt0) || (YSTRUE==isLoop0 && 3>nVt0) ||
	   (YSTRUE!=isLoop1 && 2>nVt1) || (YSTRUE==isLoop1 && 3>nVt1))
	{
		return YSERR;
	}

	YsArray <YsShellVertexHandle> newVtHdArray[2];

	shl.CleanUp();

	isLoop[0]=isLoop0;
	isLoop[1]=isLoop1;

	for(int seq=0; seq<2; ++seq)
	{
		const YSSIZE_T nVt=(0==seq ? nVt0 : nVt1);
		const YsShellVertexHandle *vtHdArray=(0==seq ? vtHdArray0 : vtHdArray1);
		const YSBOOL isLoop=(0==seq ? isLoop0 : isLoop1);

		totalLen[seq]=0.0;

		for(YSSIZE_T idx=0; idx<nVt; ++idx)
		{
			auto vtHd=shl.AddVertex(srcShl.GetVertexPosition(vtHdArray[idx]));
			patchVtKeyToSrcVtHd.AddElement(shl.GetSearchKey(vtHd),vtHdArray[idx]);
			patchVtKeyToSeqId.AddElement(shl.GetSearchKey(vtHd),seq);
			rawVtHdSeq[seq].Append(vtHd);

			if(0<idx)
			{
				totalLen[seq]+=shl.GetEdgeLength(rawVtHdSeq[seq][idx-1],vtHd);
			}
		}

		if(YSTRUE==isLoop && 3<=rawVtHdSeq[seq].GetN())
		{
			totalLen[seq]+=shl.GetEdgeLength(rawVtHdSeq[seq][0],rawVtHdSeq[seq].Last());
		}

		vtHdSeq[seq]=rawVtHdSeq[seq];
		seqCeHd[seq]=shl.AddConstEdge(rawVtHdSeq[seq],isLoop);
	}

	return YSOK;
}

void YsShellExt_PatchBetweenTwoConstEdge::CalculateParameter(void)
{
	for(int seq=0; seq<2; ++seq)
	{
		paramArray[seq].Set(vtHdSeq[seq].GetN(),NULL);

		paramArray[seq][0]=0.0;
		paramArray[seq].Last()=1.0;

		double lSum=0.0;

		for(YSSIZE_T idx=1; idx<vtHdSeq[seq].GetN()-1; ++idx)
		{
			lSum+=shl.GetEdgeLength(vtHdSeq[seq][idx-1],vtHdSeq[seq][idx]);
			paramArray[seq][idx]=lSum/totalLen[seq];
		}

		printf("Sequence %d\n",seq);
		for(auto t : paramArray[seq])
		{
			printf("%lf\n",t);
		}
	}
}

void YsShellExt_PatchBetweenTwoConstEdge::MakeInitialGuess(void)
{
	YSSIZE_T index[2]={0,0};

	while(index[0]<paramArray[0].GetN()-1 || index[1]<paramArray[1].GetN()-1)
	{
		int nextMove=-1;
		if(index[0]>=paramArray[0].GetN()-1)
		{
			nextMove=1;
		}
		else if(index[1]>=paramArray[1].GetN()-1)
		{
			nextMove=0;
		}
		else if(paramArray[0][index[0]]<paramArray[1][index[1]])
		{
			nextMove=0;
		}
		else 
		{
			nextMove=1;
		}

		YsShellVertexHandle triVtHd[3]=
		{
			vtHdSeq[0][index[0]],
			vtHdSeq[1][index[1]],
			vtHdSeq[nextMove][index[nextMove]+1]
		};
		shl.AddPolygon(3,triVtHd);

		++index[nextMove];
	}
}

void YsShellExt_PatchBetweenTwoConstEdge::MinimizeDihedralAngleSum(void)
{
	YsArray <YsStaticArray <YsShellVertexHandle,2> > allEdge;

	YsShellEdgeEnumHandle edHd=NULL;
	while(YSOK==shl.MoveToNextEdge(edHd))
	{
		YsShellVertexHandle edVtHd[2];
		shl.GetEdge(edVtHd,edHd);
		if(0==shl.GetNumConstEdgeUsingEdgePiece(edVtHd))
		{
			allEdge.Increment();
			allEdge.Last()[0]=edVtHd[0];
			allEdge.Last()[1]=edVtHd[1];
		}
	}

	for(;;)
	{
		YSBOOL improvement=YSFALSE;

		for(auto &edVtHd : allEdge)
		{
printf("%s %d\n",__FUNCTION__,__LINE__);
YsVec3 edVtPos[2]={shl.GetVertexPosition(edVtHd[0]),shl.GetVertexPosition(edVtHd[1])};
printf("%s %s\n",edVtPos[0].Txt(),edVtPos[1].Txt());
			YsShell_SwapInfo swapInfo;
			if(YSOK==swapInfo.MakeInfo((const YsShell &)shl,edVtHd))
			{
printf("%s %d\n",__FUNCTION__,__LINE__);
				if(0<shl.GetNumPolygonUsingEdge(swapInfo.newDiagonal[0],swapInfo.newDiagonal[1]))
				{
printf("%s %d\n",__FUNCTION__,__LINE__);
					continue;
				}

				int seqId[2];
				if(YSOK!=patchVtKeyToSeqId.FindElement(seqId[0],shl.GetSearchKey(swapInfo.newDiagonal[0])) ||
				   YSOK!=patchVtKeyToSeqId.FindElement(seqId[1],shl.GetSearchKey(swapInfo.newDiagonal[1])) ||
				   seqId[0]==seqId[1])
				{
printf("%s %d\n",__FUNCTION__,__LINE__);
					continue;
				}

				const double oldDhaTotal=YsShellExt_CalculateTotalDihedralAngleAroundEdge(shl,swapInfo.orgDiagonal);

				YsArray <YsShellVertexHandle,4> oldTriVtHd[2];
				shl.GetPolygon(oldTriVtHd[0],swapInfo.triPlHd[0]);
				shl.GetPolygon(oldTriVtHd[1],swapInfo.triPlHd[1]);

				shl.SetPolygonVertex(swapInfo.triPlHd[0],3,swapInfo.newTriVtHd[0]);
				shl.SetPolygonVertex(swapInfo.triPlHd[1],3,swapInfo.newTriVtHd[1]);

				const double newDhaTotal=YsShellExt_CalculateTotalDihedralAngleAroundEdge(shl,swapInfo.newDiagonal);
printf("%lf %lf\n",newDhaTotal,oldDhaTotal);
				if(newDhaTotal<oldDhaTotal-YsTolerance)
				{
printf("%s %d\n",__FUNCTION__,__LINE__);
					improvement=YSTRUE;
					edVtHd[0]=swapInfo.newDiagonal[0];
					edVtHd[1]=swapInfo.newDiagonal[1];
				}
				else
				{
					shl.SetPolygonVertex(swapInfo.triPlHd[0],oldTriVtHd[0]);
					shl.SetPolygonVertex(swapInfo.triPlHd[1],oldTriVtHd[1]);
				}
			}
		}

		if(YSTRUE!=improvement)
		{
			break;
		}
	}
}
