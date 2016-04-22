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
	friend T dot(VecN a, VecN b);
	friend VecN operator*(const T lhs, VecN rhs);
	friend VecN operator*(VecN lhs, const T rhs);
	friend VecN operator+(VecN lhs, const VecN &rhs);
	friend VecN operator-(VecN lhs, const VecN &rhs);
	friend VecN operator/(const VecN &lhs, const T rhs);
	friend bool operator==(const VecN &lhs, const VecN &rhs);
	friend bool operator!=(const VecN &lhs, const VecN &rhs);
	friend double L2Norm(VecN a);
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

inline double DistancePtToLine(Vec3 x1, Vec3 x2, Vec3 x0)
{
	double t = -1.0*dot((x1-x0),(x2-x1))/pow((L2Norm(x2-x1)),2.0);
	auto temp = ((x1-x0)+(x2-x0))*t;
	return L2Norm(temp);
}

inline Vec3 Vec3::GetProjection(Vec3 Normal, Vec3 PtPlane, Vec3 Pt)
{
    Normal.Normalize();
    double d = dot(Normal, PtPlane-Pt);
    return Pt - d*Normal;
}

#endif