#ifndef VECTOR_NAK_IS_INCLUDED
#define VECTOR_NAK_IS_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
//GENERIC VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

template <class T>
class Vec
{
protected:
	T *dat;
	long long int mysize;
	long long int available;
	void Cleanup();
	void Grow();
	void CopyFrom(const Vec &in);
	void Swap(Vec &in);
public:
	Vec();
	Vec(const Vec &in);
	Vec(Vec &&in);
	Vec &operator=(const Vec &in);
	Vec &operator=(Vec &&in);
	~Vec();
	long long int size() const;
	void clear();
	void resize(long long int newsize);
	T &operator[](const int n);
	const T &operator[](const int n) const;
	void push_back(const T &incoming);
	void pop_back();
	Vec *begin();
	Vec *end();
	const T *data() const;
	const Vec GetVecData() const;
	const T &front() const;
	T &front();
	const T &back() const;
	T &back();
};

////////////////////////////////////////////////////////////////////////////////
//LENGTH VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

template <class T, long long int nc>
class VecN : public Vec<T>
{
public:
	VecN();
    
	VecN &operator+=(const VecN rhs);
	VecN &operator-=(const VecN rhs);
//	friend T dot(VecN <T,nc> a, VecN <T,nc> b);
//	friend VecN operator*(const T lhs, VecN rhs);
//	friend VecN operator*(VecN lhs, const T rhs);
//	friend VecN operator+(VecN lhs, const VecN &rhs);
//	friend VecN operator-(VecN lhs, const VecN &rhs);
//	friend VecN <T,nc> operator/(const VecN <T,nc> &lhs, const T rhs);
//    friend VecN <T,nc> Div(const VecN <T,nc> &lhs, const T rhs);
//	friend bool operator==(const VecN &lhs, const VecN &rhs);
//	friend bool operator!=(const VecN &lhs, const VecN &rhs);
//	friend double L2Norm(VecN a);
};

////////////////////////////////////////////////////////////////////////////////
//3-D DOUBLE VECTOR CLASS
////////////////////////////////////////////////////////////////////////////////

class Vec3 : public VecN<double,3>
{
public :
    int label;
    
public:
	Vec3();
	Vec3(double x, double y, double z);
	void Set(double x, double y, double z);
	void Normalize();
    
    Vec3 &operator+=(const Vec3 &rhs);
    Vec3 &operator-=(const Vec3 &rhs);
    
	static Vec3 cross(Vec3 a, Vec3 b);
	const double xf() const;
	const double yf() const;
	const double zf() const;
    
    friend double dot(Vec3 a, Vec3 b);
    friend Vec3 operator*(const double lhs, Vec3 rhs);
    friend Vec3 operator*(Vec3 lhs, const double rhs);
    friend Vec3 operator+(Vec3 lhs, const Vec3 &rhs);
    friend Vec3 operator-(Vec3 lhs, const Vec3 &rhs);
    friend Vec3 operator/(const Vec3 &lhs, const double rhs);
    friend bool operator==(const Vec3 &lhs, const Vec3 &rhs);
    friend bool operator!=(const Vec3 &lhs, const Vec3 &rhs);
    friend double L2Norm(Vec3 a);
    
    inline VecN<double,3> conv() const
    {
        return VecN<double,3>(*this);
    }
    inline static Vec3 GetProjection(Vec3 N, Vec3 X, Vec3 Y);
};

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
    return *this;
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
    return *this;
}

template <class T, long long int nc>
VecN<T,nc> &VecN<T,nc>::operator-=(const VecN<T,nc> rhs)
{
    for (decltype(this->size()) i=0; i<this->size(); i++)
    {
        this->dat[i] -= rhs.dat[i];
    }
    return *this;
}

template <class T, long long int nc>
T dot(VecN<T,nc> a, VecN<T,nc> b)
{
    T result = 0.0;
    for (decltype(a.size()) i = 0; i<a.size(); i++)
    {
        result += a.data()[i]*b.data()[i];
    }
    return result;
}

template <class T, long long int nc>
VecN<T,nc> operator*(const T lhs, VecN<T,nc> rhs)
{
    VecN<T,nc> temp;
    for (decltype (rhs.size()) i = 0; i<rhs.size(); i++)
    {
        temp[i] = lhs * rhs.data()[i];
    }
    return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator*(VecN<T,nc> lhs, const T rhs)
{
    VecN<T,nc> temp;
    for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
    {
        temp[i] = rhs * lhs.data()[i];
    }
    return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator+(VecN<T,nc> lhs, const VecN<T,nc> &rhs)
{
    VecN<T,nc> temp;
    for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
    {
        temp[i] = rhs.data()[i] + lhs.data()[i];
    }
    return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator-(VecN<T,nc> lhs, const VecN<T,nc> &rhs)
{
    VecN<T,nc> temp;
    for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
    {
        temp[i] = rhs.data()[i] + lhs.data()[i];
    }
    return temp;
}

template <class T, long long int nc>
VecN<T,nc> operator/(const VecN<T,nc> &lhs, const T rhs)
{
    VecN<T,nc> temp;
    for (decltype (lhs.size()) i = 0; i<lhs.size(); i++)
    {
        temp[i] = lhs.data()[i] / rhs;
    }
    return temp;
}

template <class T, long long int nc>
bool operator==(const VecN<T,nc> &lhs, const VecN<T,nc> &rhs)
{
    for (int i = 0; i<nc; i++)
    {
        if (lhs.data()[i]-rhs.data()[i] >1e-10)
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
        if (lhs.data()[i]-rhs.data()[i]>1e-10)
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
        sqlen += (a.data()[i]*a.data()[i]);
    }
    return sqrt(sqlen);
}

//inline double DistancePtToLine(Vec3 x1, Vec3 x2, Vec3 x0);

inline extern double DistancePtToLine(Vec3 x1, Vec3 x2, Vec3 x0)

{
	double t = -1.0*dot((x1-x0),(x2-x1))/pow((L2Norm(x2-x1)),2.0);
	auto temp = ((x1-x0)+(x2-x0))*t;
	return L2Norm(temp);
}

inline extern Vec3 Vec3::GetProjection(Vec3 Normal, Vec3 PtPlane, Vec3 Pt)
{
    Normal.Normalize();
    double d = dot(Normal, PtPlane-Pt);
    return Pt - d*Normal;
}

#endif