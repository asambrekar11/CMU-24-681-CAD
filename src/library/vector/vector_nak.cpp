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
	mysize = 0;
	available = 0;
}

template <class T>
void Vec<T>::Grow()
{
	if (available == 0)
	{
		dat = new T[1];
		available = 1;
	}
	else
	{
		available*=2;
		T *newdat = new T[available];
		for (decltype(mysize) i = 0; i<mysize; i++)
		{
			newdat[i]=dat[i];
		}
		if (dat!=nullptr)
		{
			delete []dat;
		}
		dat=newdat;
	}
}

template <class T>
void Vec<T>::CopyFrom(const Vec<T> &in)
{
	if (this != &in)
	{
		Cleanup();
		if (in.mysize>0)
		{
			this->available = in.available;
			this->mysize = in.mysize;
			this->dat = new T[this->available];
			for (decltype(mysize) i = 0; i<mysize; i++)
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
Vec<T>::Vec()
{
	dat = nullptr;
	mysize = 0;
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
Vec<T> &Vec<T>::operator=(Vec<T> &&in)
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
long long int Vec<T>::size() const
{
	return mysize;
}

template <class T>
void Vec<T>::clear()
{
	Cleanup();
}

template <class T>
void Vec<T>::resize(long long int newsize)
{
	if (0 >= newsize)
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
		for(decltype(mysize) i=0; i<mysize && i<newsize; ++i)
		{
			newdat[i]=dat[i];
		}
		if(nullptr!=dat)
		{
			delete [] dat;
		}
		dat=newdat;
		mysize=newsize;
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
void Vec<T>::push_back(const T &incoming)
{
	if (available<=mysize)
	{
		Grow();
	}
	dat[mysize] = incoming;
	mysize++;
}

template <class T>
void Vec<T>::pop_back()
{
	mysize--;
}

template <class T>
Vec<T> *Vec<T>::begin()
{
	return &dat[0];
}

template <class T>
Vec<T> *Vec<T>::end()
{
	return &dat[mysize-1];
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

template<class T>
const T &Vec<T>::front() const
{
	return dat[0];
}

template<class T>
T &Vec<T>::front()
{
	return dat[0];
}

template<class T>
const T &Vec<T>::back() const
{
	dat[mysize-1];
}

template<class T>
T &Vec<T>::back()
{
	dat[mysize-1];
}

////////////////////////////////////////////////////////////////////////////////
//LENGTH VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

template <class T, long long int nc>
VecN<T,nc>::VecN()
{
	long long int reqsize = nc;
	this->resize(reqsize);
}

template <class T, long long int nc>
VecN<T,nc> &VecN<T,nc>::operator+=(const VecN<T,nc> rhs)
{
	for (decltype(this->size()) i=0; i<this->size(); i++)
	{
		this->dat[i] += rhs.dat[i];
	}
}

template <class T, long long int nc>
VecN<T,nc> &VecN<T,nc>::operator-=(const VecN<T,nc> rhs)
{
	for (decltype(this->size()) i=0; i<this->size(); i++)
	{
		this->dat[i] -= rhs.dat[i];
	}
}

template <class T, long long int nc>
T dot(VecN<T,nc> a, VecN<T,nc> b)
{
	T result;
	for (decltype(a.size()) i = 0; i<a.size(); i++)
	{
		result += a.dat[i]*b.dat[i];
	}
	return result;
}

template <class T, long long int nc>
VecN<T,nc> operator*(const T lhs, VecN<T,nc> rhs)
{
	VecN<T,nc> temp;
	for (decltype (rhs.size()) i = 0; i<rhs.size(); i++)
	{
		temp.dat[i] = lhs * rhs.dat[i];
	}
	return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator*(VecN<T,nc> lhs, const T rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
	{
		temp.dat[i] = rhs * lhs.dat[i];
	}
	return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator+(VecN<T,nc> lhs, const VecN<T,nc> &rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
	{
		temp.dat[i] = rhs.dat[i] + lhs.dat[i];
	}
	return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator-(VecN<T,nc> lhs, const VecN<T,nc> &rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
	{
		temp.dat[i] = rhs.dat[i] + lhs.dat[i];
	}
	return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator/(const VecN<T,nc> &lhs, const T rhs)
{
	VecN<T,nc> temp;
	for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
	{
		temp.dat[i] = lhs.dat[i] / rhs;
	}
	return temp;
}

template <class T, long long int nc>
bool operator==(const VecN<T,nc> &lhs, const VecN<T,nc> &rhs)
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

template <class T, long long int nc>
bool operator!=(const VecN<T,nc> &lhs, const VecN<T,nc> &rhs)
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

template <class T, long long int nc>
double L2Norm(VecN<T,nc> a)
{
	double sqlen = 0.0;
	for (int i = 0; i<nc; i++)
	{
		sqlen += (a.dat[i]*a.dat[i]);
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
	dat[0] /= l;
	dat[1] /= l;
	dat[2] /= l;
}

Vec3 cross(Vec3 a, Vec3 b)
{
	Vec3 result;
	result.dat[0]=a.dat[1]*b.dat[2]-a.dat[2]*b.dat[1];
	result.dat[1]=a.dat[2]*b.dat[0]-a.dat[0]*b.dat[2];
	result.dat[2]=a.dat[0]*b.dat[1]-a.dat[1]*b.dat[0];
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

double dot(Vec3 a, Vec3 b)
{
    return dot(a.conv(),b.conv());
}

Vec3 operator*(const double lhs, Vec3 rhs)
{
    auto result = (lhs*rhs.conv());
    auto dat = result.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator*(Vec3 lhs, const double rhs)
{
    auto result = (lhs.conv()*rhs);
    auto dat = result.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator+(Vec3 lhs, const Vec3 &rhs)
{
    auto result = (lhs.conv()+rhs.conv());
    auto dat = result.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator-(Vec3 lhs, const Vec3 &rhs)
{
    auto result = (lhs.conv()-rhs.conv());
    auto dat = result.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator/(const Vec3 &lhs, const double rhs)
{
    auto result = (lhs.conv()/rhs);
    auto dat = result.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

bool operator==(const Vec3 &lhs, const Vec3 &rhs)
{
    return (lhs.conv()==rhs.conv());
}

bool operator!=(const Vec3 &lhs, const Vec3 &rhs)
{
    return (lhs.conv()!=rhs.conv());
}

double L2Norm(Vec3 a)
{
    return L2Norm(a.conv());
}
