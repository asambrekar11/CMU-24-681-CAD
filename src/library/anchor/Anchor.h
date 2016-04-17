#include<stdio.h>
#include<vector>
#include<math.h>
#include"shell.h"
#include"mihashtable.h"
#include"vector_nak.h"
#include"clustering.h"

using namespace std;

#ifndef ANCHOR_H
#define ANCHOR_H

struct AncVtx
{
public:
Vec3 Anchor;
std::vector<int> label;
Shell::VertexHandle Ptr;
};

struct PxyVtx
{
public:
Shell::VertexHandle Anchor;
int label1,label2;
};

typedef std::vector<AncVtx> AncVtxHandle; 

template<int k>
class AnchorVertex
{
protected:
const Shell *shl;
const LloydCluster<k> *MyCl;
AncVtxHandle AncPts;
std::vector<AncVtxHandle> PrxyAnc;
std::vector<std::vector<PxyVtx>> Vtxlst;
double threshold;

public:
AnchorVertex();
~AnchorVertex();
void Initialize(const Shell &s, const LloydCluster<k> &MC);
void MakeAnchorVertex();
void BinAnchorVertex();
AncVtx FindAverageAnchorVertex(AncVtx vtx);
void AssignLabel();
AncVtxHandle GetAnchorVtx(int ProxyNum);
AncVtxHandle GetNeighbourAnc(AncVtx VtxHd, AncVtxHandle PxHd);
std::vector<PxyVtx> GetEdgeVertices(AncVtx vtx1, AncVtx vtx2, int ClusterNum);
void AddAncVtx(AncVtx vtx1, AncVtx vtx2, std::vector<PxyVtx> EdgeVtx, int ClusterNum);
void ExtractEdges(int ClusterNum);


};

//replaced std::vector<AncVtx> with AncVtxHandle

#endif */ANCHOR_H*/