#include "shell.h"

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

void Polygon::Add(const Vec3 &newVtx)
{
	vtx.push_back(&newVtx);
}

void Polygon::Add(const int nVtx, const Vec3 newVtx[])
{
	for (int i = 0; i<nVtx; i++)
	{
		vtx.push_back(&newVtx[i]);
	}
}

void Polygon::Add(const std::vector<Vec3> &newVtx)
{
	for (auto &vt : newVtx)
	{
		vtx.push_back(&vt);
	}
}

void Polygon::Add(const Vec3 *newVtx)
{
	vtx.push_back(newVtx);
}

void Polygon::Add(const int nVtx, const Vec3 *newVtx[])
{
	for (int i = 0; i<nVtx; i++)
	{
		vtx.push_back(newVtx[i]);
	}
}
void Polygon::Add(const std::vector<Vec3 *> newVtx)
{
	for (auto &vt : newVtx)
	{
		vtx.push_back(vt);
	}
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

const std::vector<Vec3 *> Polygon::GetVertex() const
{
	return vtx;
}

////////////////////////////////////////////////////////////////////////////////
//SHELL::EDGEPOLYGONTABLE CLASS
////////////////////////////////////////////////////////////////////////////////

void Shell::EdgePolygonTable::Initialize(const Shell &shell)
{
	Cleanup();
	shl = &shell;
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
			if (nullptr == curr_poly)
			{
				curr_poly->push_back(plHd)
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

const std::vector<Shell::PolygonHandle> Shell::EdgePolygonTable::FindPolygon(Shell::VertexHandle vtHd0, Shell::VertexHandle vtHd1) const
{
	Shell::EdgeKey key;
	key.edVtKey[0] = vtHd0;
	key.edVtKey[1] = vtHd1;
	return (*this)[key];
}

////////////////////////////////////////////////////////////////////////////////
//SHELL CLASS
////////////////////////////////////////////////////////////////////////////////

long long int Shell::GetSearchKey(VertexHandle vtHd) const
{
	for (long long int i = 0; i<vtx.size(); i++)
	{
		if (&vtx[i] == vtHd)
		{
			return i;
		}
	}
	return -1;
}

long long int Shell::GetSearchKey(PolygonHandle plHd) const
{
	for (long long int i = 0; i<plygn.size(); i++)
	{
		if (&plygn[i] == plHd)
		{
			return i;
		}
	}
	return -1;
}

Vec3 Shell::GetNormal(Shell::PolygonHandle plHd)
{
	return plHd->GetNormal();
}
	
Shell::PolygonHandle Shell::FindNextPolygon(Shell::PolygonHandle plHd)
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

Shell::PolygonHandle Shell::FindPrevPolygon(Shell::PolygonHandle plHd)
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

Shell::VertexHandle Shell::FindNextVertex(Shell::VertexHandle vtHd)
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

Shell::VertexHandle Shell::FindPrevVertex(Shell::VertexHandle vtHd)
{
	if (vtHd == nullptr)
	{
		return &vtx.back();
	}
	for (int i = 0; i<vtx.size(); i++)
	{
		if (&vtx[i] == vtHd)
		{
			return &vtx[i-1];
		}
	}
	return nullptr;
}

const Shell::VertexHandle Shell::AddVertex(const Vec3 &incoming)
{
	vtx.push_back(incoming);
	return &vtx.back();
}

const Shell::PolygonHandle AddPolygon(const int nVtx, const Shell::VertexHandle incoming[])
{
	Polygon poly;
	poly.Add(nVtx,incoming);
	plygn.push_back(poly);
	return &plygn.back();
}

const Shell::PolygonHandle AddPolygon(const std::vector<Shell::VertexHandle> incoming)
{
	Polygon poly;
	poly.Add(incoming);
	plygn.push_back(poly);
	return &plygn.back();
}

const Shell::PolygonHandle AddTriangle(const Shell::VertexHandle incoming[])
{
	return AddPolygon(3,incoming);
}

const std::vector <Shell::VertexHandle> Shell::GetPolygonVertex(Shell::PolygonHandle plHd) const
{
	return plHd->GetVertex();
}

const Vec3 Shell::GetVertexPosition(Shell::VertexHandle vtHd) const
{
	return vtHd->GetVecData();
}

void SetPolygonColor(Shell::PolygonHandle plHd, const MIColor col)
{
	plHd->SetColor(col);
}

void SetPolygonColor(Shell::PolygonHandle plHd, const float col[])
{
	plHd->SetColor(MIColor(col));
}

void Shell::EnableSearch()
{
	EdgeToPolygon.Initialize(*this);
}

const Shell::PolygonHandle GetNeighbour(Shell::PolygonHandle plHd, Shell::VertexHandle vtHd) const
{
	std::vector <Shell::VertexHandle> plvtx = GetPolygonVertex(plHd);
	int idx;
	for (int i = 0; i<plvtx.size(); i++)
	{
		if (plvtx[i] == vtHd)
		{
			idx = i;
			break;
		}
	}
	int nextidx = i==(plvtx.size()-1)?0:idx+1;
	auto plygrp = EdgeToPolygon.FindPolygon(plvtx[idx],plvtx[nextidx]);
	for (auto pl : plygrp)
	{
		if (pl != plHd)
		{
			return pl;
		}
	}
	return nullptr;
}