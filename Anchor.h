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

typedef std::vector<AncVtx> AncVtxHandle; 

Class AnhcorVertex
{

protected:
const Shell *shl;
const LlyodCluster *MyCl;
AncVtxHandle AncPts;
AncVtxHandle *EdgVtx;

public:
Anchor();
~Anchor();
void Initialize(const Shell &s, const LlyodCluster &MC) // Initialize the shell to copy the data structure
void MakeAnchorVertex();
//void Projection();
void BinAnchorVertex();
AncVtxHandle GetAnchorVtx(int ProxyNum);
AncVtxHandle GetNeighbour(AncVtx VtxHd, AncVtxHandle PxHd);
void ExtractEdges();

};

//replaced std::vector<AncVtx> with AncVtxHandle

#endif */ANCHOR_H*/