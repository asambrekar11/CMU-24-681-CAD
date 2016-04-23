/* ////////////////////////////////////////////////////////////

File Name: ysshellext_proximityutil.cpp
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

#include "ysshellext_proximityutil.h"

YsShellExt_EdgeVertexProximityUtil::YsShellExt_EdgeVertexProximityUtil()
{
	shlPtr=NULL;
	CleanUp();
}

void YsShellExt_EdgeVertexProximityUtil::CleanUp(void)
{
	edVtPairTree.CleanUp();
	edgeToEdVtPairHandle.CleanUp();
	vertexToEdVtPairHandle.CleanUp();
}

void YsShellExt_EdgeVertexProximityUtil::SetShell(const YsShellExt &shl)
{
	this->shlPtr=&shl;
	edgeToEdVtPairHandle.SetShell(shl.Conv());
	vertexToEdVtPairHandle.SetShell(shl.Conv());
}

YsShellExt_EdgeVertexProximityUtil::EDVTPAIR_HANDLE YsShellExt_EdgeVertexProximityUtil::AddEdgeVertexPair(const YsShellVertexHandle edVtHd[2],YsShellVertexHandle vtHd,const double dist)
{
	EdgeVertexPair evp;
	evp.edVtHd[0]=edVtHd[0];
	evp.edVtHd[1]=edVtHd[1];
	evp.vtHd=vtHd;
	evp.nSharingEdge=0;

	auto hd=edVtPairTree.Insert(dist,evp);
	edgeToEdVtPairHandle.AddAttrib(edVtHd,hd);
	vertexToEdVtPairHandle.SetAttrib(vtHd,hd);

	auto sameEdgePairHd=FindEdgeVertexPairFromEdgePiece(edVtHd);
	for(auto hd : sameEdgePairHd)
	{
		edVtPairTree[hd].nSharingEdge=(int)sameEdgePairHd.GetN();
	}

	return hd;
}

YsShellExt_EdgeVertexProximityUtil::EDVTPAIR_HANDLE YsShellExt_EdgeVertexProximityUtil::GetNearestEdgeVertexPairHandle(void) const
{
	return edVtPairTree.First();
}

void YsShellExt_EdgeVertexProximityUtil::DeleteEdgeVertexPair(EDVTPAIR_HANDLE hd)
{
	auto pair=edVtPairTree[hd];

	auto needUpdate=FindEdgeVertexPairFromEdgePiece(pair.edVtHd);
	for(auto hd : needUpdate)
	{
		--edVtPairTree[hd].nSharingEdge;
	}

	edgeToEdVtPairHandle.DeleteAttrib(pair.edVtHd,hd);
	vertexToEdVtPairHandle.DeleteAttrib(pair.vtHd);
	edVtPairTree.Delete(hd);
}

YSRESULT YsShellExt_EdgeVertexProximityUtil::FindNearestEdgeFromVertexNotSharingFaceGroup(YsShellVertexHandle edVtHdMin[2],double &dMin,const YsShellExt &shl,const YsShellLattice &ltc,YsShellVertexHandle vtHd) const
{
	edVtHdMin[0]=NULL;
	edVtHdMin[1]=NULL;
	dMin=0.0;

	double searchDist=ltc.GetLatticeDimension().GetLength();
	const double blkDgn=ltc.GetBlockDimension().GetLength();

	YsArray <YsShellVertexHandle,16> edVtHd;
	YsArray <YSHASHKEY,16> ceKey;
	YSSIZE_T nTested=0;

	const YsVec3 vtPos=shl.GetVertexPosition(vtHd);

	const auto excludeFgHd=shl.FindFaceGroupFromVertex(vtHd);

	for(int blkDist=0; blkDist<ltc.GetMaxNumBlock() && blkDgn*(double)blkDist<searchDist; ++blkDist)
	{
		if(YSOK==ltc.MakeEdgePieceListByPointAndBlockDistance(edVtHd,ceKey,nTested,vtPos,blkDist) && 0<nTested)
		{
			/*
			   Probablyl, it finds itself.
			   It must ignore edges that belongs to one of the face groups incident to the vertex because I don't want collapse of a face-group.
			*/

			for(int idx=0; idx<=edVtHd.GetN()-2; idx+=2)
			{
				auto edPlHd=shl.FindFaceGroupFromEdgePiece(edVtHd[idx],edVtHd[idx+1]);
				if(YSTRUE!=excludeFgHd.HasCommonItem(edPlHd))
				{
					// Is a candidate!
					const YsVec3 edVtPos[2]=
					{
						shl.GetVertexPosition(edVtHd[idx]),
						shl.GetVertexPosition(edVtHd[idx+1]),
					};
					const double d=YsGetPointLineSegDistance3(edVtPos,vtPos);
					if(NULL==edVtHdMin[0] || d<dMin)
					{
						edVtHdMin[0]=edVtHd[idx];
						edVtHdMin[1]=edVtHd[idx+1];
						dMin=d;
						searchDist=dMin;
					}
				}
			}
		}
		else
		{
			break;
		}
	}
	if(NULL!=edVtHdMin[0])
	{
		return YSOK;
	}
	return YSERR;
}


void YsShellExt_EdgeVertexProximityUtil::MoveToNextEdgeVertexPair(EDVTPAIR_HANDLE &hd) const
{
	edVtPairTree.MoveToNext(hd);
}


YsShellExt_EdgeVertexProximityUtil::EdgeVertexPair YsShellExt_EdgeVertexProximityUtil::GetEdgeVertexPair(EDVTPAIR_HANDLE hd) const
{
	return edVtPairTree[hd];
}



YsArray <YsShellExt_EdgeVertexProximityUtil::EDVTPAIR_HANDLE> YsShellExt_EdgeVertexProximityUtil::FindEdgeVertexPairFromEdgePiece(const YsShellVertexHandle edVtHd[2]) const
{
	return edgeToEdVtPairHandle.FindAttrib(edVtHd);
}

YsArray <YsShellExt_EdgeVertexProximityUtil::EDVTPAIR_HANDLE> YsShellExt_EdgeVertexProximityUtil::FindEdgeVertexPairFromEdgePiece(YsShellVertexHandle edVtHd0,YsShellVertexHandle edVtHd1) const
{
	return edgeToEdVtPairHandle.FindAttrib(edVtHd0,edVtHd1);
}

YSSIZE_T YsShellExt_EdgeVertexProximityUtil::GetEdgeIsClosestOfHowManyVertex(const YsShellVertexHandle edVtHd[2]) const
{
	return GetEdgeIsClosestOfHowManyVertex(edVtHd[0],edVtHd[1]);
}

YSSIZE_T YsShellExt_EdgeVertexProximityUtil::GetEdgeIsClosestOfHowManyVertex(YsShellVertexHandle edVtHd0,YsShellVertexHandle edVtHd1) const
{
	auto found=FindEdgeVertexPairFromEdgePiece(edVtHd0,edVtHd1);
	return found.GetN();
}

YsArray <YsShellVertexHandle> YsShellExt_EdgeVertexProximityUtil::FindNearestVertexFromEdgePiece(YsShellVertexHandle edVtHd0,YsShellVertexHandle edVtHd1) const
{
	auto foundPairHd=FindEdgeVertexPairFromEdgePiece(edVtHd0,edVtHd1);
	YsArray <YsShellVertexHandle> nearVtHd;

	if(0<foundPairHd.GetN())
	{
		YsArray <double> nearDist;
		auto &shl=*shlPtr;
		for(auto pairHd : foundPairHd)
		{
			auto pair=GetEdgeVertexPair(pairHd);
			nearVtHd.Append(pair.vtHd);
			nearDist.Append(shl.GetEdgeLength(pair.vtHd,edVtHd0));
		}

		YsQuickSort(nearDist.GetN(),nearDist.GetEditableArray(),nearVtHd.GetEditableArray());
	}

	return nearVtHd;
}

YsArray <YsShellVertexHandle> YsShellExt_EdgeVertexProximityUtil::FindNearestVertexFromEdgePiece(const YsShellVertexHandle edVtHd[2]) const
{
	return FindNearestVertexFromEdgePiece(edVtHd[0],edVtHd[1]);
}

YsShellExt_EdgeVertexProximityUtil::EDVTPAIR_HANDLE YsShellExt_EdgeVertexProximityUtil::FindEdgeVertexPairFromVertex(YsShellVertexHandle vtHd) const
{
	auto hdPtr=vertexToEdVtPairHandle[vtHd];
	if(NULL!=hdPtr)
	{
		return *hdPtr;
	}
	EDVTPAIR_HANDLE hd;
	hd.Nullify();
	return hd;
}
