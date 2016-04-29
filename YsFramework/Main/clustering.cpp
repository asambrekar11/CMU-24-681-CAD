#include <queue>
#include <functional>
#include <cmath>
#include "clustering.h"

//L2 Error Metric

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

//L2,1 Error Metric

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

void LloydCluster::AssignCenter(const YsShellExt &shl, YsShell::PolygonHandle cntr[], bool first)
{
	// Function to Assign Centers to polygons
    stud.Cleanup();
	boss.Cleanup();
    YsShellPolygonStore visited(shl.Conv());
	
	stud.Resize(pxy.size()/3);
	boss.Resize(shl.GetNumPolygon()/3);
    for (int i = 0; i<k; i++)
    {
//        printf("Iniializing clusters\n");
        std::vector<YsShell::PolygonHandle> entry;
        entry.push_back(cntr[i]);
        auto ptr = stud[pxy[i]];
        if (ptr != nullptr)
        {
            ptr->push_back(cntr[i]);
        }
        else
        {
            stud.Update(pxy[i],entry);
        }
        auto key = shl.GetSearchKey(cntr[i]);
        boss.Update(key,i);
        visited.Add(cntr[i]);
    }
	for (auto plHd : shl.AllPolygon())
	{
        if (YSTRUE != visited.IsIncluded(plHd))
        {
            visited.Add(plHd);
    // 		printf("Triangle %d with %d vertices\n",i, plHd->GetNumVertex());
            std::priority_queue <ClusterNode,std::vector<ClusterNode>,std::greater <ClusterNode>> list;
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
            std::abs(avgN.xf()-pxy[i].ProxyNormal.xf())>1e-1 ||
            std::abs(avgN.yf()-pxy[i].ProxyNormal.yf())>1e-1 ||
            std::abs(avgN.zf()-pxy[i].ProxyNormal.zf())>1e-1 )
		{
// 			printf("Assigned new proxy\n");
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
        bool first = false;
        YsShellPolygonStore visited(shl.Conv());
        
        for (int i = 0; i<k; i++)
        {
            do
            {
                auto plynum = shl.GetNumPolygon();
                long long int idx = rand()%plynum;
//                printf("Initializing triangles\n");
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
            AssignCenter(shl,cntr.data(),first);
            first = true;
//            printf("Assigned centers\n");
        }
        while (true == GetProxy(shl));
   }
   while(true != AllCLustersAssigned());
}