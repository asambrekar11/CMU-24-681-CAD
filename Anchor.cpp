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


void AnchorVertex