#ifndef CLUSTERING_IS_INCLUDED
#define CLUSTERING_IS_INCLUDED

#include <vector>
#include <math.h>
#include <ysclass.h>
#include <ysshellext.h>
#include "mihashtable.h"
#include "lattice.h"


class Proxy
{
public:
    YsVec3 ProxyPosition, ProxyNormal;
    int label;
    inline friend bool operator==(const Proxy &lhs, const Proxy &rhs)
    {
        return lhs.label==rhs.label;
    }
    inline friend bool operator!=(const Proxy &lhs, const Proxy &rhs)
    {
        return lhs.label!=rhs.label;
    }
};

double L2ErrorMetric(const YsShellExt &shl, YsShell::PolygonHandle plHd, Proxy pxy);

double L21ErrorMetric(const YsShellExt &shl, YsShell::PolygonHandle plHd, Proxy pxy);

class ClusterNode
{
protected:
    YsShell::PolygonHandle plHd;
    int label;
    double error;
public:
    inline friend bool operator>(const ClusterNode &lhs, const ClusterNode &rhs)
    {
        return lhs.error>rhs.error;
    }
    inline friend bool operator>=(const ClusterNode &lhs, const ClusterNode &rhs)
    {
        return lhs.error>=rhs.error;
    }
    inline friend bool operator<(const ClusterNode &lhs, const ClusterNode &rhs)
    {
        return lhs.error<rhs.error;
    }
    inline friend bool operator<=(const ClusterNode &lhs, const ClusterNode &rhs)
    {
        return lhs.error<=rhs.error;
    }
    inline friend bool operator==(const ClusterNode &lhs, const ClusterNode &rhs)
    {
        return lhs.error==rhs.error;
    }
    inline friend bool operator!=(const ClusterNode &lhs, const ClusterNode &rhs)
    {
        return lhs.error!=rhs.error;
    }
    inline ClusterNode()
    {
        plHd = nullptr;
    }
    inline ClusterNode(const YsShellExt &shl, YsShell::PolygonHandle plHd, Proxy pxy, const int label)
    {
        SetPolygon(plHd);
        SetProxy(shl,pxy);
        SetLabel(label);
    }
    inline ~ClusterNode()
    {
        plHd = nullptr;
    }
    inline void SetPolygon(YsShell::PolygonHandle pl)
    {
        plHd = pl;
    }
    inline void SetProxy(const YsShellExt &shl, const Proxy &pxy)
    {
    	error = L21ErrorMetric(shl,this->plHd,pxy);
    }
    inline void SetLabel(int newLabel)
    {
        label = newLabel;
    }
    inline const int GetLabel() const
    {
        return label;
    }
    inline YsShell::PolygonHandle GetPolygonHandle() const
    {
        return plHd;
    }
};

template<>
inline long long int HashTable<Proxy,std::vector<YsShell::PolygonHandle>>::HashCode(const Proxy &key) const
{
    return key.label;
}

template<>
inline long long int HashTable<long long int,int>::HashCode(const long long int &key) const
{
    return key;
}
    

class LloydCluster
{
protected:
    std::vector<Proxy> pxy;
    int k;
    class ProxyTable : public HashTable<Proxy,std::vector<YsShell::PolygonHandle>>
    {
    public:
        inline const std::vector<YsShell::PolygonHandle> &GetPolygonForProxy(const Proxy &pxy) const
        {
            return *(*this)[pxy];
        }
    };
    ProxyTable stud;
    class PolygonTable : public HashTable<long long int,int>
    {
    public:
        const YsShellExt *shl;
        inline PolygonTable()
        {
            shl = nullptr;
        }
        inline ~PolygonTable()
        {
            shl = nullptr;
        }
        inline void Setup(const YsShellExt &shell)
        {
            shl = &shell;
        }
        inline const int *GetLabelForPolygon(YsShell::PolygonHandle plHd) const
        {
            auto key = shl->GetSearchKey(plHd);
            return (*this)[key];
        }
    };
    PolygonTable boss;
    void AssignCenter(const YsShellExt &shl);
    bool GetProxy(const YsShellExt &shl);

public:
    inline LloydCluster()
    {
    }
    inline LloydCluster(const YsShellExt &shl, const int k)
    {
        pxy.resize(k);
        this->k = k;
        boss.Setup(shl);
    }
    inline const std::vector<YsShell::PolygonHandle> &GetCluster(const int idx) const
    {
        return stud.GetPolygonForProxy(pxy[idx]);
    }
    inline const Proxy &GetProxy(const int idx) const
    {
        return pxy[idx];
    }
    inline const int GetNumProxy() const
    {
        return pxy.size();
    }
    inline const int *GetPolygonLabel(const YsShell::PolygonHandle plHd) const
    {
        return boss.GetLabelForPolygon(plHd);
    }
    void MakeCluster(const YsShellExt &shl);
};

inline extern double DistancePtToLine(YsVec3 x1, YsVec3 x2, YsVec3 x0)

{
    double t = ((x0-x1)*(x2-x1))/pow((x2-x1).GetLength(),2.0);
    auto temp = (x1+(x2-x1)*t);
    return (temp-x0).GetLength();
}

inline extern YsVec3 GetProjection(YsVec3 Normal, YsVec3 PtPlane, YsVec3 Pt)
{
    Normal.Normalize();
    double d = Normal*(PtPlane-Pt);
    return Pt - Normal*d;
}

inline void GetBoundingBox(YsVec3 &min, YsVec3 &max, std::vector<float> vtx)
{
    auto nVtx=vtx.size()/3;
    if(0==nVtx)
    {
        min=YsVec3::Origin();
        max=YsVec3::Origin();
    }
    else
    {
        YsBoundingBoxMaker3 mkBbx;
        for(decltype(nVtx) i=0; i<nVtx; ++i)
        {
            YsVec3 pos(vtx[i*3],vtx[i*3+1],vtx[i*3+2]);
            mkBbx.Add(pos);
        }
        mkBbx.Get(min,max);
    }
}


inline void PrepareLatticeForConnection(Lattice3d<std::vector <YsShell::VertexHandle>> &ltc, std::vector <float> vtx)
{
    YsVec3 min,max;
    GetBoundingBox(min,max,vtx);
    double d=(max-min).GetLength()/100.0;
    min-=YsXYZ()*d; // Make absolutely sure that all vertices are inside.
    max+=YsXYZ()*d;
    
    auto nVtx=vtx.size()/3;
    ltc.Create(100,100,100);
    ltc.SetDimension(min,max);
}


inline void MakeShellFromVtxNom(YsShellExt &shl, const std::vector <float> &vtx, const std::vector <float> &nom)
{
    
    Lattice3d <std::vector <YsShell::VertexHandle> > ltc;
    PrepareLatticeForConnection(ltc,vtx);
    
    shl.CleanUp();
    for(int i=0; i<vtx.size()/9; ++i)
    {
        const YsVec3 vtNom(nom[i*9],nom[i*9+1],nom[i*9+2]);
        const YsVec3 vtPos[3]=
        {
            YsVec3(vtx[i*9  ],vtx[i*9+1],vtx[i*9+2]),
            YsVec3(vtx[i*9+3],vtx[i*9+4],vtx[i*9+5]),
            YsVec3(vtx[i*9+6],vtx[i*9+7],vtx[i*9+8]),
        };
        YsShell::VertexHandle vtHd[3];
        for(int i=0; i<3; ++i)
        {
            vtHd[i]=nullptr;
            auto idx=ltc.GetBlockIndex(vtPos[i]);
            if(true==ltc.IsInRange(idx))
            {
                for(auto tstVtHd : ltc.Elem(idx.x(),idx.y(),idx.z()))
                {
                    if(shl.GetVertexPosition(tstVtHd)==vtPos[i])
                    {
                        vtHd[i]=tstVtHd;
                        break;
                    }
                }
            }
            if(nullptr==vtHd[i])
            {
                vtHd[i]=shl.AddVertex(vtPos[i]);
                if(true==ltc.IsInRange(idx))
                {
                    ltc.Elem(idx.x(),idx.y(),idx.z()).push_back(vtHd[i]);
                }
            }
        }
        YsShell::PolygonHandle plHd;
        plHd=shl.AddPolygon(3,vtHd);
        shl.SetPolygonNormal(plHd,vtNom);
        shl.SetPolygonColor(plHd,YsBlue());
    }
}

#endif
