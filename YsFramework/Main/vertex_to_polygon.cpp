#include "vertex_to_polygon.h"


void VertexToPolygonTable::SetShell(const YsShellExt &shl)
{
	Cleanup();
	Resize(shl.GetNumPolygon());

	for(auto plHd : shl.AllPolygon())
	{
		for(auto vtHd : shl.GetPolygonVertex(plHd))
		{
			auto hashKey=shl.GetSearchKey(vtHd);
			auto vtPlPtr=(*this)[hashKey];
			if(nullptr!=vtPlPtr)
			{
				vtPlPtr->push_back(plHd);
			}
			else
			{
				std::vector <YsShell::PolygonHandle> vtPl;
				vtPl.push_back(plHd);
				this->Update(hashKey,vtPl);
			}
		}
	}
}

const std::vector <YsShell::PolygonHandle> *VertexToPolygonTable::FindPolygonFromVertex(
    const YsShellExt &shl,
    YsShell::VertexHandle vtHd) const
{
	auto hashKey=shl.GetSearchKey(vtHd);
	return (*this)[hashKey];
}
