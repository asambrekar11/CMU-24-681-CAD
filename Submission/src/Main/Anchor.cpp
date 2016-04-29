//#include <ysshellext_geomcalc.h>
#include <ysshellext_localop.h>
#include "lattice.h"
#include "Anchor.h"

//using namespace std;



AnchorVertex::~AnchorVertex()
{
AncPts.clear();
shl = nullptr;
MyCl = nullptr;
}

void AnchorVertex::Initialize(const YsShellExt &s, const LloydCluster &MC)
{
shl = &s; MyCl = &MC;
}

void AnchorVertex::MakeAnchorVertex()
{
for(auto vt : shl->AllVertex())
{
    auto Poly = shl->FindPolygonFromVertex(vt);
	int count = 0;
	std::vector<int> temp;
	for(auto &py : Poly)
	{
		const int *label = MyCl->GetPolygonLabel(py);
        if (label != nullptr)
        {
//            printf("Got the label\n");
            if(count == 0)
            {
                temp.push_back(*label);
                count++;
                
            }else
            {
                int flag = 0; //0: mean same label not present 1: means label already exists
                for(auto &t : temp)
                {
                    if(*label==t)
                    {
                        flag = 1;
                        break;
                    }
                    
                }
                
                if(flag==0)
                {	
                    temp.push_back(*label);
                    count++;
                }
            }
        }
		
	
	}
	
	if(temp.size()>=3)
	{
		AncVtx AV;
		AV.Anchor = shl->GetVertexPosition(vt);
        AV.Ptr = vt;
		for(auto i=0;i<temp.size();i++)
		{
            AV.label.push_back(temp[i]);
		}
        //FindAverageAnchorVertex(AV);
		AncPts.push_back(AV);
		
	}

}

}

void AnchorVertex::FindAverageAnchorVertex()
{
    
    for(int i=0;i<AncPts.size();i++)
    {
        YsVec3 temp(0.,0.,0.);
        auto vtPos = shl->GetVertexPosition(AncPts[i].Ptr);
        for(int j=0;j<AncPts[i].label.size();j++)
        {
            auto Proxy = MyCl->GetProxy(AncPts[i].label[j]);

            temp = temp + GetProjection(Proxy.ProxyNormal, Proxy.ProxyPosition, vtPos);

//            temp = temp + GetProjection(Proxy.ProxyNormal,Proxy.ProxyPosition,AncPts[i].Anchor);

            
        }
        temp = temp/(double)AncPts[i].label.size();
        AncPts[i].Anchor = temp;
    }
}
    


void AnchorVertex::BinAnchorVertex()//Bins all the anchors into their respective proxies
{

int nProxy = MyCl->GetNumProxy();//Get number of proxies

    PrxyAnc.resize(nProxy);
//PrxyAnc = new AncVtxHandle[nProxy];//dynamic memory allocation

	for(auto &t : AncPts)
	{

		for(auto &u : t.label)
		{
            
			PrxyAnc[u].push_back(t);
		}
        
	}

}

AncVtxHandle AnchorVertex::GetAnchorVtx(int ProxyNum)//ProxyNum : 0 to k-1 Returns all the anchor vertices associated with given proxy
{

//	if(EdgVtx!=nullptr)
        return PrxyAnc[ProxyNum];

}

AncVtxHandle AnchorVertex::GetNeighbourAnc(AncVtx VtxHd, AncVtxHandle PxHd)//can change to AncVtx on the return type
{
	AncVtxHandle Neighbour;
    
    for(int i=0;i<PxHd.size();i++)
    {
        int count=0;
        for(int j=0;j<VtxHd.label.size();j++)
        {
            for(int m=0;m<PxHd[i].label.size();m++)
            {
                
                if(VtxHd.label[j]==PxHd[i].label[m])
                {
                    count++;
                    
                }
            
            }
            
        }
        if(count==2)
        {
            Neighbour.push_back(PxHd[i]);
        }
    }
	

	return Neighbour;
}

void AnchorVertex::AssignLabel()//assign label to each vertex of every polygon in a given proxy
{
	for(int i=0;i<k;i++)
	{	
		auto cluster = MyCl->GetCluster(i);
		std::vector<PxyVtx> bin;
//        if (nullptr != cluster)
//        {
            for(auto &PlHd : cluster)
            {
                auto PlVtx = shl->GetPolygonVertex(PlHd);
                for(auto &VtHd :PlVtx)
                {
                    PxyVtx V;
                    V.label1 = i;
                    V.Anchor = VtHd;
                    bin.push_back(V);
                    
                }
            } 
//        }
		Vtxlst.push_back(bin);
	}


}

int AnchorVertex::GetCommonLabel(AncVtx vtx1, AncVtx vtx2, int ClusterNum)
{
    int commlabel[2],l=0;
    
    for(int i=0;i<vtx1.label.size();i++)
    {
        for(int j=0;j<vtx2.label.size();j++)
        {
            if(vtx1.label[i]==vtx2.label[j])
            {
                
                commlabel[l]=vtx1.label[i];
                l=l+1;
            }
            
        }
        
        if(l>1)
        {
            break;
        }
        
    }
    
    if(commlabel[0]==ClusterNum)
    {
        return commlabel[1];
    }
    return commlabel[0];
}

std::vector<YsShell::VertexHandle> AnchorVertex::GetEdges(AncVtx vtx1, AncVtx vtx2, int ClusterNum, int Nextlabel)
{

    
    auto cluster1 = Vtxlst[ClusterNum];
    auto cluster2 = Vtxlst[Nextlabel];
    
    auto Poly = shl->FindPolygonFromVertex(vtx1.Ptr);
    YsShellPolygonStore visited(shl->Conv());
    YsShellVertexStore visitedvtx(shl->Conv());
    std::vector<YsShell::VertexHandle> EdgeVtx;
    std::vector<YsShell::PolygonHandle> todoPoly;
    
    for(int i=0;i<Poly.GetN();i++)
    {
        const int *label = MyCl->GetPolygonLabel(Poly[i]);
        
        if(*label==ClusterNum)
        {
            todoPoly.push_back(Poly[i]);
            visited.Add(Poly[i]);
            visitedvtx.Add(vtx1.Ptr);
        }
        
    }
    
    EdgeVtx.push_back(vtx1.Ptr);
    
    for(long long int i=0;i<todoPoly.size();i++)
    {
        YsShell::PolygonHandle newplHd = todoPoly[i];
        auto nEdge = shl->GetPolygonNumVertex(newplHd);
        auto polyvtx = shl->GetPolygonVertex(newplHd);
        
        for(int j=0;j<nEdge;j++)
        {
            auto nei = shl->GetNeighborPolygon(newplHd, j);
            
            if(nullptr!=nei&&YSTRUE!=visited.IsIncluded(nei))
            {
                const int *neilabel = MyCl->GetPolygonLabel(nei);
                visited.Add(nei);
                if(*neilabel==Nextlabel)
                {
                    auto exvtx = polyvtx.GetCyclic(j+1);
                    if (YSTRUE == visitedvtx.IsIncluded(polyvtx[j]))
                    {
                        EdgeVtx.push_back(exvtx);
                        visitedvtx.Add(exvtx);
                        
                    }
                    else
                    {
                        EdgeVtx.push_back(polyvtx[j]);
                        visitedvtx.Add(polyvtx[j]);
                    }
                    if(vtx2.Ptr==exvtx || vtx2.Ptr == polyvtx[j])
                    {
                        return EdgeVtx;
                    }
                    
                }
                else if(ClusterNum==*neilabel)
                {
                    todoPoly.push_back(nei);
                }
            
            }
        }
     
    }
    
    return EdgeVtx;
    
}




void AnchorVertex::AddAncVtx(AncVtx vtx1, AncVtx vtx2, std::vector<YsShell::VertexHandle> EdgeVtx, int ClusterNum, int nextlabel)
{
    
	AncVtxHandle newAnchors;
    auto pxy1 = MyCl->GetProxy(ClusterNum);
	auto pxy2 = MyCl->GetProxy(nextlabel);
    YsVec3 N1 = pxy1.ProxyNormal, N2 = pxy2.ProxyNormal;
    N1.Normalize();
    N2.Normalize();
    double threshold = 0.13;
    
    AncVtx t;
    double maxD = 0.0;
    std::vector<int> maxi;
	for	(long long int i=0;i<EdgeVtx.size();i++)
	{
        double D = fabs(sin(acos(N1*N2))*DistancePtToLine(shl->GetVertexPosition(vtx1.Ptr), shl->GetVertexPosition(vtx2.Ptr), shl->GetVertexPosition(EdgeVtx[i]))/(shl->GetVertexPosition(vtx1.Ptr)-shl->GetVertexPosition(vtx2.Ptr)).GetLength());
        
		if(D>=maxD)
		{
            
            maxD = D;
            maxi.push_back(i);
            
        }
	}
    int imax = 0;
    if (maxi.size()>1)
    {
        for (auto i : maxi)
        {
            imax += i;
        }
        imax /= maxi.size();
    }
    else
        imax = maxi[0];

    
    if(maxD>threshold&&maxD!=0)
    {
        t.Anchor = shl->GetVertexPosition(EdgeVtx[imax]);
        t.label.push_back(ClusterNum);
        t.label.push_back(nextlabel);
        t.Ptr = EdgeVtx[imax];
        
        if(true != IsIncluded(t))
        {
            AncPts.push_back(t);
            PrxyAnc[t.label[0]].push_back(t);
            PrxyAnc[t.label[1]].push_back(t);
//            printf("Adding New Anchor\n");
            AddAncVtx(vtx1, t, GetEdges(vtx1, t, ClusterNum, nextlabel), ClusterNum, nextlabel);
            AddAncVtx(t, vtx2, GetEdges(t, vtx2, ClusterNum, nextlabel), ClusterNum, nextlabel);

        }
        
    }
	
}

void AnchorVertex::ExtractEdges(int ClusterNum)
{
    
    AncVtxHandle todo;
	todo = PrxyAnc[ClusterNum];//Check ProxyAnc after swapping
	
	auto t = todo.back();
	todo.pop_back();
	
	while(todo.size()!=0)
	{
		auto nextHd =  GetNeighbourAnc(t, todo);
        
        if(nextHd.size()!=0)
        {
            if(nextHd.size()==2)
            {
                auto next = nextHd.back();
                int nextlabel = GetCommonLabel(t,next,ClusterNum);
                nextHd.pop_back();
                auto EgdeVtx1 = GetEdges(t, next, ClusterNum,nextlabel);
                AddAncVtx(t,next,EgdeVtx1,ClusterNum,nextlabel);
                
                auto prev = nextHd.back();
                int prevlabel = GetCommonLabel(t,prev,ClusterNum);
                //Check if needed
                nextHd.pop_back();
                auto EgdeVtx2 = GetEdges(t, prev, ClusterNum,prevlabel);
                AddAncVtx(t,prev,EgdeVtx2,ClusterNum,prevlabel);
                
                
            }else if(nextHd.size()==1)
            {
                auto next = nextHd.back();
                int nextlabel = GetCommonLabel(t,next,ClusterNum);
                nextHd.pop_back();
                auto EgdeVtx1 = GetEdges(t, next, ClusterNum,nextlabel);
                AddAncVtx(t,next,EgdeVtx1,ClusterNum,nextlabel);
                
            }
            
        }
		
		t = todo.back();
		todo.pop_back();
		
	}
	
}

bool AnchorVertex::IsIncluded(AncVtx Vtx)
{
    for(int i=0;i<AncPts.size();i++)
    {
        if(AncPts[i].Ptr==Vtx.Ptr)
        {
            return true;
        }
    }
    
    return false;
}

void AnchorVertex::IndexLabelling(YsShellExt &newShell)
{
    std::vector <YsShell::VertexHandle> shellvtx;
    std::vector <YsShell::PolygonHandle> todo;
    
    for (long long int i = 0; i<AncPts.size(); i++)
    {
        shellvtx.push_back(AncPts[i].Ptr);
        VertexToLabel.Update(shl->GetSearchKey(AncPts[i].Ptr), i);
    }
    for (auto vtHd : shl->AllVertex())
    {
        double Dmin = 1e6;
        long long int minlabel;
        for (int i = 0; i<shellvtx.size(); i++)
        {
            double D = (shl->GetVertexPosition(vtHd)-shl->GetVertexPosition(shellvtx[i])).GetLength();
            if (D<Dmin)
            {
                minlabel = i;
                Dmin = D;
            }
        }
        VertexToLabel.Update(shl->GetSearchKey(vtHd),minlabel);
    }
    std::vector <YsShell::PolygonHandle> keypolygon;
    std::vector <float> vtx, nom;
    for (auto plHd : shl->AllPolygon())
    {
        auto vt = shl->GetPolygonVertex(plHd);
        long long int trilabels[3] = {
                            *VertexToLabel[shl->GetSearchKey(vt[0])],
                            *VertexToLabel[shl->GetSearchKey(vt[1])],
                            *VertexToLabel[shl->GetSearchKey(vt[2])],
        };
        if (trilabels[0] != trilabels[1] &&
            trilabels[1] != trilabels[2] &&
            trilabels[2] != trilabels[0] )
        {
            YsVec3 pt[3] = 	{
            				AncPts[trilabels[0]].Anchor,
							AncPts[trilabels[1]].Anchor,
							AncPts[trilabels[2]].Anchor
						};
			YsVec3 BC(0.,0.,0.);
            for (auto &p : pt)
            {
            	BC = BC + p;
            }
            BC = BC / 3.0;
            YsVec3 n(0.,0.,0.);
            for (int i = 0; i<3; i++)
            {
            	n = n + (pt[i]-BC)^(pt[(i+1)%3]-BC);
            }
            
            vtx.push_back(AncPts[trilabels[0]].Anchor.xf());
            vtx.push_back(AncPts[trilabels[0]].Anchor.yf());
            vtx.push_back(AncPts[trilabels[0]].Anchor.zf());
            vtx.push_back(AncPts[trilabels[1]].Anchor.xf());
            vtx.push_back(AncPts[trilabels[1]].Anchor.yf());
            vtx.push_back(AncPts[trilabels[1]].Anchor.zf());
            vtx.push_back(AncPts[trilabels[2]].Anchor.xf());
            vtx.push_back(AncPts[trilabels[2]].Anchor.yf());
            vtx.push_back(AncPts[trilabels[2]].Anchor.zf());
            
            nom.push_back(n.xf());
            nom.push_back(n.yf());
            nom.push_back(n.zf());
            nom.push_back(n.xf());
            nom.push_back(n.yf());
            nom.push_back(n.zf());
            nom.push_back(n.xf());
            nom.push_back(n.yf());
            nom.push_back(n.zf());
            
            
        }
    }
    MakeShellFromVtxNom(newShell,vtx,nom);
}