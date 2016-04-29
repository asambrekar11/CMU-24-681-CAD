/* ////////////////////////////////////////////////////////////

File Name: workorder_facegroup.cpp
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

#include <ysshellextedit_facegrouputil.h>
#include "workorder.h"



YSRESULT GeblCmd_WorkOrder::RunFaceGroupWorkOrder(const YsString &workOrder,const YsArray <YsString,16> &args)
{
	if(2<=args.GetN())
	{
		YsString subCmd=args[1];
		subCmd.Capitalize();
		if(0==strcmp(args[1],"ELEMSIZE"))
		{
			return RunFaceGroupAssignElemSize(workOrder,args);
		}
		else if(0==strcmp(args[1],"BLAYER"))
		{
			return RunFaceGroupAssignBoundaryLayerSpecification(workOrder,args);
		}
		else if(0==strcmp(args[1],"CLEARBLAYER"))
		{
			return RunFaceGroupRemoveBoundaryLayerSpecification(workOrder,args);
		}
		else if(0==args[1].STRCMP("CREATE"))
		{
			return RunFaceGroupCreate(workOrder,args);
		}

		YsString errorReason;
		errorReason.Printf("Unrecognized sub command [%s]",args[1].Txt());
		ShowError(workOrder,errorReason);
	}
	else
	{
		ShowError(workOrder,"Sub-command not given.");
	}
	return YSERR;
}

YSRESULT GeblCmd_WorkOrder::RunFaceGroupAssignElemSize(const YsString &workOrder,const YsArray <YsString,16> &args)
{
	auto &shl=*slHd;
	if(0==args[2].STRCMP("ALL"))
	{
		for(auto fgHd : shl.AllFaceGroup())
		{
			auto fgAttrib=shl.GetFaceGroupAttrib(fgHd);
			fgAttrib.SetElemSize(atof(args[3]));
			shl.SetFaceGroupAttrib(fgHd,fgAttrib);
		}
	}
	else
	{
		for(YSSIZE_T idx=3; idx<=args.GetN()-2; idx+=2)
		{
			auto fgHdArray=cmdSupport.GetFaceGroupHandle(shl.Conv(),args[2],args[idx]);
			if(0<fgHdArray.GetN())
			{
				for(auto fgHd :fgHdArray)
				{
					auto fgAttrib=shl.GetFaceGroupAttrib(fgHd);
					fgAttrib.SetElemSize(atof(args[idx+1]));
					shl.SetFaceGroupAttrib(fgHd,fgAttrib);
				}
			}
			else
			{
				YsString errorReason;
				errorReason.Printf("Face Group (%s,%s) does not exist.",args[2].Txt(),args[idx].Txt());
				ShowError(workOrder,errorReason);
				return YSERR;
			}
		}
	}
	return YSOK;
}

YSRESULT GeblCmd_WorkOrder::RunFaceGroupAssignBoundaryLayerSpecification(const YsString &workOrder,const YsArray <YsString,16> &args)
{
	auto &shl=*slHd;
	if(0==args[2].STRCMP("ALL"))
	{
		for(auto fgHd : shl.AllFaceGroup())
		{
			auto fgAttrib=shl.GetFaceGroupAttrib(fgHd);
			fgAttrib.SetBoundaryLayerSpecification(atoi(args[3]),atof(args[4]),atoi(args[5]),atof(args[6]));
			shl.SetFaceGroupAttrib(fgHd,fgAttrib);
		}
	}
	else
	{
		for(YSSIZE_T idx=3; idx<=args.GetN()-5; idx+=5)
		{
			auto fgHdArray=cmdSupport.GetFaceGroupHandle(shl.Conv(),args[2],args[idx]);
			if(0<fgHdArray.GetN())
			{
				for(auto fgHd :fgHdArray)
				{
					auto fgAttrib=shl.GetFaceGroupAttrib(fgHd);
					fgAttrib.SetBoundaryLayerSpecification(atoi(args[idx+1]),atof(args[idx+2]),atoi(args[idx+3]),atof(args[idx+4]));
					shl.SetFaceGroupAttrib(fgHd,fgAttrib);
				}
			}
			else
			{
				YsString errorReason;
				errorReason.Printf("Face Group (%s,%s) does not exist.",args[2].Txt(),args[idx].Txt());
				ShowError(workOrder,errorReason);
				return YSERR;
			}
		}
	}
	return YSOK;
}

YSRESULT GeblCmd_WorkOrder::RunFaceGroupRemoveBoundaryLayerSpecification(const YsString &workOrder,const YsArray <YsString,16> &args)
{
	auto &shl=*slHd;
	if(0==args[2].STRCMP("ALL"))
	{
		for(auto fgHd : shl.AllFaceGroup())
		{
			auto fgAttrib=shl.GetFaceGroupAttrib(fgHd);
			fgAttrib.SetBoundaryLayerSpecification(0,0.0,0,0.0);
			shl.SetFaceGroupAttrib(fgHd,fgAttrib);
		}
	}
	else
	{
		for(YSSIZE_T idx=3; idx<args.GetN(); ++idx)
		{
			auto fgHdArray=cmdSupport.GetFaceGroupHandle(shl.Conv(),args[2],args[idx]);
			if(0<fgHdArray.GetN())
			{
				for(auto fgHd :fgHdArray)
				{
					auto fgAttrib=shl.GetFaceGroupAttrib(fgHd);
					fgAttrib.SetBoundaryLayerSpecification(0,0.0,0,0.0);
					shl.SetFaceGroupAttrib(fgHd,fgAttrib);
				}
			}
			else
			{
				YsString errorReason;
				errorReason.Printf("Face Group (%s,%s) does not exist.",args[2].Txt(),args[idx].Txt());
				ShowError(workOrder,errorReason);
				return YSERR;
			}
		}
	}
	return YSOK;
}

YSRESULT GeblCmd_WorkOrder::RunFaceGroupCreate(const YsString &workOrder,const YsArray <YsString,16> &args)
{
	if(2<=args.GetN() && 0==args[2].STRCMP("FROMCE"))
	{
		YsShellExtEdit_MakeFaceGroupByConstEdge(*slHd);
		return YSOK;
	}
	YsString errorReason;
	errorReason.Printf("Unrecognized sub command parameter [%s]",args[2].Txt());
	ShowError(workOrder,errorReason);
	return YSERR;
}
