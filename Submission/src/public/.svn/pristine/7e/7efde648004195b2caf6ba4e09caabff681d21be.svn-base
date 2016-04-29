#ifndef YSSHELLEXT_THICKNESSUTIL_IS_INCLUDED
#define YSSHELLEXT_THICKNESSUTIL_IS_INCLUDED
/* { */

#include <ysshellext.h>

class YsShellExt_ThicknessEstimation
{
public:
	class Sample
	{
	public:
		YsShell::PolygonHandle fromPlHd,toPlHd;
		YsVec3 from,to;
		YSBOOL valid;

		Sample()
		{
			valid=YSTRUE;
		}
	};

	class Preference
	{
	public:
		double opposingAngleThr;                // Unless normals are making greater angle than this threshold, won't take it as a thickness sample.
		YSBOOL excludeVertexConnectedNeighbor;  // Ignore vertex-connected polygons when shooting a ray.
		Preference();
	};

private:
	YsArray <Sample> thicknessSample;

	YsShellPolygonAttribTable <double> plgThickness;
	YsArray <YsShell::PolygonHandle> plgThicknessFilled;

public:
	YsShellExt_ThicknessEstimation();
	~YsShellExt_ThicknessEstimation();
	void CleanUp(void);
	void MakeSample(const YsShellExt &shl,const YsShellLattice &ltc,const Preference &pref,int nThread=8);
private:
	void AddSamplePerPolygon(YsArray <Sample> &sample,const YsShellExt &shl,YsShell::PolygonHandle plHd,const YsShellLattice &ltc,const Preference &pref);
	class MakeSampleThreadInfo;
	static void MakeSampleThreadCallBack(MakeSampleThreadInfo *infoPtr);

public:
	const YsArray <Sample> &GetSample(void) const;


public:
	void TrimSample(const YsShell &shl,int nThread=8);  // See research note 2015/08/13
private:
	class TrimmingLatticeThreadInfo;
	static void MakeTrimmingLatticeThreadCallBack(TrimmingLatticeThreadInfo *infoPtr);
	class TrimSampleThreadInfo;
	static void TrimSampleThreadCallBack(TrimSampleThreadInfo *infoPtr);


public:
	/*! This function will fill the polygons that do not have an assigned thickness. */
	void AssignPolygonThickness(const YsShellExt &shl);
	void SmoothPolygonThickness(const YsShellExt &shl,int nIter,int nThread=8);
	void SmoothPolygonThicknessOneIteration(const YsShellExt &shl,int nThread=8);
	class SmoothPolygonThreadInfo;
	static void SmoothPolygonThreadCallBack(SmoothPolygonThreadInfo *infoPtr);

	YSBOOL IsThicknessAssignedPolygon(YsShell::PolygonHandle plHd) const;
	YSBOOL IsThicknessAssignedPolygon(const YsShell &shl,YsShell::PolygonHandle plHd) const;
	double GetPolygonThickness(YsShell::PolygonHandle plHd) const;
	double GetPolygonThickness(const YsShell &shl,YsShell::PolygonHandle plHd) const;
};


/* } */
#endif
