#include <queue>
#include <functional>
#include "clustering.h"

double L2ErrorMetric(const Shell &shl, Shell::PolygonHandle plHd, Proxy pxy)
{
	double d[3];
	double A = L2Norm(shl.GetPolygonArea(plHd));
	auto vt = shl.GetPolygonVertex(plHd);
	for (int i = 0; i<vt.size(); i++)
	{
		auto vtx = shl.GetVertexPosition(vt[i]);
		d[i] = L2Norm(Vec3::GetProjection(pxy.ProxyNormal,pxy.ProxyPosition,vtx)-vtx);
	}
	return A*(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]+d[0]*d[1]+d[1]*d[2]+d[2]*d[0])/6.0;
}

double L21ErrorMetric(const Shell &shl, Shell::PolygonHandle plHd, Proxy pxy)
{
	double A = L2Norm(shl.GetPolygonArea(plHd));
	return pow(L2Norm(shl.GetNormal(plHd)-pxy.ProxyNormal),2)*A;
}

bool IsIncluded(Shell::PolygonHandle plHd, Shell::PolygonHandle plList[], int size)
{
	for (int i = 0; i<size; i++)
	{
		if (plHd == plList[i])
		{
			return true;
		}
	}
	return false;
}

template<int k>
void LloydCluster<k>::AssignCenter(const Shell &shl, Shell::PolygonHandle cntr[])
{
	stud.Cleanup();
	boss.Cleanup();
	
	stud.Resize(k/3);
	boss.Resize(shl.GetNumPolygon()/3);
	
	for (auto plHd : shl.AllPolygon())
	{
		std::priority_queue <ClusterNode> list;
		for (int i = 0; i<k; i++)
		{
			ClusterNode curr_node(shl,plHd,pxy[i],i);
			list.push(curr_node);
		}
		auto best_node = list.top();
        auto curr_proxytable = stud[pxy[best_node.GetLabel()]];
		if (nullptr != curr_proxytable)
		{
			curr_proxytable->push_back(best_node.GetPolygonHandle());
		}
		else
		{
			std::vector<Shell::PolygonHandle> newEntry;
			newEntry.push_back(best_node.GetPolygonHandle());
			stud.Update(pxy[best_node.GetLabel()],newEntry);
		}
		auto key = shl.GetSearchKey(plHd);
        boss.Update(key,best_node.GetLabel());
	}
}

template <int k>
bool LloydCluster<k>::GetProxy(const Shell &shl, Shell::PolygonHandle cntr[])
{
	bool state = false;
	for (int i = 0; i<k; i++)
	{
		auto cluster = *(stud[pxy[i]]);
		Vec3 BC(0.,0.,0.);
		Vec3 avgN(0.,0.,0.);
		for (auto &plHd : cluster)
		{
			BC += shl.GetBaryCenter(plHd);
			avgN += shl.GetNormal(plHd);
		}
		BC = BC / cluster.size();
		avgN = avgN / cluster.size();
		if (BC != pxy[i].ProxyPosition || avgN != pxy[i].ProxyNormal)
		{
			pxy[i].ProxyPosition = BC;
			pxy[i].ProxyNormal = avgN;
			state = true;
		}
	}
	return state;
}

template <int k>
void LloydCluster<k>::MakeCluster(const Shell &shl)
{
	
	Shell::PolygonHandle cntr[k];
	
	for (int i = 0; i<k; i++)
	{
		do
		{
			cntr[i] = shl.PickRandomPolygon();
		}
		while (true == IsIncluded(cntr[i],cntr,k));
	}
	
	do
	{
		AssignCenter(shl,cntr);
	}
	while (true == GetProxy(shl,cntr));
}