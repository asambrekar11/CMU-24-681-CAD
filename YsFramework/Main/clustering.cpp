#include <queue>
#include <functional>
#include <cmath>
#include "clustering.h"

double L2ErrorMetric(const YsShellExt &shl, YsShell::PolygonHandle plHd, Proxy pxy)
{
	double d[3];
    double A = shl.GetPolygonArea(plHd);
	auto vt = shl.GetPolygonVertex(plHd);
	for (int i = 0; i<vt.GetN(); i++)
	{
		auto vtx = shl.GetVertexPosition(vt[i]);
        d[i] = std::abs((GetProjection(pxy.ProxyNormal,pxy.ProxyPosition,vtx)-vtx).GetLength());
	}
	return A*(d[0]*d[0]+d[1]*d[1]+d[2]*d[2]+d[0]*d[1]+d[1]*d[2]+d[2]*d[0])/6.0;
}

double L21ErrorMetric(const YsShellExt &shl, YsShell::PolygonHandle plHd, Proxy pxy)
{
	double A = shl.GetPolygonArea(plHd);
    return pow((shl.GetNormal(plHd)-pxy.ProxyNormal).GetLength(),2)*std::abs(A);
}

bool IsIncluded(YsShell::PolygonHandle plHd, YsShell::PolygonHandle plList[], int size)
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

void LloydCluster::AssignCenter(const YsShellExt &shl)
{
	stud.Cleanup();
	boss.Cleanup();
// 	printf("Number of polygons = %d\n",shl.GetNumPolygon());
	
	
	stud.Resize(pxy.size()/3);
	boss.Resize(shl.GetNumPolygon()/3);
//	int i = 0;
	for (auto plHd : shl.AllPolygon())
	{
// 		printf("Triangle %d with %d vertices\n",i, plHd->GetNumVertex());
        std::priority_queue <ClusterNode,std::vector<ClusterNode>,std::greater<ClusterNode>> list;
		for (int i = 0; i<pxy.size(); i++)
		{
			ClusterNode curr_node(shl,plHd,pxy[i],i);
			list.push(curr_node);
		}
// 		printf("Constructed priority queue\n");
		auto best_node = list.top();
        //printf("Best Error = %lf for label %d\n",best_node.GetError(),best_node.GetLabel());
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
			std::vector<YsShell::PolygonHandle> newEntry;
			newEntry.push_back(best_node.GetPolygonHandle());
			stud.Update(pxy[best_node.GetLabel()],newEntry);
		}
// 		printf("Added to stud\n");
		auto key = shl.GetSearchKey(plHd);
        int label = best_node.GetLabel();
        boss.Update(key,label);
//        i++;
	}
}

bool LloydCluster::GetProxy(const YsShellExt &shl)
{
	bool state = false;
// 	printf("Starting to get proxy\nSize of stud = %d\n",stud.size());
	
	for (int i = 0; i<pxy.size(); i++)
	{
// 		printf("Find new cluster\n");
		auto cluster = stud[pxy[i]];
// 		printf("Found new cluster\nCluster size = %d\n",cluster->size());
		YsVec3 BC(0.,0.,0.);
		YsVec3 avgN(0.,0.,0.);
// 		printf("Getting new proxy for cluster\n");
        if (nullptr != cluster)
        {
            for (auto plHd : *cluster)
            {
                BC += shl.GetCenter(plHd);
                avgN += shl.GetNormal(plHd);
            }
            BC /= cluster->size();
            avgN /= cluster->size();
        }
        else
        {
            printf("Cluster is a nullptr\n");
        }
		if (/*BC != pxy[i].ProxyPosition || avgN != pxy[i].ProxyNormal*/
            std::abs(BC.xf()-pxy[i].ProxyPosition.xf())>1e-1 ||
            std::abs(BC.yf()-pxy[i].ProxyPosition.yf())>1e-1 ||
            std::abs(BC.zf()-pxy[i].ProxyPosition.zf())>1e-1 )
		{
 			printf("Assigned new proxy\n");
			pxy[i].ProxyPosition = BC;
			pxy[i].ProxyNormal = avgN;
//            pxy[i].label=i;
			state = true;
		}
	}
	return state;
}

void LloydCluster::MakeCluster(const YsShellExt &shl)
{
	
    do
    {
        std::vector<YsShell::PolygonHandle> cntr;
        cntr.clear();
        cntr.resize(k);
        
        YsShellPolygonStore visited(shl.Conv());
        
        for (int i = 0; i<k; i++)
        {
            do
            {
                auto plynum = shl.GetNumPolygon();
                long long int idx = rand()%plynum;
                            printf("Initializing triangles\n");
                cntr[i] = shl.GetPolygonHandleFromId(idx);
                pxy[i].ProxyPosition=shl.GetCenter(cntr[i]);
                pxy[i].ProxyNormal=shl.GetNormal(cntr[i]);
                pxy[i].label = i;
                if (visited.IsIncluded(cntr[i]) != true)
                {
                    visited.Add(cntr[i]);
                    break;
                }
            }
            while (1);
            
        }
        
        
        do
        {
            AssignCenter(shl);
            printf("Assigned centers\n");
        }
        while (true == GetProxy(shl));
   }
   while(true != AllCLustersAssigned());
}