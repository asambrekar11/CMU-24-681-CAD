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
Vec3 Anc;
std::vector<int> label;
};

Class Anhcor
{

protected:
const Shell *shl;
const LlyodCluster *MyCluster;
std::vector<AncVtx> AncPts;


public:
Anchor(const Shell &s, const LlyodCluster &MC);
~Anchor();
//void Initialize(const Shell &s, const LlyodCluster &MyCluster) // Initialize the shell to copy the data structure
void MakeAnchorVertex();
void Projection();
};



#endif */ANCHOR_H*/