#ifndef YSSHELLEXT_ASTAR_IS_INCLUDED
#define YSSHELLEXT_ASTAR_IS_INCLUDED
/* { */

#include <ysshellext.h>

class YsShellExt_AstarSearch
{
private:
	YsShellVertexAttribTable <double> f_score;
	YsShellVertexAttribTable <double> g_score;

	class OpenSet
	{
	public:
		YsShellVertexAttribTable <YsAVLTree <double,YsShell::VertexHandle>::NodeHandle> vtHdToAVLTreeNode;
		YsAVLTree <double,YsShell::VertexHandle> minimumFScoreFinder;

		YSSIZE_T GetN(void) const;
		void SetShell(const YsShell &shl);
		void Update(YsShell::VertexHandle vtHd,const double fScore);
		void DeleteFirst(void);
		YSBOOL IsIncluded(YsShell::VertexHandle vtHd) const;
		YsShell::VertexHandle MinimumFScoreVtHd(void) const;
	};

public:
	YsArray <YsShell::VertexHandle> FindShortestPath(const YsShellExt &shl,YsShell::VertexHandle startVtHd,YsShell::VertexHandle goalVtHd);

	virtual double HeuristicCostEstimate(const YsShellExt &shl,YsShell::VertexHandle fromVtHd,YsShell::VertexHandle toVtHd) const;
	virtual YsArray <YsShell::VertexHandle> GetNeighbor(const YsShellExt &shl,YsShell::VertexHandle fromVtHd) const;
	virtual double GetStepDist(const YsShellExt &shl,YsShell::VertexHandle fromVtHd,YsShell::VertexHandle toVtHd) const;
	double FScore(const YsShellExt &shl,YsShell::VertexHandle vtHd) const;
	double GScore(const YsShellExt &shl,YsShell::VertexHandle vtHd) const;
};



class YsShellExt_AstarSearch_Polygon
{
private:
	YsShellPolygonAttribTable <double> f_score;
	YsShellPolygonAttribTable <double> g_score;

	class OpenSet
	{
	public:
		YsShellPolygonAttribTable <YsAVLTree <double,YsShell::PolygonHandle>::NodeHandle> plHdToAVLTreeNode;
		YsAVLTree <double,YsShell::PolygonHandle> minimumFScoreFinder;

		YSSIZE_T GetN(void) const;
		void SetShell(const YsShell &shl);
		void Update(YsShell::PolygonHandle plHd,const double fScore);
		void DeleteFirst(void);
		YSBOOL IsIncluded(YsShell::PolygonHandle plHd) const;
		YsShell::PolygonHandle MinimumFScorePlHd(void) const;
	};

public:
	YsArray <YsShell::PolygonHandle> FindShortestPath(const YsShellExt &shl,YsShell::PolygonHandle start,YsShell::PolygonHandle goal);

	virtual double HeuristicCostEstimate(const YsShellExt &shl,YsShell::PolygonHandle from,YsShell::PolygonHandle to) const;
	virtual YsArray <YsShell::PolygonHandle> GetNeighbor(const YsShellExt &shl,YsShell::PolygonHandle from) const;
	virtual double GetStepDist(const YsShellExt &shl,YsShell::PolygonHandle from,YsShell::PolygonHandle to) const;
	double FScore(const YsShellExt &shl,YsShell::PolygonHandle vtHd) const;
	double GScore(const YsShellExt &shl,YsShell::PolygonHandle vtHd) const;
};


/* } */
#endif
