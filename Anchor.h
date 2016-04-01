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

Class AnhcorVertex
{

protected:
const Shell *shl;
const LlyodCluster *MyCl;
std::vector<AncVtx> AncPts;


public:
~Anchor();
void Initialize(const Shell &s, const LlyodCluster &MC) // Initialize the shell to copy the data structure
void MakeAnchorVertex();
void Projection();
};



#endif */ANCHOR_H*/