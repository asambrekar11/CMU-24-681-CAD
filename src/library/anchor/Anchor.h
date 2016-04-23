#ifndef ANCHOR_H
#define ANCHOR_H

#include<stdio.h>
#include<vector>
#include<math.h>
#include<shell.h>
#include<mihashtable.h>
#include<vector_nak.h>
#include<clustering.h>

using namespace std;

template<>
inline long long int HashTable<Shell::VertexHandle,int>::HashCode(const Shell::VertexHandle &key) const
{
    return (long long int)key;
}


class AncVtx
{
public:
    Vec3 Anchor;
    std::vector<int> label;
    Shell::VertexHandle Ptr;
    
public:
    inline AncVtx &operator=(AncVtx &incoming)
    {
        if(this!=&incoming)
        {
            this->Anchor = incoming.Anchor;
            this->label = incoming.label;
            this->Ptr = incoming.Ptr;
        }
        
        return *this;
    }
        

};

struct PxyVtx
{
public:
    Shell::VertexHandle Anchor;
    int label1,label2;
};

typedef std::vector<AncVtx> AncVtxHandle; 

class AnchorVertex
{
protected:
    int k;
    const Shell *shl;
    const LloydCluster *MyCl;
    AncVtxHandle AncPts;
    std::vector<AncVtxHandle> PrxyAnc;
    std::vector<std::vector<PxyVtx>> Vtxlst;
    class VertexTable : public HashTable<Shell::VertexHandle,int>
    {
    public:
        inline int GetLabelForVertex(Shell::VertexHandle vtHd)
        {
            return *(*this)[vtHd];
        }
    };
public:
    VertexTable VertexToLabel;
    inline AnchorVertex()
    {
        
    }
    inline AnchorVertex(int k)
    {
        this->k=k;
    }
//    ~AnchorVertex();
    void Initialize(const Shell &s, const LloydCluster &MC);
    void MakeAnchorVertex();
    void BinAnchorVertex();
    void FindAverageAnchorVertex(AncVtx &vtx);
    void AssignLabel();
    AncVtxHandle GetAnchorVtx(int ProxyNum);
    AncVtxHandle GetNeighbourAnc(AncVtx VtxHd, AncVtxHandle PxHd);
    std::vector<PxyVtx> GetEdgeVertices(AncVtx vtx1, AncVtx vtx2, int ClusterNum);
    void AddAncVtx(AncVtx vtx1, AncVtx vtx2, std::vector<PxyVtx> EdgeVtx, int ClusterNum);
    void ExtractEdges(int ClusterNum);
    Shell IndexLabelling();

};


#endif