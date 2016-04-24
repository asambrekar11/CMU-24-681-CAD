//#include <ysshellext_geomcalc.h>
#include <ysshellext_localop.h>
#include "lattice.h"
#include "Anchor.h"

using namespace std;



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
            printf("Got the label\n");
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
		for(int i=0;i<temp.size();i++)
		{
            AV.label.push_back(temp[i]);
		}
        //FindAverageAnchorVertex(AV);
		AncPts.push_back(AV);
		
	}

}

}


void AnchorVertex::FindAverageAnchorVertex(AncVtx &vtx)
{
    YsVec3 temp(0.,0.,0.);
    AncVtx vtx1;
    for(int j=0;j<vtx.label.size();j++)
    {
        auto Proxy = MyCl->GetProxy(vtx.label[j]);
        temp = temp + GetProjection(Proxy.ProxyPosition, Proxy.ProxyNormal, vtx.Anchor);
        
    }
    temp = temp/(double)vtx.label.size();
    vtx.Anchor = temp;
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
		
		Vtxlst.push_back(bin);
	}


}

std::vector<PxyVtx> AnchorVertex::GetEdgeVertices(AncVtx vtx1, AncVtx vtx2, int ClusterNum)
{
	int commlabel[2],l=0;
	std::vector<PxyVtx> EdgVtx; 
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

	auto cluster1 = Vtxlst[commlabel[0]];
	auto cluster2 = Vtxlst[commlabel[1]];
	
	for(int i=0;i<cluster1.size();i++)
	{
		auto temp = cluster1[i];
		for(int j=0;j<cluster2.size();j++)
		{
			if(temp.Anchor==cluster2[j].Anchor)//check here
			{	
				if(temp.label1==commlabel[0])
                {
                    temp.label2 = commlabel[1];
                    EdgVtx.push_back(temp);
                }else
                {
                    temp.label2 = commlabel[0];
                    EdgVtx.push_back(temp);
                    
                }
                
               
			}
			
		}
	}
    
    return EdgVtx;

}


void AnchorVertex::AddAncVtx(AncVtx vtx1, AncVtx vtx2, std::vector<PxyVtx> EdgeVtx, int ClusterNum)
{
    
	AncVtxHandle newAnchors;
    auto pxy1 = MyCl->GetProxy(EdgeVtx[0].label1);
	auto pxy2 = MyCl->GetProxy(EdgeVtx[0].label2);
    YsVec3 N1 = pxy1.ProxyNormal, N2 = pxy2.ProxyNormal;
    N1.Normalize();
    N2.Normalize();
    double threshold = 0.8;
    
    AncVtx t;
    double maxD = 0.0;
    int maxi;
	for	(int i=0;i<EdgeVtx.size();i++)
	{
        double D = fabs(sin(acos(N1*N2))*DistancePtToLine(shl->GetVertexPosition(vtx1.Ptr), shl->GetVertexPosition(vtx2.Ptr), shl->GetVertexPosition(EdgeVtx[i].Anchor))/(shl->GetVertexPosition(vtx1.Ptr)-shl->GetVertexPosition(vtx2.Ptr)).GetLength());
        
		if(D>=maxD)
		{
            
            maxD = D;
            maxi = i;
            
            //FindAverageAnchorVertex(t);
        }
	}
    

    
    if(maxD>threshold&&maxD!=0)
    {
        t.Anchor = shl->GetVertexPosition(EdgeVtx[maxi].Anchor);
        t.label.push_back(EdgeVtx[maxi].label1);
        t.label.push_back(EdgeVtx[maxi].label2);
        t.Ptr = EdgeVtx[maxi].Anchor;
        
        AncPts.push_back(t);
        PrxyAnc[t.label[0]].push_back(t);
        PrxyAnc[t.label[1]].push_back(t);
        printf("Adding New Anchor\n");
        AddAncVtx(vtx1, t, GetEdgeVertices(vtx1, t, ClusterNum), ClusterNum);
        AddAncVtx(t, vtx2, GetEdgeVertices(t, vtx2, ClusterNum), ClusterNum);

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
               
                nextHd.pop_back();
                auto EgdeVtx1 = GetEdgeVertices(t, next, ClusterNum);
                AddAncVtx(t,next,EgdeVtx1,ClusterNum);
                
                auto prev = nextHd.back();
                auto EgdeVtx2 = GetEdgeVertices(t, prev, ClusterNum);
                AddAncVtx(t,prev,EgdeVtx2,ClusterNum);
                
                
            }else if(nextHd.size()==1)
            {
                auto next = nextHd.back();
                
                nextHd.pop_back();
                auto EgdeVtx1 = GetEdgeVertices(t, next, ClusterNum);
                AddAncVtx(t,next,EgdeVtx1,ClusterNum);
                
            }
            
        }
		
		t = todo.back();
		todo.pop_back();
		
	}
	
}

void AnchorVertex::IndexLabelling(YsShellExt &newShell)
{
    std::vector <YsShell::VertexHandle> shellvtx;
    for (int i = 0; i<AncPts.size(); i++)
    {
        shellvtx.push_back(AncPts[i].Ptr);
        VertexToLabel.Update(shl->GetSearchKey(AncPts[i].Ptr), i);
    }
    for (auto vtHd : shl->AllVertex())
    {
        double Dmin = 1e6;
        int minlabel;
        for (int i = 0; i<shellvtx.size(); i++)
        {
            double D = (shl->GetVertexPosition(vtHd)-shl->GetVertexPosition(shellvtx[i])).GetLength();
            if (D<Dmin)
            {
                minlabel = i;
                Dmin = D;
            }
        }
        printf("minlabel = %d\n",minlabel);
        VertexToLabel.Update(shl->GetSearchKey(vtHd),minlabel);
    }
    std::vector <YsShell::PolygonHandle> keypolygon;
    std::vector <float> vtx, nom;
    for (auto plHd : shl->AllPolygon())
    {
        auto vt = shl->GetPolygonVertex(plHd);
        int trilabels[3] = {
                            *VertexToLabel[shl->GetSearchKey(vt[0])],
                            *VertexToLabel[shl->GetSearchKey(vt[1])],
                            *VertexToLabel[shl->GetSearchKey(vt[2])],
        };
//        printf("Labels %d %d %d\n",trilabels[0], trilabels[1], trilabels[2]);
        if (trilabels[0] != trilabels[1] &&
            trilabels[1] != trilabels[2] &&
            trilabels[2] != trilabels[0] )
        {
            YsVec3 pt[3] = 	{
            				AncPts[trilabels[0]].Anchor,
							AncPts[trilabels[0]].Anchor,
							AncPts[trilabels[0]].Anchor
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
    printf("vtx size = %d; nom size = %d\n",vtx.size(),nom.size());
    MakeShellFromVtxNom(newShell,vtx,nom);
}