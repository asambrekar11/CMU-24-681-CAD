#ifndef LATTICE_IS_INCLUDED
#define LATTICE_IS_INCLUDED
/* { */

#include <ysclass.h>

template <class T>
class Lattice3d
{
protected:
	int nx,ny,nz;
	T *dat;
	YsVec3 min,max,dgn;

private:
	Lattice3d(const Lattice3d <T> &incoming);
	Lattice3d &operator=(const Lattice3d <T> &incoming);
public:
	Lattice3d();
	~Lattice3d();
	void CleanUp(void);

	int GetN(void) const;
	T &Elem(int i);
	const T &Elem(int i) const;

	T &Elem(int x,int y,int z);
	const T &Elem(int x,int y,int z) const;

	/*! nx,ny,nz=number of blocks, not nodes. */
	void Create(int nx,int ny,int nz);

	void SetDimension(const YsVec3 &min,const YsVec3 &max);
	YsVec3i GetBlockIndex(const YsVec3 &pos);
	bool IsInRange(YsVec3i idx) const;
};

template <class T>
Lattice3d<T>::Lattice3d()
{
	nx=0;
	ny=0;
	nz=0;
	dat=nullptr;
}

template <class T>
Lattice3d<T>::~Lattice3d()
{
	CleanUp();
}

template <class T>
void Lattice3d<T>::CleanUp(void)
{
	if(nullptr!=dat)
	{
		delete [] dat;
	}
	nx=0;
	ny=0;
	nz=0;
	dat=nullptr;
}

template <class T>
int Lattice3d<T>::GetN(void) const
{
	return (nx+1)*(ny+1)*(nz+1);
}
template <class T>
T &Lattice3d<T>::Elem(int i)
{
	return dat[i];
}
template <class T>
const T &Lattice3d<T>::Elem(int i) const
{
	return dat[i];
}

template <class T>
void Lattice3d<T>::Create(int nx,int ny,int nz)
{
	CleanUp();
	if(0<nx && 0<ny && 0<nz)
	{
		this->nx=nx;
		this->ny=ny;
		this->nz=nz;
		dat=new T [(nx+1)*(ny+1)*(nz+1)];
	}
}

template <class T>
T &Lattice3d<T>::Elem(int x,int y,int z)
{
	return dat[z*(nx+1)*(ny+1)+y*(nx+1)+x];
}

template <class T>
const T &Lattice3d<T>::Elem(int x,int y,int z) const
{
	return dat[z*(nx+1)*(ny+1)+y*(nx+1)+x];
}

template <class T>
void Lattice3d<T>::SetDimension(const YsVec3 &min,const YsVec3 &max)
{
	this->min=min;
	this->max=max;
	this->dgn=max-min;
}

template <class T>
YsVec3i Lattice3d<T>::GetBlockIndex(const YsVec3 &pos)
{
	const YsVec3 rel=pos-min;
	const double tx=rel.x()/dgn.x();
	const double ty=rel.y()/dgn.y();
	const double tz=rel.z()/dgn.z();
	int ix=(int)(tx*(double)nx);
	int iy=(int)(ty*(double)ny);
	int iz=(int)(tz*(double)nz);
	return YsVec3i(ix,iy,iz);
}

template <class T>
bool Lattice3d<T>::IsInRange(YsVec3i idx) const
{
	if(0<=idx.x() && idx.x()<=nx &&
	   0<=idx.y() && idx.y()<=ny &&
	   0<=idx.z() && idx.z()<=nz)
	{
		return true;
	}
	return false;
}

/* } */
#endif
