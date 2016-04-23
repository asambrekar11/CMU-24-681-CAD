/* ////////////////////////////////////////////////////////////

File Name: ysshellextio.cpp
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

#include "ysshellextio.h"

template <const int N>
YSRESULT YsShellExtReader::GetVtHdArray(YsArray <YsShellVertexHandle,N> &plVtHd,YSSIZE_T n,const YsString args[],const char src[]) const
{
	plVtHd.Clear();
	YSRESULT res=YSOK;
	for(YSSIZE_T idx=0; idx<n; ++idx)
	{
		const int vtIdx=atoi(args[idx]);
		if(YSTRUE==readEntity.vtHdArray.IsInRange(vtIdx))
		{
			plVtHd.Append(readEntity.vtHdArray[vtIdx]);
		}
		else
		{
			lastErrorMessage.Printf("Vertex Idx=%d is not defined.",vtIdx);
			lastErrorLine=src;
			res=YSERR;
		}
	}
	return res;
}

template <const int N>
YSRESULT YsShellExtReader::GetPlHdArray(YsArray <YsShellPolygonHandle,N> &fgPlHd,YSSIZE_T n,const YsString args[],const char src[]) const
{
	fgPlHd.Clear();
	YSRESULT res=YSOK;
	for(YSSIZE_T idx=0; idx<n; ++idx)
	{
		const int plIdx=atoi(args[idx]);
		if(YSTRUE==readEntity.plHdArray.IsInRange(plIdx))
		{
			fgPlHd.Append(readEntity.plHdArray[plIdx]);
		}
		else
		{
			lastErrorMessage.Printf("Polygon Idx=%d is not defined.",plIdx);
			lastErrorLine=src;
			res=YSERR;
		}
	}
	return res;
}

template <const int N>
YSRESULT YsShellExtReader::GetFgHdArray(YsArray <YsShellExt::FaceGroupHandle,N> &fgHdArray,YSSIZE_T n,const YsString args[],const char src[]) const
{
	fgHdArray.Clear();
	YSRESULT res=YSOK;
	for(YSSIZE_T idx=0; idx<n; ++idx)
	{
		const int fgIdent=atoi(args[idx]);
		YsShellExt::FaceGroupHandle fgHd;
		if(YSOK==srf.fgIdentToFgHd.FindElement(fgHd,fgIdent))
		{
			fgHdArray.Append(fgHd);
		}
		else
		{
			lastErrorMessage.Printf("Face Group Ident=%d is not defined.",fgIdent);
			lastErrorLine=src;
			res=YSERR;
		}
	}
	return res;
}

YsShellVertexHandle YsShellExtReader::GetVertexHandle(const char arg[],const char src[]) const
{
	const int vtIdx=atoi(arg);
	if(YSTRUE==readEntity.vtHdArray.IsInRange(vtIdx))
	{
		return readEntity.vtHdArray[vtIdx];
	}
	else
	{
		lastErrorMessage.Printf("Vertex Idx=%d is not defined.",vtIdx);
		lastErrorLine=src;
		return NULL;
	}
}

YsShellPolygonHandle YsShellExtReader::GetPolygonHandle(const char arg[],const char src[]) const
{
	const int plIdx=atoi(arg);
	if(YSTRUE==readEntity.plHdArray.IsInRange(plIdx))
	{
		return readEntity.plHdArray[plIdx];
	}
	else
	{
		lastErrorMessage.Printf("Polygon Idx=%d is not defined.",plIdx);
		lastErrorLine=src;
		return NULL;
	}
}

YSRESULT YsShellExtReader::MergeSrf(YsShellExt &shl,YsTextInputStream &inStream)
{
	YSRESULT res=YSOK;

	StartMergeSrf(shl);

	YsString str;
	while(NULL!=inStream.Gets(str))
	{
		if(YSOK!=ReadSrfOneLine(shl,str))
		{
			res=YSERR;
		}
	}

	EndMergeSrf();

	return res;
}

const YsShellExtEntitySet &YsShellExtReader::GetEntitySet(void) const
{
	return readEntity;
}

void YsShellExtReader::StartMergeSrf(const YsShellExt &shl)
{
	readEntity.CleanUp();

	srf.state=ReadSrfVariable::NORMAL;
	srf.alreadyUsedCeIdent.CleanUp();
	srf.alreadyUsedFgIdent.CleanUp();
	srf.alreadyUsedVlIdent.CleanUp();

	for(auto ceHd : shl.AllConstEdge())
	{
		auto ceId=shl.GetConstEdgeIdent(ceHd);
		if(0<=ceId)
		{
			srf.alreadyUsedCeIdent.AddKey((YSHASHKEY)ceId);
		}
	}
	for(auto fgHd : shl.AllFaceGroup())
	{
		auto fgId=shl.GetFaceGroupIdent(fgHd);
		if(0<=fgId)
		{
			srf.alreadyUsedFgIdent.AddKey((YSHASHKEY)fgId);
		}
	}
//	for(auto vlHd : shl.AllVolume())
//	{
//		auto vlId=shl.GetVolumeIdent(vlHd);
//		if(0<=vlId)
//		{
//			srf.alreadyUsedVlIdent.AddKey((YSHASHKEY)vlId);
//		}
//	}

	srf.ceIdentToCeHd.CleanUp();
	srf.fgIdentToFgHd.CleanUp();
	srf.vlIdentToVlHd.CleanUp();
}

YSRESULT YsShellExtReader::ReadSrfOneLine(YsShellExt &shl,const YsString &str)
{
	for(YSSIZE_T idx=0; idx<str.Strlen(); ++idx)
	{
		if(' '!=str[idx] && '\t'!=str[idx])
		{
			if('#'==str[idx] || 'S'==str[idx])  // 'S' for file identifier.
			{
				return YSOK;
			}
			else
			{
				break;
			}
		}
	}

	switch(srf.state)
	{
	default:
	case ReadSrfVariable::NORMAL:
		return ReadSrfOneLineNormal(shl,str);
	case ReadSrfVariable::IN_POLYGON:
		return ReadSrfOneLineInPolygon(shl,str);
	}
}

YSRESULT YsShellExtReader::ReadSrfOneLineNormal(YsShellExt &shl,const YsString &str)
{
	// Doesn't give an error, but unsupported: 
	//   GVE    Const-edge to volume
	//   X      Adjacency information
	//   M      Tensor-field string

	// Keywords starting with X,Y, or Z are for expanded information, and will not give an error even if the command is not recognized.
	// However, if the command is recognized and does not give an adequate set of parameters, it will be an error.

	YsArray <YsString,16> args;
	str.Arguments(args);
	if(0<args.GetN())
	{
		args[0].Capitalize();
		switch(args[0][0])
		{
		case '#':
			return YSOK; // Comment line
		case 'E':
			return YSOK;  // E outside polygon used to be used for making the end of .SRF file.  It is no longer used.
		case 'F':
			StartPolygon();
			return YSOK;
		case 'V':
			if(4<=args.GetN())
			{
				const YsVec3 newVtPos(args[1],args[2],args[3]);
				const YSBOOL round=((5<=args.GetN() && (args[4][0]=='R' || args[4][0]=='r')) ? YSTRUE : YSFALSE);
				const YsShellVertexHandle vtHd=shl.AddVertex(newVtPos);
				shl.SetVertexRoundFlag(vtHd,round);

				readEntity.vtHdArray.Append(vtHd);

				return YSOK;
			}
			else if(3<=args.GetN())
			{
				const YsVec3 newVtPos(args[1],args[2],"0.0");
				const YsShellVertexHandle vtHd=shl.AddVertex(newVtPos);

				readEntity.vtHdArray.Append(vtHd);

				return YSOK;
			}
			lastErrorMessage="Insufficient arguments for a vertex.";
			lastErrorLine=str;
			break;
		case 'G':
			switch(args[0][1])
			{
			case 'E': // GE
			case 'L': // GL
				if(2<=args.GetN())
				{
					const int ceIdent=atoi(args[1]);
					YsShellExt::ConstEdgeHandle ceHd=FindConstEdgeByIdentCreateIfNotExist(shl,ceIdent);
					shl.SetConstEdgeIsLoop(ceHd,(args[0][1]=='L' ? YSTRUE : YSFALSE));
					YsArray <YsShellVertexHandle,16> ceVtHd;
					YSRESULT res=GetVtHdArray(ceVtHd,args.GetN()-2,args.GetArray()+2,str);
					shl.AddConstEdgeVertex(ceHd,ceVtHd);
					return res;
				}
				break;
			case 'V': // GV
				if(0==strcmp("GV",args[0]) && 2<=args.GetN())
				{
					const int vlIdent=atoi(args[1]);
					YsShellExt::VolumeHandle vlHd=FindVolumeByIdentCreateIfNotExist(shl,vlIdent);
					YsArray <YsShellExt::FaceGroupHandle,16> vlFgHd;
					YSRESULT res=GetFgHdArray(vlFgHd,args.GetN()-2,args.GetArray()+2,str);
					shl.AddVolumeFaceGroup(vlHd,vlFgHd);
					return res;
				}
				else if(0==strcmp("GVE",args[0]))
				{
					return YSOK;
				}
				break;
			case 'F': // GF
				if(2<=args.GetN())
				{
					const int fgIdent=atoi(args[1]);
					YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,fgIdent);
					YsArray <YsShellPolygonHandle,16> fgPlHd;
					YSRESULT res=GetPlHdArray(fgPlHd,args.GetN()-2,args.GetArray()+2,str);
					shl.AddFaceGroupPolygon(fgHd,fgPlHd);
					return res;
				}
				break;
			}
			break;
		case 'C':
			switch(args[0][1])
			{
			case 'V':
				{
					YsArray <YsShellVertexHandle,16> vtHd;
					YSRESULT res=GetVtHdArray(vtHd,args.GetN()-1,args.GetArray()+1,str);
					for(YSSIZE_T vtIdx=0; vtIdx<vtHd.GetN(); ++vtIdx)
					{
						shl.SetVertexConstrainedFlag(vtHd[vtIdx],YSTRUE);
					}
					return res;
				}
				break;
			}
			break;
		case 'L':
			switch(args[0][1])
			{
			case 'E':
			case 'L':
				if(3<=args.GetN())
				{
					const int ceIdent=atoi(args[1]);
					YsShellExt::ConstEdgeHandle ceHd=FindConstEdgeByIdentCreateIfNotExist(shl,ceIdent);
					return shl.SetConstEdgeLabel(ceHd,args[2]);
				}
				break;
			case 'F':
				if(3<=args.GetN())
				{
					const int fgIdent=atoi(args[1]);
					YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,fgIdent);
					return shl.SetFaceGroupLabel(fgHd,args[2]);
				}
				break;
			case 'V':
				if(3<=args.GetN())
				{
					const int vlIdent=atoi(args[1]);
					YsShellExt::VolumeHandle vlHd=FindVolumeByIdentCreateIfNotExist(shl,vlIdent);
					return shl.SetVolumeLabel(vlHd,args[2]);
				}
				break;
			}
			break;
		case 'M':
			return YSOK;
		case 'X':
			return YSOK;
		case 'Y':
			switch(args[0][1])
			{
			case 'E':
				if(4<=args.GetN())
				{
					YsShellVertexHandle vtHd=GetVertexHandle(args[1],str);
					if(NULL!=vtHd)
					{
						return shl.SetVertexTrackingToEdge(vtHd,atoi(args[2]),atoi(args[3]));
					}
					return YSERR;
				}
				break;
			case 'F':
				if(3<=args.GetN())
				{
					YsShellVertexHandle vtHd=GetVertexHandle(args[1],str);
					if(NULL!=vtHd)
					{
						return shl.SetVertexTrackingToPolygon(vtHd,atoi(args[2]));
					}
					return YSERR;
				}
				break;
			case 'N':
				if(3<=args.GetN())
				{
					YsShellVertexHandle vtHd=GetVertexHandle(args[1],str);
					if(NULL!=vtHd)
					{
						return shl.SetVertexTrackingToVertex(vtHd,atoi(args[2]));
					}
					return YSERR;
				}
				break;
			default:
				return YSOK;
			}
			break;
		case 'Z':
			switch(args[0][1])
			{
			case 'C':
				if(0==args[0].Strcmp("ZCRE") && 2<=args.GetN())
				{
					for(auto &arg : args.Subset(1))
					{
						const int ceIdent=atoi(arg);
						YsShellExt::ConstEdgeHandle ceHd=FindConstEdgeByIdentCreateIfNotExist(shl,ceIdent);
						if(NULL!=ceHd)
						{
							shl.SetConstEdgeIsCrease(ceHd,YSTRUE);
						}
					}
				}
				break;
			case 'A':
				if(0==strcmp("ZA",args[0]))
				{
					for(YSSIZE_T argIdx=1; argIdx<=args.GetN()-2; argIdx+=2)
					{
						YsShellPolygonHandle plHd=GetPolygonHandle(args[argIdx],str);
						if(NULL!=plHd)
						{
							const double alpha=(double)(255-atoi(args[argIdx+1]))/255.0;
							shl.SetPolygonAlpha(plHd,alpha);
						}
					}
					return YSOK;
				}
				break;
			case 'F':
				if(0==args[0].STRCMP("ZF"))
				{
					for(YSSIZE_T argIdx=1; argIdx<=args.GetN()-2; ++argIdx)
					{
						YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,atoi(args[argIdx]));
						auto attrib=shl.GetFaceGroupAttrib(fgHd);
						attrib.elemSize=atof(args[argIdx+1]);
						shl.SetFaceGroupAttrib(fgHd,attrib);
					}
					return YSOK;
				}
				if(0==strcmp("ZFI",args[0]))
				{
					for(YSSIZE_T argIdx=1; argIdx<args.GetN(); ++argIdx)
					{
						YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,atoi(args[argIdx]));
						shl.SetFaceGroupInteriorConstSurfaceFlag(fgHd,YSTRUE);
					}
					return YSOK;
				}
				if(0==args[0].STRCMP("ZFFL") && 3<=args.GetN())
				{
					YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,atoi(args[1]));
					auto attrib=shl.GetFaceGroupAttrib(fgHd);
					attrib.bLayerSpec.nLayer=atoi(args[2]);
					shl.SetFaceGroupAttrib(fgHd,attrib);
					return YSOK;
				}
				if(0==args[0].STRCMP("ZFFF") && 3<=args.GetN())
				{
					YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,atoi(args[1]));
					auto attrib=shl.GetFaceGroupAttrib(fgHd);
					attrib.bLayerSpec.firstLayerThickness=atof(args[2]);
					shl.SetFaceGroupAttrib(fgHd,attrib);
					return YSOK;
				}
				if(0==args[0].STRCMP("ZFFT") && 4<=args.GetN())
				{
					YsShellExt::FaceGroupHandle fgHd=FindFaceGroupByIdentCreateIfNotExist(shl,atoi(args[1]));
					auto attrib=shl.GetFaceGroupAttrib(fgHd);
					attrib.bLayerSpec.growthFuncType=atoi(args[2]);
					attrib.bLayerSpec.growthFuncParam=atof(args[3]);
					shl.SetFaceGroupAttrib(fgHd,attrib);
					return YSOK;
				}
				break;
			case 'L':
				if(0==strcmp("ZL",args[0]))
				{
					for(YSSIZE_T argIdx=1; argIdx<args.GetN(); ++argIdx)
					{
						YsShellPolygonHandle plHd=GetPolygonHandle(args[argIdx],str);
						if(NULL!=plHd)
						{
							shl.SetPolygonAsLightFlag(plHd,YSTRUE);
						}
					}
					return YSOK;
				}
			case 'P':
				if(0==strcmp("ZP",args[0]))
				{
					for(YSSIZE_T argIdx=1; argIdx<=args.GetN()-2; argIdx+=2)
					{
						YsShellPolygonHandle plHd=GetPolygonHandle(args[argIdx],str);
						if(NULL!=plHd)
						{
							const double elemSize=atof(args[argIdx+1]);
							shl.SetPolygonDesiredElementSize(plHd,elemSize);
						}
					}
				}
				break;
			}
			return YSOK;
		}
		lastErrorMessage="Unrecognized command or incorrect parameters.";
		lastErrorLine=str;
		return YSERR;
	}
	else
	{
		// Just an empty line.
		return YSOK;
	}
}

YSRESULT YsShellExtReader::ReadSrfOneLineInPolygon(YsShellExt &shl,const YsString &str)
{
	YsArray <YsString,16> args;
	str.Arguments(args);

	if(0<args.GetN())
	{
		args[0].Capitalize();
		switch(args[0][0])
		{
		case 'B':
			srf.currentPlNoShad=YSTRUE;
			return YSOK;
		case 'C':
			if(5<=args.GetN())
			{
				const unsigned int r=atoi(args[1]);
				const unsigned int g=atoi(args[2]);
				const unsigned int b=atoi(args[3]);
				const unsigned int a=atoi(args[4]);
				srf.currentPlCol.SetIntRGBA(r,g,b,a);
				return YSOK;
			}
			else if(4<=args.GetN())
			{
				const unsigned int r=atoi(args[1]);
				const unsigned int g=atoi(args[2]);
				const unsigned int b=atoi(args[3]);
				srf.currentPlCol.SetIntRGB(r,g,b);
				return YSOK;
			}
			else if(2<=args.GetN())
			{
				const unsigned int col15=atoi(args[1]);
				srf.currentPlCol.Set15BitRGB(col15);
				return YSOK;
			}
			else
			{
				lastErrorMessage="Insufficient arguments for polygon color.";
				lastErrorLine=str;
			}
			break;
		case 'N':
			if(7<=args.GetN())
			{
				srf.currentPlNom.Set(args[4],args[5],args[6]);
				return YSOK;
			}
			else if(4<=args.GetN())
			{
				srf.currentPlNom.Set(args[1],args[2],args[3]);
				return YSOK;
			}
			lastErrorMessage="Insufficient arguments for polygon normal.";
			lastErrorLine=str;
			break;
		case 'V':
			{
				YsArray <YsShellVertexHandle,16> plVtHd;
				YSRESULT res=GetVtHdArray(plVtHd,args.GetN()-1,args.GetArray()+1,str);
				srf.currentPlVtHd.Append(plVtHd);
				return res;
			}
			break;
		case 'E':
			{
				const YsShellPolygonHandle plHd=shl.AddPolygon(srf.currentPlVtHd);
				shl.SetPolygonNoShadingFlag(plHd,srf.currentPlNoShad);
				shl.SetPolygonColor(plHd,srf.currentPlCol);
				shl.SetPolygonNormal(plHd,srf.currentPlNom);
				readEntity.plHdArray.Append(plHd);
				EndPolygon();
				return YSOK;
			}
			break;
		}
		lastErrorMessage="Unrecognized command (In polygon).";
		lastErrorLine=str;
		return YSERR;
	}
	else
	{
		// Just an empty line.
		return YSOK;
	}
}

void YsShellExtReader::StartPolygon(void)
{
	srf.state=ReadSrfVariable::IN_POLYGON;
	srf.currentPlVtHd.Clear();
	srf.currentPlNoShad=YSFALSE;
	srf.currentPlCol=YsBlue();
	srf.currentPlNom=YsOrigin();
}

void YsShellExtReader::EndPolygon(void)
{
	srf.state=ReadSrfVariable::NORMAL;
}

void YsShellExtReader::EndMergeSrf(void)
{
	srf.ceIdentToCeHd.CleanUp();
	srf.fgIdentToFgHd.CleanUp();
	srf.vlIdentToVlHd.CleanUp();
}

YsShellExt::ConstEdgeHandle YsShellExtReader::FindConstEdgeByIdentCreateIfNotExist(YsShellExt &shl,int ceIdent)
{
	YsShellExt::ConstEdgeHandle ceHd;
	if(YSOK==srf.ceIdentToCeHd.FindElement(ceHd,ceIdent))
	{
		return ceHd;
	}
	else
	{
		ceHd=shl.AddConstEdge(0,NULL,YSFALSE);
		srf.ceIdentToCeHd.AddElement(ceIdent,ceHd);
		if(YSTRUE!=srf.alreadyUsedCeIdent.IsIncluded((YSHASHKEY)ceIdent))
		{
			shl.SetConstEdgeIdent(ceHd,ceIdent);
			srf.alreadyUsedCeIdent.AddKey((YSHASHKEY)ceIdent);
		}
		return ceHd;
	}
}

YsShellExt::FaceGroupHandle YsShellExtReader::FindFaceGroupByIdentCreateIfNotExist(YsShellExt &shl,int fgIdent)
{
	YsShellExt::FaceGroupHandle fgHd;
	if(YSOK==srf.fgIdentToFgHd.FindElement(fgHd,fgIdent))
	{
		return fgHd;
	}
	else
	{
		fgHd=shl.AddFaceGroup(0,NULL);
		srf.fgIdentToFgHd.AddElement(fgIdent,fgHd);
		if(YSTRUE!=srf.alreadyUsedFgIdent.IsIncluded((YSHASHKEY)fgIdent))
		{
			shl.SetFaceGroupIdent(fgHd,fgIdent);
			srf.alreadyUsedFgIdent.AddKey((YSHASHKEY)fgIdent);
		}
		return fgHd;
	}
}

YsShellExt::VolumeHandle YsShellExtReader::FindVolumeByIdentCreateIfNotExist(YsShellExt &shl,int vlIdent)
{
	YsShellExt::VolumeHandle vlHd;
	if(YSOK==srf.vlIdentToVlHd.FindElement(vlHd,vlIdent))
	{
		return vlHd;
	}
	else
	{
		vlHd=shl.AddVolume(0,NULL);
		srf.vlIdentToVlHd.AddElement(vlIdent,vlHd);
		if(YSTRUE!=srf.alreadyUsedVlIdent.IsIncluded((YSHASHKEY)vlIdent))
		{
			// shl.SetVolumeIdent(vlHd,(YSHASHKEY)vlIdent);
			srf.alreadyUsedVlIdent.AddKey((YSHASHKEY)vlIdent);
		}
		return vlHd;
	}
}

const char *YsShellExtReader::GetLastErrorMessage(void) const
{
	return lastErrorMessage;
}

const char *YsShellExtReader::GetLastErrorLine(void) const
{
	return lastErrorLine;
}



////////////////////////////////////////////////////////////


YSRESULT YsShellExtWriter::SaveSrf(const char fn[],const YsShellExt &shl)
{
	FILE *fp=fopen(fn,"w");
	if(NULL!=fp)
	{
		YsTextFileOutputStream outStream(fp);
		auto res=SaveSrf(outStream,shl);
		fclose(fp);
		return res;
	}
	return YSERR;
}

YSRESULT YsShellExtWriter::SaveSrf(YsTextOutputStream &outStream,const YsShellExt &shl)
{
	MakeCeKeyToCeIdentMap(shl);
	MakeFgKeyToFgIdentMap(shl);

	outStream.Printf("Surf\n");

	shl.Encache();

	for(YsShellVertexHandle vtHd=NULL; YSOK==shl.MoveToNextVertex(vtHd); )
	{
		YsVec3 vtPos;
		shl.GetVertexPosition(vtPos,vtHd);

		char round=' ';
		if(YSTRUE==shl.GetVertexAttrib(vtHd)->IsRound())
		{
			round='R';
		}
		outStream.Printf("V %.16lf %.16lf %.16lf %c\n",vtPos.x(),vtPos.y(),vtPos.z(),round);
	}

	for(YsShellVertexHandle vtHd=NULL; YSOK==shl.MoveToNextVertex(vtHd); )
	{
		auto vtAttrib=shl.GetVertexAttrib(vtHd);
		if(0<=vtAttrib->oPlId)
		{
			outStream.Printf("YF %d %d\n",shl.GetVertexIdFromHandle(vtHd),vtAttrib->oPlId);
		}
		else if(0<=vtAttrib->oVtId[0] && 0<=vtAttrib->oVtId[1] && vtAttrib->oVtId[0]!=vtAttrib->oVtId[1])
		{
			outStream.Printf("YE %d %d %d\n",shl.GetVertexIdFromHandle(vtHd),vtAttrib->oVtId[0],vtAttrib->oVtId[1]);
		}
		else if(0<=vtAttrib->oVtId[0] && vtAttrib->oVtId[0]==vtAttrib->oVtId[1])
		{
			outStream.Printf("YN %d %d\n",shl.GetVertexIdFromHandle(vtHd),vtAttrib->oVtId[0]);
		}
	}

	for(YsShellPolygonHandle plHd=NULL; YSOK==shl.MoveToNextPolygon(plHd); )
	{
		int nPlVt;
		const YsShellVertexHandle *plVtHd;
		shl.GetVertexListOfPolygon(nPlVt,plVtHd,plHd);

		YsColor col;
		shl.GetColorOfPolygon(col,plHd);

		YsVec3 nom;
		shl.GetNormalOfPolygon(nom,plHd);

		outStream.Printf("F\n");
		for(YSSIZE_T idx=0; idx<nPlVt; ++idx)
		{
			if(0==idx%16)
			{
				outStream.Printf("V");
			}
			outStream.Printf(" %d",shl.GetVertexIdFromHandle(plVtHd[idx]));
			if(15==idx%16 || idx==nPlVt-1)
			{
				outStream.Printf("\n");
			}
		}
		if(YsOrigin()!=nom)
		{
			outStream.Printf("N 0 0 0 %lf %lf %lf\n",nom.x(),nom.y(),nom.z());
		}
		outStream.Printf("C %d %d %d %d\n",col.Ri(),col.Gi(),col.Bi(),col.Ai());
		outStream.Printf("E\n");
	}

	{
		int n=0;
		for(YsShellPolygonHandle plHd=NULL; YSOK==shl.MoveToNextPolygon(plHd); )
		{
			auto plAttrib=shl.GetPolygonAttrib(plHd);
			if(YSTRUE==plAttrib->GetAsLight())
			{
				if(0==(n%16))
				{
					outStream.Printf("ZL");
				}
				outStream.Printf(" %d",shl.GetPolygonIdFromHandle(plHd));
				if(15==(n%16))
				{
					outStream.Printf("\n");
				}
				++n;
			}
		}
		if(0<n)
		{
			outStream.Printf("\n");
		}
	}

	for(YsShellExt::ConstEdgeHandle ceHd=NULL; YSOK==shl.MoveToNextConstEdge(ceHd); )
	{
		YSSIZE_T nVt;
		const YsShellVertexHandle *ceVtHd;
		YSBOOL isLoop;
		shl.GetConstEdge(nVt,ceVtHd,isLoop,ceHd);

		YSSIZE_T ident;
		ceKeyToCeIdentMap.FindElement(ident,shl.GetSearchKey(ceHd));

		for(YSSIZE_T idx=0; idx<nVt; ++idx)
		{
			if(0==idx%16)
			{
				outStream.Printf("%s %d",(YSTRUE==isLoop ? "GL" : "GE"),(int)ident);
			}
			outStream.Printf(" %d",shl.GetVertexIdFromHandle(ceVtHd[idx]));
			if(15==idx%16 || idx==nVt-1)
			{
				outStream.Printf("\n");
			}
		}

		auto ceLabel=shl.GetConstEdgeLabel(ceHd);
		if(nullptr!=ceLabel && 0!=ceLabel[0])
		{
			const char *tag=(YSTRUE==isLoop ? "LL" : "LE");
			outStream.Printf("%s %d \"%s\"\n",tag,(int)ident,(const char *)ceLabel);
		}
	}

	{
		int ctr=0;
		for(YsShellExt::ConstEdgeHandle ceHd=NULL; YSOK==shl.MoveToNextConstEdge(ceHd); )
		{
			if(YSTRUE==shl.IsCreaseConstEdge(ceHd))
			{
				YSSIZE_T ident;
				ceKeyToCeIdentMap.FindElement(ident,shl.GetSearchKey(ceHd));

				if(0==ctr)
				{
					outStream.Printf("ZCRE");
				}
				outStream.Printf(" %d",(int)ident);
				++ctr;
				if(16==ctr)
				{
					outStream.Printf("\n");
					ctr=0;
				}
			}
		}
		if(0<ctr)
		{
			outStream.Printf("\n");
		}
	}

	for(YsShellExt::FaceGroupHandle fgHd=NULL; YSOK==shl.MoveToNextFaceGroup(fgHd); )
	{
		YSSIZE_T nPl;
		const YsShellPolygonHandle *fgPlHd;
		shl.GetFaceGroup(nPl,fgPlHd,fgHd);

		YSSIZE_T ident;
		fgKeyToFgIdentMap.FindElement(ident,shl.GetSearchKey(fgHd));

		for(YSSIZE_T idx=0; idx<nPl; ++idx)
		{
			if(0==idx%16)
			{
				outStream.Printf("GF %d",(int)ident);
			}
			outStream.Printf(" %d",shl.GetPolygonIdFromHandle(fgPlHd[idx]));
			if(15==idx%16 || idx==nPl-1)
			{
				outStream.Printf("\n");
			}
		}

		const char *label=shl.GetFaceGroupLabel(fgHd);
		if(NULL!=label && 0!=label[0])
		{
			outStream.Printf("LF %d \"%s\"\n",(int)ident,label);
		}

		auto fgProp=shl.GetFaceGroupAttrib(fgHd);
		if(YsTolerance<fgProp.elemSize)
		{
			outStream.Printf("ZF %d %.10lf\n",(int)ident,fgProp.elemSize);
		}

		if(0<fgProp.bLayerSpec.nLayer)
		{
			outStream.Printf("ZFFL %d %d\n",(int)ident,fgProp.bLayerSpec.nLayer);
			outStream.Printf("ZFFF %d %.10lf\n",(int)ident,fgProp.bLayerSpec.firstLayerThickness);
			outStream.Printf("ZFFT %d %d %.10lf\n",(int)ident,fgProp.bLayerSpec.growthFuncType,fgProp.bLayerSpec.growthFuncParam);
		}
	}

	return YSOK;
}

void YsShellExtWriter::MakeCeKeyToCeIdentMap(const YsShellExt &shl)
{
	YSSIZE_T nextCeIdent=0;

	for(YsShellExt::ConstEdgeHandle ceHd=NULL; YSOK==shl.MoveToNextConstEdge(ceHd); )
	{
		YSSIZE_T ceIdent=shl.GetConstEdgeIdent(ceHd);
		nextCeIdent=YsGreater(ceIdent+1,nextCeIdent);
	}

	YsKeyStore usedCeIdent;

	ceKeyToCeIdentMap.CleanUp();
	for(YsShellExt::ConstEdgeHandle ceHd=NULL; YSOK==shl.MoveToNextConstEdge(ceHd); )
	{
		YSSIZE_T ceIdent=shl.GetConstEdgeIdent(ceHd);
		if(0<=ceIdent && YSTRUE!=usedCeIdent.IsIncluded((YSHASHKEY)ceIdent))
		{
			ceKeyToCeIdentMap.AddElement(shl.GetSearchKey(ceHd),(YSHASHKEY)ceIdent);
			usedCeIdent.AddKey((YSHASHKEY)ceIdent);
		}
		else
		{
			ceKeyToCeIdentMap.AddElement(shl.GetSearchKey(ceHd),(YSHASHKEY)nextCeIdent);
			++nextCeIdent;
		}
	}
}

void YsShellExtWriter::MakeFgKeyToFgIdentMap(const YsShellExt &shl)
{
	YSSIZE_T nextFgIdent=0;

	for(YsShellExt::FaceGroupHandle fgHd=NULL; YSOK==shl.MoveToNextFaceGroup(fgHd); )
	{
		YSSIZE_T fgIdent=shl.GetFaceGroupIdent(fgHd);
		nextFgIdent=YsGreater(fgIdent+1,nextFgIdent);
	}

	YsKeyStore usedFgIdent;

	fgKeyToFgIdentMap.CleanUp();
	for(YsShellExt::FaceGroupHandle fgHd=NULL; YSOK==shl.MoveToNextFaceGroup(fgHd); )
	{
		YSSIZE_T fgIdent=shl.GetFaceGroupIdent(fgHd);
		if(0<=fgIdent && YSTRUE!=usedFgIdent.IsIncluded((YSHASHKEY)fgIdent))
		{
			fgKeyToFgIdentMap.AddElement(shl.GetSearchKey(fgHd),(YSHASHKEY)fgIdent);
			usedFgIdent.AddKey((YSHASHKEY)fgIdent);
		}
		else
		{
			fgKeyToFgIdentMap.AddElement(shl.GetSearchKey(fgHd),(YSHASHKEY)nextFgIdent);
			++nextFgIdent;
		}
	}
}

////////////////////////////////////////////////////////////

void YsShellExtObjReader::BeginReadObj(const ReadOption &option)
{
	this->option=option;
	vtHdArray.CleanUp();
	nomArray.CleanUp();
	texCoordArray.CleanUp();

	inPlGrp=YSFALSE;
	currentPlGrpName.Set("");
	currentSmthShadGrpName.Set("");
	currentPlGrp.CleanUp();
}

YSRESULT YsShellExtObjReader::ReadObjOneLine(YsShellExt &shl,YsString &str)
{
	YsArray <YsString,16> args;
	str.Arguments(args);
	if(0<args.GetN())
	{
		if(0==strcmp(args[0],"v"))
		{
			if(args.GetN()<4)
			{
				return YSERR;
			}
			YsVec3 pos(args[1],args[2],args[3]);
			vtHdArray.Append(shl.AddVertex(pos));
		}
		else if(0==strcmp(args[0],"vn"))
		{
			if(args.GetN()<4)
			{
				return YSERR;
			}
			YsVec3 nom(args[1],args[2],args[3]);
			nomArray.Append(nom);
		}
		else if(0==strcmp(args[0],"vt"))
		{
			if(args.GetN()<3)
			{
				return YSERR;
			}
			YsVec2 tex(args[1],args[2]);
			texCoordArray.Append(tex);
		}
		else if(0==strcmp(args[0],"f"))
		{
			YsArray <YsShellVertexHandle> plVtHdArray;
			for(int idx=1; idx<args.GetN(); ++idx)
			{
				int vtIdx,texCoordIdx,nomIdx;
				DecodePlVtId(vtIdx,texCoordIdx,nomIdx,args[idx]);

				--vtIdx;
				--nomIdx;
				--texCoordIdx;

				if(YSTRUE==vtHdArray.IsInRange(vtIdx))
				{
					plVtHdArray.Append(vtHdArray[vtIdx]);
				}
				else
				{
					return YSERR;
				}
			}

			auto plHd=shl.AddPolygon(plVtHdArray);
			if(YSTRUE==inPlGrp)
			{
				currentPlGrp.Append(plHd);
			}
		}
		else if(0==strcmp(args[0],"g") && YSTRUE==option.usePolygonGroupAsFaceGroup)
		{
			if(YSTRUE==inPlGrp)
			{
				YsString fgName=currentPlGrpName;
				fgName.Append(currentSmthShadGrpName);
				auto fgHd=shl.AddFaceGroup(currentPlGrp);
				shl.SetFaceGroupLabel(fgHd,fgName);
			}

			if(2<=args.GetN())
			{
				currentPlGrpName=args[1];
			}
			currentPlGrp.CleanUp();
			inPlGrp=YSTRUE;
		}
		else if(0==strcmp(args[0],"s") && YSTRUE==option.useSmoothShadingGroupAsFaceGroup)
		{
			if(YSTRUE==inPlGrp)
			{
				YsString fgName=currentPlGrpName;
				fgName.Append(currentSmthShadGrpName);
				auto fgHd=shl.AddFaceGroup(currentPlGrp);
				shl.SetFaceGroupLabel(fgHd,fgName);
			}

			if(2<=args.GetN())
			{
				currentSmthShadGrpName=args[1];
			}
			currentPlGrp.CleanUp();
			inPlGrp=YSTRUE;
		}
	}

	return YSOK;
}

void YsShellExtObjReader::EndReadObj(YsShellExt &shl)
{
	if(YSTRUE==inPlGrp)
	{
		auto fgHd=shl.AddFaceGroup(currentPlGrp);
		shl.SetFaceGroupLabel(fgHd,currentPlGrpName);
	}
}

YSRESULT YsShellExtObjReader::ReadObj(YsShellExt &shl,YsTextInputStream &inStream,const ReadOption &option)
{
	YSRESULT res=YSOK;

	BeginReadObj(option);

	YsString str;
	while(NULL!=inStream.Gets(str))
	{
		if(YSOK!=ReadObjOneLine(shl,str))
		{
			res=YSERR;
		}
	}

	EndReadObj(shl);

	return res;
}

void YsShellExtObjReader::DecodePlVtId(int &vtIdx,int &texCoordIdx,int &nomIdx,const YsString &str) const
{
	vtIdx=atoi(str);
	texCoordIdx=-1;
	nomIdx=-1;

	int slashCount=0;
	for(int strPos=0; strPos<str.Strlen(); ++strPos)
	{
		if('/'==str[strPos])
		{
			if('0'<=str[strPos+1] && str[strPos+1]<='9')
			{
				if(0==slashCount)
				{
					texCoordIdx=atoi(str.Txt()+strPos+1);
				}
				else if(1==slashCount)
				{
					nomIdx=atoi(str.Txt()+strPos+1);
				}
			}

			++slashCount;
		}
	}
}

////////////////////////////////////////////////////////////

void YsShellExtObjWriter::BeginWriteObj(const YsShellExtObjWriter::WriteOption &)
{
	plKeyToObjPlIdx.CleanUp();
	plHdArray.CleanUp();
	fgKeyArray.CleanUp();

	vtKeyToNomIdx.CleanUp();
	nomArray.CleanUp();
}

void YsShellExtObjWriter::WriteVertex(YsTextOutputStream &outStream,const YsShellExt &shl)
{
	for(auto vtHd : shl.AllVertex())
	{
		auto vtPos=shl.GetVertexPosition(vtHd);
		outStream.Printf("v %.10lf %.10lf %.10lf\n",vtPos.x(),vtPos.y(),vtPos.z());
	}
}

void YsShellExtObjWriter::MakeNormalArray(const YsShellExt &shl)
{
	for(auto vtHd : shl.AllVertex())
	{
		auto *vtAttrib=shl.GetVertexAttrib(vtHd);
		if(NULL!=vtAttrib && YSTRUE==vtAttrib->IsRound())
		{
			auto vtPlHd=shl.FindPolygonFromVertex(vtHd);

			YsVec3 nomSum=YsOrigin();
			for(auto plHd : vtPlHd)
			{
				YsVec3 nom;
				shl.GetNormal(nom,plHd);
				nomSum+=nom;
			}

			if(YSOK==nomSum.Normalize())
			{
				auto nomIdx=(int)nomArray.GetN();
				nomArray.Append(nomSum);
				vtKeyToNomIdx.AddElement(shl.GetSearchKey(vtHd),nomIdx);
			}
		}
	}
}

void YsShellExtObjWriter::WriteNormal(YsTextOutputStream &outStream)
{
	for(auto nom : nomArray)
	{
		outStream.Printf("vn %lf %lf %.lf\n",nom.x(),nom.y(),nom.z());
	}
}

void YsShellExtObjWriter::SortPolygonByFaceGroupKey(const YsShellExt &shl)
{
	for(auto plHd : shl.AllPolygon())
	{
		plHdArray.Append(plHd);

		auto fgHd=shl.FindFaceGroupFromPolygon(plHd);
		if(NULL!=fgHd)
		{
			fgKeyArray.Append(shl.GetSearchKey(fgHd));
		}
		else
		{
			fgKeyArray.Append(0x7fffffff);  // :-P
		}
	}

	YsQuickSort <YSHASHKEY,YsShellPolygonHandle> (fgKeyArray.GetN(),fgKeyArray,plHdArray);
}

void YsShellExtObjWriter::WritePolygon(YsTextOutputStream &outStream,const YsShellExt &shl)
{
	for(YSSIZE_T idx=0; idx<fgKeyArray.GetN(); ++idx)
	{
		if(0==idx || fgKeyArray[idx-1]!=fgKeyArray[idx])
		{
			outStream.Printf("g\n");
		}

		auto plVtHd=shl.GetPolygonVertex(plHdArray[idx]);
		outStream.Printf("f");
		for(auto vtHd : plVtHd)
		{
			int vtIdx=shl.GetVertexIdFromHandle(vtHd);
			int nomIdx=-1;
			vtKeyToNomIdx.FindElement(nomIdx,shl.GetSearchKey(vtHd));

			++vtIdx;
			++nomIdx;
			if(YSTRUE==nomArray.IsInRange(nomIdx))
			{
				outStream.Printf(" %d//%d",vtIdx,nomIdx);
			}
			else
			{
				outStream.Printf(" %d",vtIdx);
			}
		}
		outStream.Printf("\n");
	}
}

void YsShellExtObjWriter::EndWriteObj(YsTextOutputStream &,const YsShellExt &)
{
}

YSRESULT YsShellExtObjWriter::WriteObj(YsTextOutputStream &outStream,const YsShellExt &shl,const YsShellExtObjWriter::WriteOption &option)
{
	shl.Encache();

	BeginWriteObj(option);

	WriteVertex(outStream,shl);
	MakeNormalArray(shl);
	WriteNormal(outStream);
	SortPolygonByFaceGroupKey(shl);
	WritePolygon(outStream,shl);
	EndWriteObj(outStream,shl);

	return YSOK;
}



void YsShellExtOffReader::BeginReadOff(void)
{
	state=STATE_SKIPPING_OFF;
	nVtx=0;
	nVtxRead=0;
	nPlg=0;
	nPlgRead=0;
	vtHdArray.CleanUp();
}

YSRESULT YsShellExtOffReader::ReadOffOneLine(YsShellExt &shl,YsString &str)
{
	if(STATE_SKIPPING_OFF==state)
	{
		if(0==strncmp(str,"OFF",3))
		{
			state=STATE_GETTING_NUMBERS;
		}
	}
	else if(STATE_GETTING_NUMBERS==state)
	{
		YsArray <YsString> args;
		str.Arguments(args);
		if(2<=args.GetN())
		{
			nVtx=atoi(args[0]);
			nPlg=atoi(args[1]);
			state=STATE_READING_VERTEX;
		}
	}
	else if(STATE_READING_VERTEX==state)
	{
		YsArray <YsString> args;
		str.Arguments(args);
		if(3<=args.GetN())
		{
			const YsVec3 pos(args[0],args[1],args[2]);
			vtHdArray.Append(shl.AddVertex(pos));
			++nVtxRead;
			if(nVtx<=nVtxRead)
			{
				state=STATE_READING_POLYGON;
			}
		}
	}
	else if(STATE_READING_POLYGON==state)
	{
		YsArray <YsString> args;
		str.Arguments(args);
		if(1<=args.GetN())
		{
			YsArray <YsShellVertexHandle,4> plVtHd;
			for(int i=1; i<args.GetN(); ++i)
			{
				const int vtIdx=atoi(args[i]);
				if(YSTRUE==vtHdArray.IsInRange(vtIdx))
				{
					plVtHd.Append(vtHdArray[vtIdx]);
				}
			}

			if(3<=plVtHd.GetN())
			{
				shl.AddPolygon(plVtHd);
				++nPlgRead;
				if(nPlg<=nPlgRead)
				{
					state=STATE_NONE;
				}
			}
		}
	}
	else if(STATE_NONE==state)
	{
	}
	return YSOK;
}

void YsShellExtOffReader::EndReadOff(YsShellExt &)
{
}

YSRESULT YsShellExtOffReader::ReadOff(YsShellExt &shl,YsTextInputStream &inStream)
{
	YSRESULT res=YSOK;

	BeginReadOff();

	YsString str;
	while(NULL!=inStream.Gets(str))
	{
		if(YSOK!=ReadOffOneLine(shl,str))
		{
			res=YSERR;
		}
	}

	EndReadOff(shl);

	return res;
}



YSRESULT YsShellExtOffWriter::WriteOff(YsTextOutputStream &outStream,const YsShellExt &shl)
{
	shl.Encache();

	outStream.Printf("OFF\n");
	outStream.Printf("%d %d 0\n",shl.GetNumVertex(),shl.GetNumPolygon());

	for(auto vtHd : shl.AllVertex())
	{
		const YsVec3 pos=shl.GetVertexPosition(vtHd);
		outStream.Printf("%lf %lf %lf\n",pos.x(),pos.y(),pos.z());
	}

	for(auto plHd : shl.AllPolygon())
	{
		auto plVtHd=shl.GetPolygonVertex(plHd);
		outStream.Printf("%d",plVtHd.GetN());
		for(auto vtHd : plVtHd)
		{
			int id=shl.GetVertexIdFromHandle(vtHd);
			outStream.Printf(" %d",id);
		}
		outStream.Printf("\n");
	}

	return YSOK;
}



////////////////////////////////////////////////////////////
YSRESULT YsShellExt_LoadGeneral(YsShellExt &shl,const char fnIn[])
{
	YsString fn(fnIn);
	auto ext=fn.GetExtension();
	if(0==ext.STRCMP(".SRF"))
	{
		FILE *fp=fopen(fn,"r");
		if(nullptr!=fp)
		{
			shl.CleanUp();

			YsShellExtReader reader;
			YsTextFileInputStream inStream(fp);
			auto res=reader.MergeSrf(shl,inStream);

			fclose(fp);
			return res;
		}
	}
	else if(0==ext.STRCMP(".STL"))
	{
		return shl.LoadStl(fn);
	}
	else if(0==ext.STRCMP(".OFF"))
	{
		FILE *fp=fopen(fn,"r");
		if(nullptr!=fp)
		{
			shl.CleanUp();

			YsShellExtOffReader reader;
			YsTextFileInputStream inStream(fp);
			auto res=reader.ReadOff(shl,inStream);

			fclose(fp);
			return res;
		}
	}
	else if(0==ext.STRCMP(".OBJ"))
	{
		FILE *fp=fopen(fn,"r");
		if(nullptr!=fp)
		{
			shl.CleanUp();

			YsShellExtObjReader::ReadOption defaultOption;

			YsShellExtObjReader reader;
			YsTextFileInputStream inStream(fp);
			auto res=reader.ReadObj(shl,inStream,defaultOption);

			fclose(fp);
			return res;
		}
	}
	return YSERR;
}

YSRESULT YsShellExtEdit_LoadGeneral(YsShellExtEdit &shl,const char fnIn[])
{
	YsString fn(fnIn);
	auto ext=fn.GetExtension();
	if(0==ext.STRCMP(".SRF"))
	{
		return shl.LoadSrf(fn);
	}
	else if(0==ext.STRCMP(".STL"))
	{
		return shl.LoadStl(fn);
	}
	else if(0==ext.STRCMP(".OFF"))
	{
		return shl.LoadOff(fn);
	}
	else if(0==ext.STRCMP(".OBJ"))
	{
		YsShellExtObjReader::ReadOption defaultOption;
		return shl.LoadObj(fn);
	}
	return YSERR;
}

YSRESULT YsShellExt_SaveGeneral(const char fnIn[],const YsShellExt &shl)
{
	YsString fn(fnIn);
	auto ext=fn.GetExtension();
	if(0==ext.STRCMP(".SRF"))
	{
		FILE *fp=fopen(fn,"w");
		if(nullptr!=fp)
		{
			YsShellExtWriter writer;
			YsTextFileOutputStream outStream(fp);
			auto res=writer.SaveSrf(outStream,shl);

			fclose(fp);
			return res;
		}
	}
	else if(0==ext.STRCMP(".STL"))
	{
		FILE *fp=fopen(fn,"wb");
		if(nullptr!=fp)
		{
			auto res=shl.SaveBinStl(fp);
			fclose(fp);
			return res;
		}
	}
	else if(0==ext.STRCMP(".OFF"))
	{
		FILE *fp=fopen(fn,"w");
		if(nullptr!=fp)
		{
			YsShellExtOffWriter writer;
			YsTextFileOutputStream outStream(fp);
			auto res=writer.WriteOff(outStream,shl);

			fclose(fp);
			return res;
		}
	}
	else if(0==ext.STRCMP(".OBJ"))
	{
		FILE *fp=fopen(fn,"w");
		if(nullptr!=fp)
		{
			YsShellExtObjWriter::WriteOption defaultOption;

			YsShellExtObjWriter writer;
			YsTextFileOutputStream outStream(fp);
			auto res=writer.WriteObj(outStream,shl,defaultOption);

			fclose(fp);
			return res;
		}
	}
	return YSERR;
}
