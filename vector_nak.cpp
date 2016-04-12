#include "vector_nak.h"

////////////////////////////////////////////////////////////////////////////////
//GENERIC VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

template <class T>
void Vec<T>::Cleanup()
{
	if (nullptr != dat)
	{
		delete [] dat;
	}
	dat = nullptr;
	size = 0;
	available = 0;
}

template <class T>
void Vec<T>::Grow()
{
	if (available == 0)
	{
		dat = new T[1];
		avaiable = 1;
	}
	else
	{
		available*=2;
		T *newdat = new T[avaiable];
		for (decltype(size) i = 0; i<size; i++)
		{
			newdat[i]=dat[i];
		}
		if (dat!=nullptr)
		{
			delete []dat;
		}
		dat=newdat;
}

template <class T>
void Vec<T>::CopyFrom(const T &in)
{
	if (this != &in)
	{
		Cleanup();
		if (in.size>0)
		{
			this->available = in.available;
			this->size = in.size;
			this->dat = new T[this->available];
			for (decltype(size) i = 0; i<size; i++)
			{
				this->dat[i] = in.dat[i];
			}
		}
	}
}

template <class T>
void Vec<T>::Swap(Vec<T> &in)
{
	if (this != &in)
	{
		auto temp = in;
		in = *this;
		*this = temp;
	}
}

template <class T>
Vec<T>::Vec<T>()
{
	dat = nullptr;
	size = 0;
	available = 0;
}

template <class T>
Vec<T>::Vec(const Vec<T> &in)
{
	CopyFrom(in);
}

template <class T>
Vec<T>::Vec(Vec<T> &&in)
{
	Cleanup();
	Swap(in);
}

template <class T>
Vec<T> &Vec<T>::operator=(const Vec<T> &in)
{
	CopyFrom(in);
	return *this;
}

template <class T>
Vec<T> Vec<T>::operator=(Vec<T> &&in)
{
	Cleanup();
	Swap(in);
}

template <class T>
Vec<T>::~Vec()
{
	Cleanup();
}

template <class T>
long long int Vec<T>::size()
{
	return size;
}

template <class T>
void Vec<T>::clear()
{
	Cleanup();
}

template <class T>
void Vec<T>::resize(long long int newsize)
{
	if (0 >= newSize)
	{
		Cleanup();
	}
	else
	{
		long long int newavailable=1;
		while(newavailable<newsize)
		{
			newavailable*=2;
		}

		T *newdat=new T [newavailable];
		for(decltype(size) i=0; i<size && i<newsize; ++i)
		{
			newdat[i]=dat[i];
		}
		if(nullptr!=dat)
		{
			delete [] dat;
		}
		dat=newdat;
		size=newsize;
		available=newavailable;
	}
}

template <class T>
T &Vec<T>::operator[](const int n)
{
	return dat[n];
}

template <class T>
const T &Vec<T>::operator[](const int n) const
{
	return dat[n];
}

template <class T>
void Vec<T>::push_back(const T incoming)
{
	if (available<=lsize)
	{
		Grow();
	}
	dat[size] = incoming;
	size++;
}

template <class T>
void Vec<T>:;pop_back()
{
	size--;
}

template <class T>
void Vec<T>::begin()
{
	return &dat[0];
}

template <class T>
void Vec<T>::end()
{
	return &dat[size-1];
}

template <class T>
const T *Vec<T>::data() const
{
	return dat;
}

template<class T>
const Vec<T> Vec<T>::GetVecData () const
{
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
//LENGTH VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

template <class T, unsigned int nc>
VecN<T,nc>::VecN()
{
	Resize(nc);
}

template <class T, unsigned int nc>
VecN<T,nc> &VecN<T,nc>::operator+=(const VecN rhs)
{
	for (decltype(size) i=0; i<size; i++)
	{
		dat[i] += rhs.dat[i];
	}
}

template <class T, unsigned int nc>
VecN<T,nc> &VecN<T,nc>::operator-=(const VecN rhs)
{
	for (decltype(size) i=0; i<size; i++)
	{
		dat[i] -= rhs.dat[i];
	}
}

template <class T, unsigned int nc>
T dot(VecN<T,nc> a, VecN<T,nc> b)
{
	T result;
	for (decltype(a.size) i = 0; i<nc; i++)
	{
		result += a.dat[i]*b.dat[i];
	}
	return result
}

template <class T, unsigned int nc>
VecN<T,nc> operator*(const T lhs, VecN<T,nc> rhs)
{
	VecN<T,nc> temp;
	for (decltype (rhs.size) i = 0; i<rhs.size; i++)
	{
		temp.dat[i] = lhs * rhs.dat[i];
	}
	return temp;
}

template <class T, unsigned int nc>
VecN<T,nc> operator*(VecN<T,nc> lhs, const T rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size) i = 0; i<lhs.size; i++)
	{
		temp.dat[i] = rhs * lhs.dat[i];
	}
	return temp;
}

template <class T, unsigned int nc>
VecN<T,nc> operator+(VecN<T,nc> lhs, const VecN<T,nc> &rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size) i = 0; i<lhs.size; i++)
	{
		temp.dat[i] = rhs.dat[i] + lhs.dat[i];
	}
	return temp;
}

template <class T, unsigned int nc>
VecN<t,nc> operator-(VecN<T,nc> lhs, const VecN<T,nc> &rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size) i = 0; i<lhs.size; i++)
	{
		temp.dat[i] = rhs.dat[i] + lhs.dat[i];
	}
	return temp;
}

template <class T, unsigned int nc>
VecN<T,nc> operator/(const VecN<T,nc> &lhs, const T rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size) i = 0; i<lhs.size; i++)
	{
		temp.dat[i] = lhs.dat[i] / rhs;
	}
	return temp;
}

template <class T, unsigned int nc>
bool operator==(const VecN &lhs, const VecN &rhs)
{
	for (int i = 0; i<nc; i++)
	{
		if (lhs.dat[i]-rhs.dat[i] >1e-10)
		{
			return false;
		}
	}
	return true;
}

template <class T, unsigned int nc>
bool operator!=(const VecN &lhs, const VecN &rhs)
{
	for (int i = 0; i<nc; i++)
	{
		if (lhs.dat[i]-rhs.dat[i]>1e-10)
		{
			return true;
		}
	}
	return false;
}

template <class T, unsigned int nc>
double L2Norm(VecN a)
{
	double sqlen = 0.0;
	for (int i = 0; i<nc; i++)
	{
		sqlen += (dat[i]*dat[i]);
	}
	return sqrt(sqlen);
}

////////////////////////////////////////////////////////////////////////////////
//3-D DOUBLE VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

Vec3::Vec3()
{
	dat[0] = 0;
	dat[1] = 0;
	dat[2] = 0;
}

Vec3::Vec3(double x, double y, double z)
{
	Set(x,y,z);
}

void Vec3::Set(double x, double y, double z)
{
	dat[0] = x;
	dat[1] = y;
	dat[2] = z;
}

void Vec3::Normalize()
{
	double l = L2Norm(*this);
	x /= l;
	y /= l;
	z /= l;
}

Vec3 cross(Vec3 a, Vec3 b)
{
	Vec3 result;
	result.x=a.y*b.z-a.z*b.y;
	result.y=a.z*b.x-a.x*b.z;
	result.z=a.x*b.y-a.y*b.x;
	return result;
}

const double Vec3::xf() const
{
	return dat[0];
}

const double Vec3::yf() const
{
	return dat[1];
}

const double Vec3::zf() const
{
	return dat[2];
}	