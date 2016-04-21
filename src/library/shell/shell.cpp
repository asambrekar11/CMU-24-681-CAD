#include "shell.h"



template<>
long long int HashTable<Shell::EdgeKey,std::vector<Shell::PolygonHandle>>::HashCode(const Shell::EdgeKey &key) const
{
	return key.edVtKey[0]+key.edVtKey[1];
}

template<>
long long int HashTable<Shell::VertexHandle,std::vector<Shell::PolygonHandle>>::HashCode(const Shell::VertexHandle &key) const
{
	return (long long int)key;
}


////////////////////////////////////////////////////////////////////////////////
//POLYGON CLASS
////////////////////////////////////////////////////////////////////////////////

Polygon::~Polygon()
{
	for (auto vtHd : vtx)
	{
		vtHd = nullptr;
	}
}

void Polygon::AddVertex(Vec3 &newVtx)
{
	vtx.push_back(&newVtx);
}

void Polygon::AddVertex(const Vec3 *newVtx)
{
	vtx.push_back(newVtx);
}


Polygon *Polygon::Add(const int nVtx, const Vec3 newVtx[])
{
	for (int i = 0; i<nVtx; i++)
	{
		vtx.push_back(&newVtx[i]);
	}
	return this;
}

Polygon *Polygon::Add(const std::vector<Vec3> &newVtx)
{
	for (auto &vt : newVtx)
	{
		vtx.push_back(&vt);
	}
	return this;
}

Polygon *Polygon::Add(const int nVtx, const Vec3 *newVtx[])
{
	for (int i = 0; i<nVtx; i++)
	{
		vtx.push_back(newVtx[i]);
	}
	return this;
}
Polygon *Polygon::Add(std::vector<const Vec3 *> newVtx)
{
	for (auto &vt : newVtx)
	{
		vtx.push_back(vt);
	}
	return this;
}

void Polygon::SetNormal(Vec3 normal)
{
	this->normal = normal;
}

void Polygon::SetColor(const MIColor &col)
{
	color = col;
}

const Vec3 Polygon::GetNormal() const
{
	return normal;
}

const std::vector <const Vec3 *> Polygon::GetVertex() const
{
	return vtx;
}

const MIColor Polygon::GetColor(const Polygon * plHd) const
{
	return plHd->color;
}

Vec3 Polygon::PolygonArea() const
{
    auto BC = GetBaryCenter();
    Vec3 Area(0.,0.,0.);
    for (int i = 0; i<vtx.size(); i++)
    {
        auto v1 = *vtx[i];
        auto v2 = i==vtx.size()-1?*vtx[0]:*vtx[i+1];
        Area = Area + cross(v1-BC,v2-BC);
    }
    return Area;
}

Vec3 Polygon::GetBaryCenter() const
{
    Vec3 BC(0.,0.,0.);
    for (auto vtHd : vtx)
    {
        BC = BC + *vtHd;
    }
    BC = BC / vtx.size();
    return BC;
}

////////////////////////////////////////////////////////////////////////////////
//SHELL::EDGEPOLYGONTABLE CLASS
////////////////////////////////////////////////////////////////////////////////

void Shell::EdgePolygonTable::Initialize(const Shell &shl)
{
	Cleanup();
	this->shl = &shl;
	Resize(shl.GetNumPolygon());
	
	for (auto plHd : shl.AllPolygon())
	{
		auto plvtx = shl.GetPolygonVertex(plHd);
		for (int edIdx = 0; edIdx < plvtx.size(); edIdx++)
		{
			Shell::EdgeKey key;
			key.edVtKey[0] = shl.GetSearchKey(plvtx[edIdx]);
			key.edVtKey[1] = shl.GetSearchKey(plvtx[edIdx==(plvtx.size()-1)?0:edIdx+1]);
			auto curr_poly = (*this)[key];
			if (nullptr != curr_poly)
			{
				curr_poly->push_back(plHd);
			}
			else
			{
				std::vector<Shell::PolygonHandle> poly;
				poly.push_back(plHd);
				this->Update(key,poly);
			}
		}
	}
	
}

std::vector<Shell::PolygonHandle> Shell::EdgePolygonTable::FindPolygon(Shell::VertexHandle vtHd0, Shell::VertexHandle vtHd1) const
{
	Shell::EdgeKey key;
	key.edVtKey[0] = shl->GetSearchKey(vtHd0);
	key.edVtKey[1] = shl->GetSearchKey(vtHd1);
	return *(*this)[key];
}

////////////////////////////////////////////////////////////////////////////////
//SHELL::VERTEXPOLYGONTABLE CLASS
////////////////////////////////////////////////////////////////////////////////

void Shell::VertexPolygonTable::Initialize(const Shell &shl)
{
	Cleanup();
	this->shl=&shl;
	Resize(shl.GetNumPolygon());
	
	for (auto plHd : shl.AllPolygon())
	{
		for (auto vtHd : shl.GetPolygonVertex(plHd))
		{
			auto plVtptr=(*this)[vtHd];
			if (plVtptr != nullptr)
			{
				plVtptr->push_back(plHd);
			}
			else
			{
				std::vector <Shell::PolygonHandle> vtPl;
				vtPl.push_back(plHd);
				this->Update(vtHd,vtPl);
			}
		}
	}
}

std::vector <Shell::PolygonHandle> Shell::VertexPolygonTable::FindPolygon(Shell::VertexHandle vtHd) const
{
	return *(*this)[vtHd];
}

////////////////////////////////////////////////////////////////////////////////
//SHELL CLASS
////////////////////////////////////////////////////////////////////////////////

long long int Shell::GetSearchKey(VertexHandle vtHd) const
{
	return (long long int)vtHd;
}

long long int Shell::GetSearchKey(PolygonHandle plHd) const
{
	return (long long int)plHd;
}

Vec3 Shell::GetNormal(Shell::PolygonHandle plHd) const
{
	return plHd->GetNormal();
}

Vec3 Shell::GetPolygonArea(Shell::PolygonHandle plHd) const
{
	return plHd->PolygonArea();
}

Vec3 Shell::GetBaryCenter(Shell::PolygonHandle plHd) const
{
    return plHd->GetBaryCenter();
}
	
Shell::PolygonHandle Shell::FindNextPolygon(Shell::PolygonHandle plHd) const
{
	for (int i = 0; i<plygn.size(); i++)
	{
		if (&plygn[i] == plHd)
		{
			return &plygn[i+1];
		}
	}
	return nullptr;
}

Shell::PolygonHandle Shell::FindPrevPolygon(Shell::PolygonHandle plHd) const
{
	for (int i = 0; i<plygn.size(); i++)
	{
		if (&plygn[i] == plHd)
		{
			return &plygn[i-1];
		}
	}
	return nullptr;
}

Shell::VertexHandle Shell::FindNextVertex(Shell::VertexHandle vtHd) const
{
	if (vtHd == nullptr)
	{
		return &vtx[0];
	}
	for (int i = 0; i<vtx.size(); i++)
	{
		if (&vtx[i] == vtHd)
		{
			return &vtx[i+1];
		}
	}
	return nullptr;
}

Shell::VertexHandle Shell::FindPrevVertex(Shell::VertexHandle vtHd) const
{
	if (vtHd == nullptr)
	{
		Shell::VertexHandle last = &vtx.back();
		return last;
	}
	for (int i = 0; i<vtx.size(); i++)
	{
		if (&vtx[i] == vtHd)
		{
			return &vtx[i+1];
		}
	}
	return nullptr;
}

Shell::VertexHandle Shell::AddVertex(const Vec3 &incoming)
{
	vtx.push_back(incoming);
	return &vtx.back();
}

Shell::PolygonHandle Shell::AddPolygon(const int nVtx, Shell::VertexHandle incoming[])
{
	Polygon poly;
	poly.Add(nVtx,incoming);
	plygn.push_back(poly);
	return &plygn.back();
}

Shell::PolygonHandle Shell::AddPolygon(std::vector<Shell::VertexHandle> incoming)
{
	Polygon poly;
	poly.Add(incoming);
	plygn.push_back(poly);
	return &plygn.back();
}

Shell::PolygonHandle Shell::AddTriangle(Shell::VertexHandle incoming[])
{
	return AddPolygon(3,incoming);
}

std::vector <Shell::VertexHandle> Shell::GetPolygonVertex(Shell::PolygonHandle plHd) const
{
	return plHd->GetVertex();
}

const Vec3 Shell::GetVertexPosition(Shell::VertexHandle vtHd) const
{
	return *vtHd;
}

void Shell::SetPolygonColor(Shell::PolygonHandle plHd, const MIColor col)
{
	auto edit_plHd = (Polygon *)plHd;
	edit_plHd->SetColor(col);
}

void Shell::SetPolygonColor(Shell::PolygonHandle plHd, const float col[])
{
	auto edit_plHd = (Polygon *)plHd;
	edit_plHd->SetColor(MIColor(col));
}

void Shell::EnableSearch()
{
	EdgeToPolygon.Initialize(*this);
	VertexToPolygon.Initialize(*this);
}

Shell::PolygonHandle Shell::GetNeighbour(Shell::PolygonHandle plHd, Shell::VertexHandle vtHd) const
{
	auto plvtx = GetPolygonVertex(plHd);
	int idx;
	for (int i = 0; i<plvtx.size(); i++)
	{
		if (plvtx[i] == vtHd)
		{
			idx = i;
			break;
		}
	}
	int nextidx = idx==(plvtx.size()-1)?0:idx+1;
	auto plygrp = Shell::EdgeToPolygon.FindPolygon(plvtx[idx],plvtx[nextidx]);
	for (auto pl : plygrp)
	{
		if (pl != plHd)
		{
			return pl;
		}
	}
	return nullptr;
}

Shell::PolygonHandle Shell::PickRandomPolygon()
{
	return &plygn[rand()%plygn.size()];
}