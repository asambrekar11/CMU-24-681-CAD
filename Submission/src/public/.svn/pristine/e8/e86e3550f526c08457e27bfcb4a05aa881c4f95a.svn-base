#include "ysshellext_regiongrowing.h"



/* virtual */ double YsShellExt_RegionGrowingFromFixedNormal::CalculateMetric(const YsShellExt &shl,RegionHandle rgHd,YsShell::PolygonHandle plHd) const
{
	auto rgPtr=regionArray[rgHd];
	auto nom=shl.GetNormal(plHd);

	return acos(YsBound(nom*rgPtr->seedInfo,-1.0,1.0));
}
