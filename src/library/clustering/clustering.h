#ifndef CLUSTERING_IS_INCLUDED
#define CLUSTERING_IS_INCLUDED

#include <vector>
#include "vector_nak.h"
#include "mihashtable.h"
#include "shell.h"

struct Proxy
{
public:
    Vec3 ProxyPosition, ProxyNormal;
    int label;
};

class ClusterNode
{
protected:
    Shell::PolygonHandle plHd;
    int label;
    double error;
public:
    inline bool operator>(const ClusterNode &rhs)
    {
        return this->error>rhs.error;
    }
    inline bool operator>=(const ClusterNode &rhs)
    {
        return this->error>=rhs.error;
    }
    inline bool operator<(const ClusterNode &rhs)
    {
        return this->error<rhs.error;
    }
    inline bool operator<=(const ClusterNode &rhs)
    {
        return this->error<=rhs.error;
    }
    inline bool operator==(const ClusterNode &rhs)
    {
        return this->error==rhs.error;
    }
    inline bool operator!=(const ClusterNode &rhs)
    {
        return this->error!=rhs.error;
    }
    inline ClusterNode()
    {
        plHd = nullptr;
    }
    inline ClusterNode(Shell::PolygonHandle pl, Proxy pxy)
    {
        SetPolygon(pl);
        SetProxy(pxy);
    }
    inline ~ClusterNode()
    {
        plHd = nullptr;
    }
    inline void SetPolygon(Shell::PolygonHandle pl)
    {
        plHd = pl;
    }
    void SetProxy(Proxy pxy);
    inline void SetLabel(int newLabel)
    {
        label = newLabel;
    }
    inline int GetLabel()
    {
        return label;
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
    

template <int k>
class LloydCluster
{
protected:
    Proxy pxy[k];
    class ProxyTable : public HashTable<Proxy,std::vector<Shell::PolygonHandle>>
    {
    public:
        inline const std::vector<Shell::PolygonHandle> &GetPolygonForProxy(const Proxy &pxy) const
        {
            return *((*this)[pxy]);
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
        inline const int GetLabelForPolygon(const Shell::PolygonHandle plHd) const
        {
            return *((*this)[shl->GetSearchKey(plHd)]);
        }
    };
    PolygonTable boss;
public:
    inline const std::vector<Shell::PolygonHandle> &GetCluster(const int idx) const
    {
        return stud.GetPolygonForProxy(pxy[idx]);
    }
    inline const Proxy &GetProxy(const int idx) const
    {
        return pxy[idx];
    }
    inline const int GetPolygonLabel(const Shell::PolygonHandle plHd) const
    {
        return boss.GetLabelForPolygon(plHd);
    }
    void MakeCluster(const Shell &shell);
};

#endif
