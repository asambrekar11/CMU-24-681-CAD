#ifndef CLUSTERING_IS_INCLUDED
#define CLUSTERING_IS_INCLUDED

#include <vector>
#include <math.h>
#include <vector_nak.h>
#include <mihashtable.h>
#include <shell.h>

class Proxy
{
public:
    Vec3 ProxyPosition, ProxyNormal;
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

double L2ErrorMetric(const Shell &shl, Shell::PolygonHandle plHd, Proxy pxy);

double L21ErrorMetric(const Shell &shl, Shell::PolygonHandle plHd, Proxy pxy);

class ClusterNode
{
protected:
    Shell::PolygonHandle plHd;
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
    inline ClusterNode(const Shell &shl, Shell::PolygonHandle plHd, Proxy pxy, const int label)
    {
        SetPolygon(plHd);
        SetProxy(shl,pxy);
        SetLabel(label);
    }
    inline ~ClusterNode()
    {
        plHd = nullptr;
    }
    inline void SetPolygon(Shell::PolygonHandle pl)
    {
        plHd = pl;
    }
    inline void SetProxy(const Shell &shl, const Proxy &pxy)
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
    inline Shell::PolygonHandle GetPolygonHandle() const
    {
        return plHd;
    }
};

template<>
inline long long int HashTable<Proxy,std::vector<Shell::PolygonHandle>>::HashCode(const Proxy &key) const
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
    class ProxyTable : public HashTable<Proxy,std::vector<Shell::PolygonHandle>>
    {
    public:
        inline const std::vector<Shell::PolygonHandle> &GetPolygonForProxy(const Proxy &pxy) const
        {
            return *(*this)[pxy];
        }
    };
    ProxyTable stud;
    class PolygonTable : public HashTable<long long int,int>
    {
    public:
        const Shell *shl;
        inline PolygonTable()
        {
            shl = nullptr;
        }
        inline ~PolygonTable()
        {
            shl = nullptr;
        }
        inline void Setup(const Shell &shell)
        {
            shl = &shell;
        }
        inline const int GetLabelForPolygon(Shell::PolygonHandle plHd) const
        {
            return *((*this)[shl->GetSearchKey(plHd)]);
        }
    };
    PolygonTable boss;
    void AssignCenter(const Shell &shl, Shell::PolygonHandle cntr[]);
    bool GetProxy(const Shell &shl, Shell::PolygonHandle cntr[]);

public:
    inline LloydCluster()
    {
        
    }
    inline LloydCluster(int k)
    {
        pxy.resize(k);
    }
    inline const std::vector<Shell::PolygonHandle> &GetCluster(const int idx) const
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
    inline const int GetPolygonLabel(const Shell::PolygonHandle plHd) const
    {
        return boss.GetLabelForPolygon(plHd);
    }
    void MakeCluster(const Shell &shl);
};


#endif
