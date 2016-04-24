#ifndef VERTEX_TO_POLYGON_IS_INCLUDED
#define VERTEX_TO_POLYGON_IS_INCLUDED

#include <vector>
#include <ysshellext.h>
#include "mihashtable.h"

template <>
inline long long int HashTable <YSHASHKEY,std::vector <YsShell::PolygonHandle> >::HashCode(const YSHASHKEY &key) const
{
	return key;
}

class VertexToPolygonTable : public HashTable <YSHASHKEY,std::vector <YsShell::PolygonHandle> >
{
public:
	void SetShell(const YsShellExt &shl);
	const std::vector <YsShell::PolygonHandle> *FindPolygonFromVertex(
	    const YsShellExt &shl,
	    YsShell::VertexHandle vtHd) const;
};

#endif
