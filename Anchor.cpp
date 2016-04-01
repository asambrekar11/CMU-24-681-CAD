#include"Anchor.h"

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

void AnchorVertex::BinAnchorVertex()
{

int nProxy = MyCl->GetNumProxy();//Get number of proxies

EdgVtx = new AncVtxHandle[nProxy];//dynamic memory allocation

	for(auto t : &AncPts)
	{
		for(auto u : &t.label)
		{
			EdgVtx[u].push_back(t);
		}

	}

}

AncVtxHandle AnchorVertex::GetAnchorVtx(int ProxyNum)//0 to k-1
{

	if(EdgVtx!=nullptr)
	{
		return EdgVtx[ProxyNum]

	}else
	{
		return nullptr;
	}	
}

AncVtxHandle AnchorVertex::GetNeighbour(AncVtx VtxHd, AncVtxHandle PxHd)
{
	AncVtxHandle Neighbour;
	for(int i=0;i<2;i++)
	{	
		double minD = 10^4;//Need something better
		AncVtx temp;
		
		for(int j=0;j<PxHd.size();j++)
		{
			double d = VecN<double,3>L2Norm(VtxHd.Anchor,PxHd[j].Anchor);
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

