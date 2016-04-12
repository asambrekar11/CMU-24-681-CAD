#include "Anchor.h"

using namespace std;

Anchor::~AnchorVertex()
{
for (auto x : AncPts)
	{
		x.clear();
	}

}

void AnchorVertex::Initialize(const Shell &s, const LlyodCluster &MC)
{
shl = &s; MyCl = &MC;
}

void AnchorVertex::MakeAnchorVertex()
{

for(auto vt : shl->AllVertex())
{
	auto Poly = shl->GetVertexPolygon(vt);
	int count = 0;
	std::vector<int> temp;
	for(auto py : Poly)
	{
		int label = MyCl->GetLabelForPolygon(py);
		if(count==0)
		{
			temp.push_back(label);
			count++;
			
		}else
		{
			int flag = 0; //0: mean same label not present 1: means label already exists
			for(auto t : &temp)
			{
				if(label==t)
				{
					flag = 1;
					break;
				}
				
			}
			
			if(flag==0)
			{	
				temp.push_back(label);
				count++;
			}
		}
		
	
	}
	
	if(temp.size()>=3)
	{
		AncVtx AV;
		AV.Anchor = GetVertexPosition(vt);
		for(int i=0;i<temp.size();i++)
		{
			AV.label[i] = temp[i];
		}
		AncPts.push_back(AV);
		
	}

}

}

void AnchorVertex::BinAnchorVertex()//Bins all the anchors into their respective proxies
{

int nProxy = MyCl->GetNumProxy();//Get number of proxies

PrxyAnc = new AncVtxHandle[nProxy];//dynamic memory allocation

	for(auto t : &AncPts)
	{
		for(auto u : &t.label)
		{
			PrxyAnc[u].push_back(t);
		}

	}

}

AncVtxHandle AnchorVertex::GetAnchorVtx(int ProxyNum)//ProxyNum : 0 to k-1 Returns all the anchor vertices associated with given proxy
{

	if(EdgVtx!=nullptr)
	{
		return PrxyAnc[ProxyNum]

	}else
	{
		return nullptr;
	}	
}

AncVtxHandle AnchorVertex::GetNeighbourAnc(AncVtx VtxHd, AncVtxHandle PxHd)//can change to AncVtx on the return type
{
	AncVtxHandle Neighbour;
	
	for(int i=0;i<2&&i<PxHd.size();i++)
	{	
		double minD = 10^6;//Need something better. Does inf work?
		AncVtx temp;
		
		for(int j=0;j<PxHd.size();j++)
		{
			double d = L2Norm(VtxHd.Anchor-PxHd[j].Anchor);
			if(d<minD && d!=0)
			{
				minD = d;
				temp.Anchor = PxHd[j].anchor;
				temp.label.swap(PxHd[j].label);
				
			}
		}
		Neighbour.push_back(temp);
		
		for(int j=0;j<PxHd.size();j++)
		{
			if(PxHd[j].Anchor==temp.Anchor)
			{	
				PxHd.erase(PxHd.begin()+j);
				break;
			}
		}

	}
	
	return Neighbour;
}

void AnchorVertex::AssignLabel()//assign label to each vertex of every polygon in a given proxy
{
	for(int i=0;i<k;i++)
	{	
		auto cluster = MyCl->GetCluster(k);
		std::vector<PxyVtx> bin;
		for(auto PlHd : &cluster)
		{
			for(auto VtHd :&PlHd)
			{
				PxyVtx V;
				V.label1 = k;
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
		
	}

	auto cluster1 = Vtxlst[commlabel[0]];
	auto cluster2 = Vtxlst(commlabel[1]);
	
	for(int i=0;i<cluster1.size();i++)
	{
		auto temp = cluster1[i];
		for(int j=0;j<cluster2.size();j++)
		{
			if(temp.Anchor==cluster2[j].Anchor)
			{	
				temp.label2 = commlabel[1];
				EdgVtx.push_back(temp);	
			}
			
		}
	}

}

AncVtxHandle AnchorVertex::AddAncVtx(AncVtx vtx1, AncVtx vtx2, std::vector<PxyVtx> EdgeVtx)
{
	AncVtxHandle newAnchors;
	auto pxy1 = MyCl->pxy[EdgeVtx[0].label1];
	auto pxy2 = MyCl->pxy[EdgeVtx[0].label2];
	double N1 = L2Norm(pxy1.ProxyNormal), N2 = L2Norm(pxy2.ProxyNormal);;
	double minD = 0;
	for	(int i=0;i<EdgeVtx.size();i++)
	{
		double tempD = sin(N1/N2)*DistancePtToLine(vtx1.Anchor, vtx2.Anchor, EdgeVtx[i].Anchor)*L2Norm(vtx1.Anchor-vtx2.Anchor);
		if(tempD>threshold)
		{
			AncVtx t;
			t.Anchor = EdgeVtx[i].Anchor;
			t.label.push_back(EdgeVtx.label1);
			t.label.push_back(EdgeVtx.label2);
			newAnchors.push_back(t);
		}
		
	}	
	
	retrun newAnchors;
}


void AnchorVertex::ExtractEdges(int ClusterNum)
{
	AncVtxHandle todo,newAncls;
	todo.swap(PrxyAnc[ClusterNum]);
	
	auto t = todo.back();
	todo.pop_back();
	newAncls.push_back(t);
	
	while(todo.size()!=0)
	{
		auto nextHd =  GetNeighbourAnc(t, todo);
		auto next = nextHd.back();
		auto EgdeVtx = GetEdgeVertices(t, next, ClusterNum);
		
		auto newAnc = AddAncVtx(t,next,EdgeVtx);
		
		for(auto i : &newAnc)
		{
			newAncls.push_back(i);
		}
		
		t = todo.back();
		todo.pop_back();
		newAncls.push_back(t);
		
	}
	
	PrxyAnc[ClusterNum].push_back(newAncls);
}