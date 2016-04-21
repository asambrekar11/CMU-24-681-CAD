#ifndef SHELL_IS_INCLUDED
#define SHELL_IS_INCLUDED

#include <stdio.h>
#include <vector>
#include "vector_nak.h"
#include "mihashtable.h"


////////////////////////////////////////////////////////////////////////////////
//MICOLOR CLASS : CLASS FOR COLORS
////////////////////////////////////////////////////////////////////////////////

class MIColor;

static const MIColor MIRed();
static const MIColor MIRed();
static const MIColor MIYellow();
static const MIColor MIGreen();
static const MIColor MICyan();
static const MIColor MIBlue();
static const MIColor MIMagenta();

class MIColor
{
protected:
	float rgb[3];
public:
	inline MIColor()
	{
		rgb[0] = 0.0;
		rgb[1] = 0.0;
		rgb[2] = 0.0;
	}
	inline MIColor(const float col[])
	{
		SetColor(col);
	}
	inline void SetColor(const float col[])
	{
		for (int i = 0; i<3; i++)
		{
			rgb[i] = col[i];
		}
	}
	inline void SetColor(const MIColor &col)
	{
		for (int i = 0; i<3; i++)
		{
			rgb[i] = col.rgb[i];
		}
	}
	inline float GetR() const
	{
		return rgb[0];
	}
	inline float GetG() const
	{
		return rgb[1];
	}
	inline float GetB() const
	{
		return rgb[2];
	}

	inline MIColor GetMIColor() const
	{
		return *this;
	}
	inline bool operator==(const MIColor &from) const
	{
		for (int i = 0; i<3; i++)
		{
			if (rgb[i] != from.rgb[i])
			{
				return false;
			}
		}
		return true;
	}
	inline bool operator!=(const MIColor &from) const
	{
		for (int i = 0; i<3; i++)
		{
			if (rgb[i] != from.rgb[i])
			{
				return true;
			}
		}
		return false;
	}
};

static const MIColor MIRed()
{
	float col[] = {1.0,0.0,0.0};
	return MIColor(col);
}

static const MIColor MIYellow()
{
	float col[] = {1.0,1.0,0.0};
	return MIColor(col);
}

static const MIColor MIGreen()
{
	float col[] = {0.0,1.0,0.0};
	return MIColor(col);
}

static const MIColor MICyan()
{
	float col[] = {0.0,1.0,1.0};
	return MIColor(col);
}

static const MIColor MIBlue()
{
	float col[] = {0.0,0.0,1.0};
	return MIColor(col);
}

static const MIColor MIMagenta()
{
	float col[] = {1.0,0.0,1.0};
	return MIColor(col);
}

static const MIColor MIWhite()
{
	float col[] = {1.0,1.0,1.0};
	return MIColor(col);
}

static const MIColor MIBlack()
{
	float col[] = {0.0,0.0,0.0};
	return MIColor(col);
}

////////////////////////////////////////////////////////////////////////////////
//END OF MICOLOR CLASS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//POLYGON CLASS
////////////////////////////////////////////////////////////////////////////////

class Polygon
{
protected:
	std::vector <const Vec3 *> vtx;
	Vec3 normal;
	MIColor color;
	void Cleanup();
public:
	~Polygon();
	void AddVertex(Vec3 &newVtx);
	void AddVertex(const Vec3 *newVtx);
	Polygon *Add(const int nVtx, const Vec3 newVtx[]);
	Polygon *Add(const std::vector<Vec3> &newVtx);
	Polygon *Add(const int nVtx, const Vec3 *newVtx[]);
	Polygon *Add(std::vector<const Vec3 *> newVtx);
	void SetNormal(Vec3 normal);
	void SetColor(const MIColor &col);
	const Vec3 GetNormal() const;
	const std::vector <const Vec3 *> GetVertex() const;
    inline const int GetNumVertex() const
    {
        return vtx.size();
    }
	const MIColor GetColor(const Polygon *plHd) const;
	double PolygonArea();
};

////////////////////////////////////////////////////////////////////////////////
//END OF POLYGON CLASS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//SHELL CLASS
////////////////////////////////////////////////////////////////////////////////

class Shell
{
public:
	typedef const Polygon * PolygonHandle;
	typedef const Vec3 * VertexHandle;

//  Polyton *editablePolygon=(Polygon *)plHd;
	
protected:
	std::vector <Vec3> vtx;
	std::vector <Polygon> plygn;
	
	////////////////////////////////////////////////////////////////////////////
	//EDGEKEY CLASS : KEY FOR EDGES
	////////////////////////////////////////////////////////////////////////////
public:
	class EdgeKey
	{
	public:
		long long int edVtKey[2];
		inline bool operator==(const EdgeKey &incoming) const
		{
			return (edVtKey[0]==incoming.edVtKey[0] && edVtKey[1]==incoming.edVtKey[1]) ||
				   (edVtKey[0]==incoming.edVtKey[1] && edVtKey[1]==incoming.edVtKey[0]);
		}
		inline bool operator!=(const EdgeKey &incoming) const
		{
			return (edVtKey[0]!=incoming.edVtKey[0] || edVtKey[1]!=incoming.edVtKey[1]) &&
				   (edVtKey[0]!=incoming.edVtKey[1] || edVtKey[1]!=incoming.edVtKey[0]);
		}
	};
	
	////////////////////////////////////////////////////////////////////////////
	//END OF EDGEKEY CLASS
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	//EDGEPOLYGONTABLECLASS : HASHTABLE FOR EDGE TO POLYGON ASSOCIATIVITY
	////////////////////////////////////////////////////////////////////////////
	
	class EdgePolygonTable : public HashTable<EdgeKey,std::vector<PolygonHandle>>
	{
	public:
		const Shell *shl;
		inline EdgePolygonTable()
		{
			shl = nullptr;
		}
		inline ~EdgePolygonTable()
		{
			shl = nullptr;
		}
		void Initialize(const Shell &shl);
		std::vector<PolygonHandle> FindPolygon(VertexHandle vtHd0, VertexHandle vtHd1) const;
	};
	
	////////////////////////////////////////////////////////////////////////////
	//END OF EDGEPOLYGONTABLE CLASS
	////////////////////////////////////////////////////////////////////////////
	
	EdgePolygonTable EdgeToPolygon;
	
	////////////////////////////////////////////////////////////////////////////
	//VERTEXPOLYGONTABLECLASS : HASHTABLE FOR VERTEX TO POLYGON ASSOCIATIVITY
	////////////////////////////////////////////////////////////////////////////
	
	class VertexPolygonTable : public HashTable<VertexHandle,std::vector<PolygonHandle>>
	{
	public:
		const Shell *shl;
		inline VertexPolygonTable()
		{
			shl = nullptr;
		}
		inline ~VertexPolygonTable()
		{
			shl = nullptr;
		}
		void Initialize(const Shell &shl);
		std::vector<PolygonHandle> FindPolygon(VertexHandle vtHd) const;
	};
	
	////////////////////////////////////////////////////////////////////////////
	//END OF EDGEPOLYGONTABLE CLASS
	////////////////////////////////////////////////////////////////////////////
	
	VertexPolygonTable VertexToPolygon;
	
public:
	
	////////////////////////////////////////////////////////////////////////////
	//POLYGONENUMERATOR CLASS : CLASS FOR RANGE-BASED FOR LOOP IN POLYGONS
	////////////////////////////////////////////////////////////////////////////
	
	class PolygonEnumerator
	{
	public:
		const Shell* shl;
		
		////////////////////////////////////////////////////////////////////////
		//POLYGON::ITERATOR CLASS
		////////////////////////////////////////////////////////////////////////
		
		class iterator
		{
		public:
			const Shell * shl;
			PolygonHandle prev,next,plHd;
			inline iterator &operator++()
			{
				prev = plHd;
				plHd = next;
				next = shl->FindNextPolygon(plHd);
				return *this;
			}
			inline iterator operator++(int)
			{
				iterator copy = *this;
				prev = plHd;
				plHd = next;
				next = shl->FindNextPolygon(plHd);
				return copy;
			}
			inline iterator &operator--()
			{
				next = plHd;
				plHd = prev;
				prev = shl->FindPrevPolygon(plHd);
				return *this;
			}
			inline iterator operator--(int)
			{
				iterator copy = *this;
				next = plHd;
				plHd = prev;
				prev = shl->FindPrevPolygon(plHd);
				return copy;
			}
			inline bool operator==(const iterator &from) const
			{
				return (shl == from.shl && plHd == from.plHd);
			}
			inline bool operator!=(const iterator &from) const
			{
				return (shl != from.shl || plHd != from.plHd);
			}
			inline PolygonHandle operator*()
			{
				return plHd;
			}
		};
		
		////////////////////////////////////////////////////////////////////////
		//END OF POLYGON::ITERATOR CLASS
		////////////////////////////////////////////////////////////////////////
		
		inline iterator begin()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.plHd = shl->FindNextPolygon(nullptr);
			iter.next = shl->FindNextPolygon(iter.plHd);
			return iter;
		}
		
		inline const iterator begin() const
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.plHd = shl->FindNextPolygon(nullptr);
			iter.next = shl->FindNextPolygon(iter.plHd);
			return iter;
		}
		inline iterator end()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.plHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
		inline const iterator end() const
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.plHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
		inline iterator rbegin()
		{
			iterator iter;
			iter.shl = shl;
			iter.next = nullptr;
			iter.plHd = shl->FindPrevPolygon(nullptr);
			iter.prev = shl->FindPrevPolygon(iter.plHd);
			return iter;
		}
		inline const iterator rbegin() const
		{
			iterator iter;
			iter.shl = shl;
			iter.next = nullptr;
			iter.plHd = shl->FindPrevPolygon(nullptr);
			iter.prev = shl->FindPrevPolygon(iter.plHd);
			return iter;
		}
		inline iterator rend()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.plHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
		inline const iterator rend() const
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.plHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
	};
	
	////////////////////////////////////////////////////////////////////////////
	//END OF POLYGONENUMERATOR CLASS
	////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////
	//VERTEXENUMERATOR CLASS : CLASS FOR RANGE-BASED FOR LOOP IN VERTICES
	////////////////////////////////////////////////////////////////////////////
	
	class VertexEnumerator
	{
	public:
		const Shell *shl;
		
		////////////////////////////////////////////////////////////////////////
		//VERTEX::ITERATOR CLASS
		////////////////////////////////////////////////////////////////////////
		
		class iterator
		{
		public:
			const Shell *shl;
			VertexHandle prev,next,vtHd;
			inline iterator &operator++()
			{
				prev = vtHd;
				vtHd = next;
				next = shl->FindNextVertex(vtHd);
				return *this;
			}
			inline iterator operator++(int)
			{
				iterator copy = *this;
				prev = vtHd;
				vtHd = next;
				next = shl->FindNextVertex(vtHd);
				return copy;
			}
			inline iterator &operator--()
			{
				next = vtHd;
				vtHd = prev;
				prev = shl->FindPrevVertex(vtHd);
				return *this;
			}
			inline iterator operator--(int)
			{
				iterator copy = *this;
				next = vtHd;
				vtHd = prev;
				prev = shl->FindPrevVertex(vtHd);
				return copy;
			}
			inline bool operator==(const iterator &from) const
			{
				return (shl == from.shl && vtHd == from.vtHd);
			}
			inline bool operator!=(const iterator &from) const
			{
				return (shl != from.shl || vtHd != from.vtHd);
			}
			inline VertexHandle operator*()
			{
				return vtHd;
			}
		};
		
		////////////////////////////////////////////////////////////////////////
		//END OF VERTEX::ITERATOR CLASS
		////////////////////////////////////////////////////////////////////////
		
		inline iterator begin()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = shl->FindNextVertex(nullptr);
			iter.next = shl->FindNextVertex(iter.vtHd);
			return iter;
		}
		inline iterator end()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
		inline iterator rbegin()
		{
			iterator iter;
			iter.shl = shl;
			iter.next = nullptr;
			iter.vtHd = shl->FindPrevVertex(nullptr);
			iter.prev = shl->FindPrevVertex(iter.vtHd);
			return iter;
		}
		inline iterator rend()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
		inline const iterator begin() const
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = shl->FindNextVertex(nullptr);
			iter.next = shl->FindNextVertex(iter.vtHd);
			return iter;
		}
		inline const iterator end() const
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
		inline const iterator rbegin() const
		{
			iterator iter;
			iter.shl = shl;
			iter.next = nullptr;
			iter.vtHd = shl->FindPrevVertex(nullptr);
			iter.prev = shl->FindPrevVertex(iter.vtHd);
			return iter;
		}
		inline const iterator rend() const
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = nullptr;
			iter.next = nullptr;
			return iter;
		}
	};
	
	////////////////////////////////////////////////////////////////////////////
	//END OF VERTEXENUMERATOR CLASS
	////////////////////////////////////////////////////////////////////////////
	
	inline const PolygonEnumerator AllPolygon() const
	{
		PolygonEnumerator AllplHd;
		AllplHd.shl = this;
		return AllplHd;
	}
	
	inline const VertexEnumerator AllVertex() const
	{
		VertexEnumerator AllvtHd;
		AllvtHd.shl = this;
		return AllvtHd;
	}

	inline long long int GetNumVertex() const
	{
		return vtx.size();
	}

	inline long long int GetNumPolygon() const
	{
		return plygn.size();
	}
	
	long long int GetSearchKey(VertexHandle vtHd) const;
	long long int GetSearchKey(PolygonHandle plHd) const;
	
	Vec3 GetNormal(PolygonHandle plHd) const;
	Vec3 GetPolygonArea(PolygonHandle plHd) const;
	Vec3 GetBaryCenter(PolygonHandle plHd) const;
	
	PolygonHandle FindNextPolygon(PolygonHandle plHd) const;
	PolygonHandle FindPrevPolygon(PolygonHandle plHd) const;
	
	VertexHandle FindNextVertex(VertexHandle vtHd) const;
	VertexHandle FindPrevVertex(VertexHandle vtHd) const;
	
	VertexHandle AddVertex(const Vec3 &incoming);
	
	PolygonHandle AddPolygon(const int nVtx, VertexHandle incoming[]);
	PolygonHandle AddPolygon(std::vector<VertexHandle> incoming);
	PolygonHandle AddTriangle(VertexHandle incoming[]);
	
	std::vector <VertexHandle> GetPolygonVertex(PolygonHandle plHd) const;
	inline std::vector <PolygonHandle> GetVertexPolygon(VertexHandle vtHd) const
	{
		return VertexToPolygon.FindPolygon(vtHd);
	}
	const Vec3 GetVertexPosition(VertexHandle vtHd) const;
	
	void SetPolygonColor(PolygonHandle plHd, const MIColor col);
	void SetPolygonColor(PolygonHandle plHd, const float col[]);
	
	void EnableSearch();
	PolygonHandle GetNeighbour(PolygonHandle plHd, VertexHandle vtHd) const;
	PolygonHandle PickRandomPolygon() const;
};

typedef HashSet<Shell::PolygonHandle> PolygonStore;

#endif