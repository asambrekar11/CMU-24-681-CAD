/* ////////////////////////////////////////////////////////////

File Name: yspositivevector.cpp
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
#include "yspositivevector.h"

YsPositiveVectorCalculator::YsPositiveVectorCalculator()
{
	shl.AttachSearchTable(&search);
	Initialize();
}

YsPositiveVectorCalculator::~YsPositiveVectorCalculator()
{
}

void YsPositiveVectorCalculator::Initialize(void)
{
	YsShellVertexHandle vtHd[6];
	vtHd[0]=shl.AddVertexH(YsVec3( 1.0, 0.0, 0.0));
	vtHd[1]=shl.AddVertexH(YsVec3(-1.0, 0.0, 0.0));
	vtHd[2]=shl.AddVertexH(YsVec3( 0.0, 1.0, 0.0));
	vtHd[3]=shl.AddVertexH(YsVec3( 0.0,-1.0, 0.0));
	vtHd[4]=shl.AddVertexH(YsVec3( 0.0, 0.0, 1.0));
	vtHd[5]=shl.AddVertexH(YsVec3( 0.0, 0.0,-1.0));

	const int idx[]=
	{
		0,2,4,
		1,2,4,
		0,3,4,
		1,3,4,
		0,2,5,
		1,2,5,
		0,3,5,
		1,3,5
	};

	int i;
	YsShellVertexHandle triVtHd[3];
	for(i=0; i<8; i++)
	{
		triVtHd[0]=vtHd[idx[i*3]];
		triVtHd[1]=vtHd[idx[i*3+1]];
		triVtHd[2]=vtHd[idx[i*3+2]];
		shl.AddPolygonH(3,triVtHd);
	}

	nVec=0;
}

void YsPositiveVectorCalculator::AddVector(YsVec3 n)
{
	if(YSOK!=n.Normalize())
	{
		return;
	}

	if(nVec<3)
	{
		vec[nVec]=n;
	}
	nVec++;


	YsPlane pln(YsVec3(0.0,0.0,0.0),n);

	YsArray <YsShellVertexHandle,16> vtHdInsert;
	CalculateVertexOnEdge(vtHdInsert,pln);
	InsertVertexOnEdge(vtHdInsert);

	SlashPolygon(pln);
	DeleteNegativePolygon(pln);
}

void YsPositiveVectorCalculator::CalculateVertexOnEdge(YsArray <YsShellVertexHandle,16> &vtHdInsert,const YsPlane &pln)
{
	YsShellEdgeEnumHandle edHd;
	if(YSOK==search.RewindEdgeEnumHandle(shl,edHd))
	{
		for(;;)
		{
			YsShellVertexHandle edVtHd[2];
			search.GetEdge(shl,edVtHd[0],edVtHd[1],edHd);

			YsVec3 edVtPos[2];
			shl.GetVertexPosition(edVtPos[0],edVtHd[0]);
			shl.GetVertexPosition(edVtPos[1],edVtHd[1]);

			int sideOfPln[2];
			sideOfPln[0]=pln.GetSideOfPlane(edVtPos[0]);
			sideOfPln[1]=pln.GetSideOfPlane(edVtPos[1]);

			if(0>sideOfPln[0]*sideOfPln[1])
			{
				YsVec3 crs;
				if(YSOK==pln.GetIntersectionHighPrecision(crs,edVtPos[0],edVtPos[1]-edVtPos[0]))
				{
					YsShellVertexHandle crsVtHd=shl.AddVertexH(crs);
					vtHdInsert.Append(2,edVtHd);
					vtHdInsert.Append(crsVtHd);
				}
			}
			else if(0==sideOfPln[0] && 0<sideOfPln[1])
			{
				YsVec3 crs;
				if(YSOK==pln.GetIntersectionHighPrecision(crs,edVtPos[0],edVtPos[1]-edVtPos[0]))
				{
					shl.ModifyVertexPosition(edVtHd[0],crs);
				}
			}
			else if(0==sideOfPln[1] && 0<sideOfPln[0])
			{
				YsVec3 crs;
				if(YSOK==pln.GetIntersectionHighPrecision(crs,edVtPos[1],edVtPos[0]-edVtPos[1]))
				{
					shl.ModifyVertexPosition(edVtHd[1],crs);
				}
			}

			if(YSOK!=search.FindNextEdge(shl,edHd))
			{
				break;
			}
		}
	}
}

void YsPositiveVectorCalculator::InsertVertexOnEdge(const YsArray <YsShellVertexHandle,16> &vtHdInsert)
{
	int i;
	for(i=0; i<=vtHdInsert.GetN()-3; i+=3)
	{
		int nEdPl;
		const YsShellPolygonHandle *edPlHdIn;
		if(YSOK==search.FindPolygonListByEdge(nEdPl,edPlHdIn,shl,vtHdInsert[i],vtHdInsert[i+1]))
		{
			YsArray <YsShellPolygonHandle,2> edPlHd(nEdPl,edPlHdIn);

			int j;
			for(j=0; j<edPlHd.GetN(); j++)
			{
				YsArray <YsShellVertexHandle,16> plVtHd;
				shl.GetVertexListOfPolygon(plVtHd,edPlHd[j]);

				if(3<=plVtHd.GetN())
				{
					plVtHd.Append(plVtHd[0]);

					int k;
					YSBOOL mod=YSFALSE;
					for(k=0; k<plVtHd.GetN()-1; k++)
					{
						if(YSTRUE==YsSameEdge(plVtHd[k],plVtHd[k+1],vtHdInsert[i],vtHdInsert[i+1]))
						{
							plVtHd.Insert(k+1,vtHdInsert[i+2]);
							mod=YSTRUE;
							break;
						}
					}

					if(YSTRUE==mod)
					{
						plVtHd.DeleteLast();
						shl.ModifyPolygon(edPlHd[j],plVtHd.GetN(),plVtHd);
					}
				}
			}
		}
	}
}

void YsPositiveVectorCalculator::SlashPolygon(const YsPlane &pln)
{
	YsShellPolygonHandle plHd=NULL;
	while(NULL!=(plHd=shl.FindNextPolygon(plHd)))
	{
		int nPlVt;
		const YsShellVertexHandle *plVtHd;
		YsArray <YsShellVertexHandle,16> newPlVtHd;

		int nOnPlane=0,positive=-1,negative=-1;
		int onPlane[2];
		shl.GetVertexListOfPolygon(nPlVt,plVtHd,plHd);

		int i;
		for(i=0; i<nPlVt; i++)
		{
			YsVec3 vtPos;
			shl.GetVertexPosition(vtPos,plVtHd[i]);

			const int side=pln.GetSideOfPlane(vtPos);
			if(0==side)
			{
				if(2>nOnPlane)
				{
					onPlane[nOnPlane]=i;
				}
				nOnPlane++;
			}
			else if(0>side)
			{
				negative=i;
			}
			else // if(0<side)
			{
				positive=i;
			}
		}

		if(0<=negative && 0<=positive && 2==nOnPlane)
		{
			newPlVtHd.Clear();
			if(onPlane[0]<positive && positive<onPlane[1])
			{
				int i;
				for(i=onPlane[0]; i<=onPlane[1]; i++)
				{
					newPlVtHd.Append(plVtHd[i]);
				}
			}
			else
			{
				int i;
				for(i=onPlane[1]; i<nPlVt; i++)
				{
					newPlVtHd.Append(plVtHd[i]);
				}
				for(i=0; i<=onPlane[0]; i++)
				{
					newPlVtHd.Append(plVtHd[i]);
				}
			}

			if(3<=newPlVtHd.GetN())
			{
				shl.ModifyPolygon(plHd,newPlVtHd.GetN(),newPlVtHd);
			}
		}
	}
}

void YsPositiveVectorCalculator::DeleteNegativePolygon(const YsPlane &pln)
{
	YsArray <YsShellPolygonHandle> plHdToDel;
	YsShellPolygonHandle plHd=NULL;
	while(NULL!=(plHd=shl.FindNextPolygon(plHd)))
	{
		int nPlVt;
		const YsShellVertexHandle *plVtHd;
		shl.GetVertexListOfPolygon(nPlVt,plVtHd,plHd);

		int i;
		for(i=0; i<nPlVt; i++)
		{
			YsVec3 vtPos;
			shl.GetVertexPosition(vtPos,plVtHd[i]);
			if(pln.GetSideOfPlane(vtPos)<0)
			{
				plHdToDel.Append(plHd);
				break;
			}
		}
	}

	YsArray <YsShellVertexHandle> vtHdToDel;
	YsShellVertexHandle vtHd=NULL;
	while(NULL!=(vtHd=shl.FindNextVertex(vtHd)))
	{
		YsVec3 vtPos;
		shl.GetVertexPosition(vtPos,vtHd);
		if(pln.GetSideOfPlane(vtPos)<0)
		{
			vtHdToDel.Append(vtHd);
		}
	}

	int i;
	for(i=0; i<plHdToDel.GetN(); i++)
	{
		shl.DeletePolygon(plHdToDel[i]);
	}
	for(i=0; i<vtHdToDel.GetN(); i++)
	{
		shl.DeleteVertex(vtHdToDel[i]);
	}
}

YSRESULT YsPositiveVectorCalculator::GetPositiveVector(YsVec3 &vec) const
{
	if(0<shl.GetNumPolygon())
	{
		vec=YsVec3(0.0,0.0,0.0);
		YsShellPolygonHandle plHd=NULL;
		while(NULL!=(plHd=shl.FindNextPolygon(plHd)))
		{
			YsVec3 cen;
			shl.GetCenterOfPolygon(cen,plHd);
			vec+=cen;
		}
		return vec.Normalize();
	}
	return YSERR;
}

YSRESULT YsPositiveVectorCalculator::SaveSrf(const char fn[]) const
{
	return shl.SaveSrf(fn);
}


// See also positive-vector-2.docx
/* static */ YSRESULT YsPositiveVectorCalculator::Optimize(YsVec3 &vecOut,const YsVec3 &vecIn,const YSSIZE_T nRefVec,const YsVec3 refVec[],int nConst,const YsVec3 constVec[])
{
	YSRESULT improvement=YSERR;

	YsArray <YSBOOL,16> isMinimum(nRefVec,NULL);
	YsArray <double,16> slope(nRefVec,NULL);
	YsArray <double,16> dotProd(nRefVec,NULL);

	vecOut=vecIn;

	ApplyConstraint(vecOut,nConst,constVec);

	double minDot=0.0;
	for(int i=0; i<nRefVec; i++)
	{
		dotProd[i]=refVec[i]*vecOut;
		if(0==i || dotProd[i]<minDot)
		{
			minDot=dotProd[i];
		}
	}

	for(;;) // Until no more improvement can be found.
	{
		if(YSOK!=vecOut.Normalize())
		{
			return YSERR;
		}

		YsVec3 goal(0.0,0.0,0.0);
		for(int i=0; i<nRefVec; i++)
		{
			const double dotProd=refVec[i]*vecOut;
			if(YSTRUE==YsEqual(minDot,dotProd))
			{
				goal+=refVec[i];
			}
		}

		if(YSOK!=goal.Normalize())
		{
			return YSERR;
		}

		ApplyConstraint(goal,nConst,constVec);

		double newMinimum;
		if(YSOK==OptimizeByBiSection(newMinimum,vecOut,goal,nRefVec,refVec,nConst,constVec,minDot) &&
		   fabs(minDot)*0.01<(newMinimum-minDot))  // 1% cut off
		{
			minDot=newMinimum;
			improvement=YSOK;
		}
		else
		{
			break;
		}
	}

	return improvement;
}

/* static */ YSRESULT YsPositiveVectorCalculator::OptimizeByBiSection(
    double &newMinimum,YsVec3 &vecInOut,const YsVec3 &vecGoal,const YSSIZE_T nRefVec,const YsVec3 refVec[],int /*nConst*/,const YsVec3 /*constVec*/[],const double currentMinimum)
{
	double t0,t1,f0,f1;
	t0=0.0;
	t1=1.0;

	f0=currentMinimum;
	f1=YsInfinity;
	for(int i=0; i<nRefVec; i++)
	{
		f1=YsSmaller(f1,refVec[i]*vecGoal);
	}

	for(int i=0; i<10; i++)
	{
		const double tm=(t0+t1)/2.0;
		YsVec3 vm=vecInOut*(1.0-tm)+vecGoal*tm;
		vm.Normalize();

		double fm=YsInfinity;
		for(int i=0; i<nRefVec; i++)
		{
			fm=YsSmaller(fm,refVec[i]*vm);
		}

		if(f0>f1)
		{
			f1=fm;
			t1=tm;
		}
		else
		{
			f0=fm;
			t0=tm;
		}
	}

	const double t=(f0>f1 ? t0 : t1);
	if(YSTRUE!=YsEqual(t,0.0))
	{
		YsVec3 newVec=vecInOut*(1.0-t)+vecGoal*t;
		if(YSOK==newVec.Normalize())
		{
			newMinimum=YsGreater(f0,f1);
			vecInOut=newVec;
			return YSOK;
		}
	}
	return YSERR;
}

/* static */YSRESULT YsPositiveVectorCalculator::ApplyConstraint(YsVec3 &vecInOut,int nConst,const YsVec3 constVec[])
{
	// Not really correct if 1<nConst
	if(0<nConst)
	{
		for(int i=0; i<nConst; ++i)
		{
			if(0.0>vecInOut*constVec[i])
			{
				vecInOut-=constVec[i]*(constVec[i]*vecInOut);
			}
		}
		vecInOut.Normalize();
	}
	return YSOK;
}

