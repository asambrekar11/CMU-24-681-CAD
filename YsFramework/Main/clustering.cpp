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

void LloydCluster::AssignCenter(const Shell &shl, Shell::PolygonHandle cntr[])
{
	stud.Cleanup();
	boss.Cleanup();
// 	printf("Number of polygons = %d\n",shl.GetNumPolygon());
	
	
	stud.Resize(pxy.size()/3);
	boss.Resize(shl.GetNumPolygon()/3);
	int i = 0;
	for (auto plHd : shl.AllPolygon())
	{
// 		printf("Triangle %d with %d vertices\n",i, plHd->GetNumVertex());
		std::priority_queue <ClusterNode> list;
		for (int i = 0; i<pxy.size(); i++)
		{
			ClusterNode curr_node(shl,plHd,pxy[i],i);
			list.push(curr_node);
		}
// 		printf("Constructed priority queue\n");
		auto best_node = list.top();
// 		printf("Found the best\n");
        auto curr_proxytable = stud[pxy[best_node.GetLabel()]];
		if (nullptr != curr_proxytable)
		{
// 			printf("I have something here\n");
			curr_proxytable->push_back(best_node.GetPolygonHandle());
		}
		else
		{
// 			printf("I am empty. Please make a new entry");
			std::vector<Shell::PolygonHandle> newEntry;
			newEntry.push_back(best_node.GetPolygonHandle());
			stud.Update(pxy[best_node.GetLabel()],newEntry);
		}
// 		printf("Added to stud\n");
		auto key = shl.GetSearchKey(plHd);
        int label = best_node.GetLabel();
        boss.Update(key,label);
        i++;
	}
}

bool LloydCluster::GetProxy(const Shell &shl, Shell::PolygonHandle cntr[])
{
	bool state = false;
// 	printf("Starting to get proxy\nSize of stud = %d\n",stud.size());
	
	for (int i = 0; i<pxy.size(); i++)
	{
// 		printf("Find new cluster\n");
		auto cluster = stud[pxy[i]];
// 		printf("Found new cluster\nCluster size = %d\n",cluster->size());
		Vec3 BC(0.,0.,0.);
		Vec3 avgN(0.,0.,0.);
// 		printf("Getting new proxy for cluster\n");
		for (auto &plHd : *cluster)
		{
			BC = BC + shl.GetBaryCenter(plHd);
			avgN = avgN + shl.GetNormal(plHd);
		}
		BC = BC / cluster->size();
		avgN = avgN / cluster->size();
		if (BC != pxy[i].ProxyPosition || avgN != pxy[i].ProxyNormal)
		{
// 			printf("Assigned new proxy\n");
			pxy[i].ProxyPosition = BC;
			pxy[i].ProxyNormal = avgN;
			state = true;
		}
	}
	return state;
}

void LloydCluster::MakeCluster(const Shell &shl)
{
	
	Shell::PolygonHandle cntr[pxy.size()];
	PolygonStore visited;
	
	for (int i = 0; i<pxy.size(); i++)
	{
		do
		{
// 			printf("Initializing triangles\n");
			cntr[i] = shl.PickRandomPolygon();
			if (visited.IsIncluded(shl.GetSearchKey(cntr[i])) != true)
			{
				visited.Add(cntr[i]);
				break;
			}
		}
		while (true == visited.IsIncluded(shl.GetSearchKey(cntr[i])));
	}
	
	do
	{
		AssignCenter(shl,cntr);
		printf("Assigned centers\n");
	}
	while (true == GetProxy(shl,cntr));
}