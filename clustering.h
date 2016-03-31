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
    
class ProxyTable : public HashTable<Proxy,std::vector<Shell::PolygonHandle>>
{
public:
    inline std::vector<Shell::PolygonHandle> &GetPolygonForProxy(Proxy pxy)
    {
        return *((*this)[pxy]);
    }
};
    

template <int k>
class LloydCluster
{
protected:
    Proxy pxy[k];
    ProxyTable stud;
public:
    inline LloydCluster()
    {
        shl = nullptr;
    }
    inline ~LloydCLuster()
    {
        shl = nullptr;
    }
    inline std::vector<Shell::PolygonHandle> &GetCluster(const int idx) const
    {
        return stud.GetPolygonForProxy(pxy[idx]);
    }
    inline Proxy &GetProxy(const int idx) const
    {
        return pxy[idx];
    }
    void MakeCluster(const Shell &shell);
};

#endif
