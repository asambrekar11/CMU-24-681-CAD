#ifndef SHELL_IS_INCLUDED
#define SHELL_IS_INCLUDED

#include <stdio.h>
#include <vector>
#include "vector_nak.h"
#include "mihashtable.h"

////////////////////////////////////////////////////////////////////////////////
//MICOLOR CLASS : CLASS FOR COLORS
////////////////////////////////////////////////////////////////////////////////

class MIColor
{
protected:
	float rgb[3];
public:
	inline MIColor()
	{
		SetColor({0.0,0.0,0.0});
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
	inline float *GetColor() const
	{
		return rgb;
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
	return MIColor({1.0,0.0,0.0});
}

static const MIColor MIYellow()
{
	return MIColor({1.0,1.0,0.0});
}

static const MIColor MIGreen()
{
	return MIColor({0.0,1.0,0.0});
}

static const MIColor MICyan()
{
	return MIColor({0.0,1.0,1.0});
}

static const MIColor MIBlue()
{
	return MIColor({0.0,0.0,1.0});
}

static const MIColor MIMagenta()
{
	return MIColor({1.0,0.0,1.0});
}

static const MIColor MIWhite()
{
	return MIColor({1.0,1.0,1.0});
}

static const MIColor MIBlack()
{
	return MIColor({0.0,0.0,0.0});
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
	std::vector<Vec3 *> vtx;
	Vec3 normal;
	MIColor color;
	void Cleanup();
public:
	~Polygon();
	void Add(const Vec3 &newVtx);
	void Add(const int nVtx, const Vec3 newVtx[]);
	void Add(const std::vector<Vec3> &newVtx);
	void Add(const Vec3 *newVtx);
	void Add(const int nVtx, const Vec3 *newVtx[]);
	void Add(const std::vector<Vec3 *> newVtx);
	void SetNormal(Vec3 normal);
	void SetColor(const MIColor col);
	const Vec3 GetNormal() const;
	std::vector<Vec3 *> GetVertex() const;
};

////////////////////////////////////////////////////////////////////////////////
//END OF POLYGON CLASS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//SHELL CLASS
////////////////////////////////////////////////////////////////////////////////

class Shell
{
protected:
	std::vector <Vec3> vtx;
	std::vector <Polygon> plygn;
	
	////////////////////////////////////////////////////////////////////////////
	//EDGEKEY CLASS : KEY FOR EDGES
	////////////////////////////////////////////////////////////////////////////
	
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
	}
	
	////////////////////////////////////////////////////////////////////////////
	//END OF EDGEKEY CLASS
	////////////////////////////////////////////////////////////////////////////

	template<>
	inline long long int HashTable<EdgeKey,std::vector<PolygonHandle>::HashCode(const EdgeKey &key) const
	{
		return key.edVtKey[0]+key.edVtKey[1];
	}

	////////////////////////////////////////////////////////////////////////////
	//EDGEPOLYGONTABLECLASS : HASHTABLE FOR EDGE TO POLYGON ASSOCIATIVITY
	////////////////////////////////////////////////////////////////////////////
	
	class EdgePolygonTable : public HashTable<EdgeKey,std::vector<PolygonHandle>
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
		const std::vector<PolygonHandle> FindPolygon(VertexHandle vtHd0, VertexHandle vtHd1) const;
	};
	
	////////////////////////////////////////////////////////////////////////////
	//END OF EDGEPOLYGONTABLE CLASS
	////////////////////////////////////////////////////////////////////////////
	
	EdgePolygonTable EdgeToPolygon;
	
public:
	typedef Polygon* PolygonHandle;
	typedef Vec3* VertexHandle;
	
	////////////////////////////////////////////////////////////////////////////
	//POLYGONENUMERATOR CLASS : CLASS FOR RANGE-BASED FOR LOOP IN POLYGONS
	////////////////////////////////////////////////////////////////////////////
	
	class PolygonEnumerator
	{
	public:
		const Shell *shl;
		
		////////////////////////////////////////////////////////////////////////
		//POLYGON::ITERATOR CLASS
		////////////////////////////////////////////////////////////////////////
		
		class iterator
		{
		public:
			const Shell *shl;
			PolygonHandle prev,next,plHd;
			inline iterator &operator++()
			{
				prev = plHd;
				plHd = next;
				next = shl->FindNextPolygon(plHd);
				return *this;
			}
			inline iterator &operator--()
			{
				next = plHd;
				plHd = prev;
				prev = shl->FindPrevPolygon(plHd);
				return *this;
			}
			inline bool operator==(const iterator &from) const
			{
				return (shl == from.shl && plHd == from.plHd);
			}
			inline bool operator!=(const iterator &from) const
			{
				return (shl != from.shl || plHd != from.plHd);
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
		inline iterator end()
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
		inline iterator rend()
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
				plHd = next;
				next = shl->FindNextVertex(vtHd);
				return *this;
			}
			inline iterator operator++(int)
			{
				iterator copy = *this;
				prev = vtHd;
				plHd = next;
				next = shl->FindNextVertex(vtHd);
				return copy;
			}
			inline iterator &operator--()
			{
				next = vtHd;
				plHd = prev;
				prev = shl->FindPrevVertex(vtHd);
				return *this;
			}
			inline iterator operator--(int)
			{
				iterator copy = *this;
				next = vtHd;
				plHd = prev;
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
		};
		
		////////////////////////////////////////////////////////////////////////
		//END OF VERTEX::ITERATOR CLASS
		////////////////////////////////////////////////////////////////////////
		
		inline iterator begin()
		{
			iterator iter;
			iter.shl = shl;
			iter.prev = nullptr;
			iter.vtHd = shl->FindNextPolygon(nullptr);
			iter.next = shl->FindNextPolygon(iter.vtHd);
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
			iter.vtHd = shl->FindPrevPolygon(nullptr);
			iter.prev = shl->FindPrevPolygon(iter.vtHd);
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

	inline long long int GetNumVertex()
	{
		return vtx.size();
	}

	inline long long int GetNumPolygon()
	{
		return plygn.size();
	}
	
	long long int GetSearchKey(VertexHandle vtHd) const;
	long long int GetSearchKey(PolygonHandle plHd) const;
	
	Vec3 GetNormal(PolygonHandle plHd);
	
	PolygonHandle FindNextPolygon(PolygonHandle plHd);
	PolygonHandle FindPrevPolygon(PolygonHandle plHd);
	
	VertexHandle FindNextVertex(VertexHandle vtHd);
	VertexHandle FindPrevVertex(VertexHandle vtHd);
	
	const VertexHandle AddVertex(const Vec3 &incoming);
	
	const PolygonHandle AddPolygon(const int nVtx, VertexHandle incoming[]);
	const PolygonHandle AddPolygon(std::vector<VertexHandle> incoming);
	const PolygonHandle AddTriangle(VertexHandle incoming[]);
	
	const std::vector <VertexHandle> GetPolygonVertex(PolygonHandle plHd) const;
	const Vec3 GetVertexPosition(VertexHandle vtHd) const;
	
	void SetPolygonColor(PolygonHandle plHd, const MIColor col);
	void SetPolygonColor(PolygonHandle plHd, const float col[]);
	
	void EnableSearch();
	const PolygonHandle GetNeighbour(PolygonHandle plHd, VertexHandle vtHd) const;
};

#endif