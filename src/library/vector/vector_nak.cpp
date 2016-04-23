#include "vector_nak.h"

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

Vec3 &Vec3::operator+=(const Vec3 &rhs)
{
    *this = *this + rhs;
    return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &rhs)
{
    *this = *this - rhs;
    return *this;
}


Vec3 Vec3::cross(Vec3 a, Vec3 b)
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
    return dot<double,3>(a.conv(),b.conv());
}

Vec3 operator*(const double lhs, Vec3 rhs)
{
    auto temp = lhs*rhs.conv();
    auto dat = temp.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator*(Vec3 lhs, const double rhs)
{
    auto temp = rhs*lhs.conv();
    auto dat = temp.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator+(Vec3 lhs, const Vec3 &rhs)
{
    auto temp = lhs.conv()+rhs.conv();
    auto dat = temp.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator-(Vec3 lhs, const Vec3 &rhs)
{
    auto temp = lhs.conv()-rhs.conv();
    auto dat = temp.GetVecData();
    return Vec3(dat[0],dat[1],dat[2]);
}

Vec3 operator/(const Vec3 &lhs, const double rhs)
{
    const VecN <double,3> &v3=lhs;
    auto temp = v3/rhs; // Div(v3,rhs);
    auto dat = temp.data();
    return Vec3(dat[0],dat[1],dat[2]);
}

bool operator==(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.conv()==rhs.conv();
}

bool operator!=(const Vec3 &lhs, const Vec3 &rhs)
{
    return lhs.conv()!=rhs.conv();
}

double L2Norm(Vec3 a)
{
    return L2Norm<double,3>(a.conv());
}
