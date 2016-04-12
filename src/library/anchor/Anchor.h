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
};

struct PxyVtx
{
public:
Vec3 Anchor;
int label1,label2;
};

typedef std::vector<AncVtx> AncVtxHandle; 

Class AnhcorVertex
{

protected:
const Shell *shl;
const LlyodCluster *MyCl;
AncVtxHandle AncPts;
AncVtxHandle *PrxyAnc;
std::vector<std::vector<PxyVtx>> Vtxlst;
double threshold;

public:
Anchor();
~Anchor();
void Initialize(const Shell &s, const LlyodCluster &MC) // Initialize the shell to copy the data structure
void MakeAnchorVertex();
//void Projection();
void BinAnchorVertex();
void AssignLabel();
AncVtxHandle GetAnchorVtx(int ProxyNum);
AncVtxHandle GetNeighbourAnc(AncVtx VtxHd, AncVtxHandle PxHd);
std::vector<PxyVtx> GetEdgeVertices(AncVtx vtx1, AncVtx vtx2, int ClusterNum);
AncVtxHandle AddAncVtx(AncVtx vtx1, AncVtx vtx2, std::vector<PxyVtx> EdgeVtx);
void ExtractEdges(int ClusterNum);
void 

};

//replaced std::vector<AncVtx> with AncVtxHandle

#endif */ANCHOR_H*/