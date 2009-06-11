/*  
 * Copyright (C) 2007 University of South Australia
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * Contributors: 
 *   Andrew Cunningham <andrewcunningham@mac.com>
 * 
 */

#ifndef Coord_h
#define Coord_h

#include <sstream>
#include <cmath>

// 2-dimensional coordinates
template <class T>
struct Coord {
	T x, y;
	
	Coord() 
		: x(0)
		, y(0) 
	{
	}

	Coord(T x, T y) 
		: x(x)
		, y(y) 
	{
	}
    
	Coord(const Coord<T>& orig) 
		: x(orig.x)
		, y(orig.y) 
	{
	}

	Coord& operator=(const Coord<T>& orig)
	{
		x = orig.x; 
        y = orig.y; 
        return *this;
	}

	Coord operator+(T v)
	{
        return Coord<T>(x + v, y + v);
	}

	Coord operator+(const Coord<T>& v)
	{
        return Coord<T>(x + v.x, y + v.y);
	}

	Coord& operator+=(T v)
	{
		x += v; 
        y += v; 
        return *this;
	}

	Coord& operator+=(const Coord<T>& v)
	{
		x += v.x; 
        y += v.y; 
        return *this;
	}

	Coord operator-(T v) const
	{
        return Coord<T>(x - v, y - v);
	}

	Coord operator-(const Coord<T>& v)
	{
        return Coord<T>(x - v.x, y - v.y);
	}

	Coord operator*(const Coord<T>& v)
	{
        return Coord<T>(x * v.x, y * v.y);
	}

	Coord operator*(T v) const
	{
        return Coord<T>(x * v, y * v);
	}

	Coord& operator*=(T d)
	{
		x *= d;
		y *= d;
		return *this;
	}

	Coord operator/(T d) const
	{
		return Coord<T>(x / d, y / d);
	}

	Coord& operator/=(T d)
	{
		x /= d;
		y /= d;
		return *this;
	}

    void set(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

	double len(void)
	{
		return hypot(x, y);
	}

	Coord<T> normalise(void) const
	{
		return Coord(x / hypot(x, y), y / hypot(x, y));
	}

	Coord<T> unit(void) const
	{
		return Coord(x / hypot(x, y), y / hypot(x, y));
	}
};

template <class T>
std::ostream& operator<<( std::ostream& os, const Coord<T>& t ){
	os << t.x << " " << t.y;
	return os;
}

template <class T>
std::istream& operator>>( std::istream& is, Coord<T>& t ){
	is >> t.x >> t.y;
	return is;
}

// math functions
template <class T>
Coord<T> operator+(Coord<T>& a, Coord<T>& b){
	return Coord<T>(a.x + b.x, a.y + b.y);
}


// 3-dimensional coordinates
template <class T>
struct Coord3 {
	T x, y, z;
	
	Coord3() 
		: x(0)
		, y(0)
        , z(0)
	{
	}

	Coord3(T x, T y, T z) 
		: x(x)
		, y(y)
        , z(z)
	{
	}

	Coord3(const Coord<T>& orig) 
		: x(orig.x)
		, y(orig.y)
        , z(orig.z)
	{
	}

    void set(const Coord3<T>& t)
    {
        this->x = t.x;
        this->y = t.y;
        this->z = t.z;
    }

    void set(const T& x, const T& y, const T& z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

	int operator==(const Coord3<T>& t) const
	{
        return x == t.x && y == t.y && z == t.z;
	}
	
    int operator!=(const Coord3<T>& t) const
	{
        return !(x == t.x && y == t.y && z == t.z);
	}

	Coord3& operator=(const Coord3<T>& orig)
	{
		x = orig.x; 
        y = orig.y; 
        z = orig.z; 
        return *this;
	}

	Coord3 operator-(const Coord3<T>& c) const
	{
		return Coord3<T>(x - c.x, y - c.y, z - c.z);
	}

	Coord3 operator+(const Coord3<T>& c) const
	{
		return Coord3<T>(x + c.x, y + c.y, z + c.z);
	}

	Coord3& operator+=(const Coord3<T>& c)
	{
		x += c.x;
		y += c.y;
		z += c.z;
		return *this;
	}

	Coord3 operator*(const double m) const
	{
		return Coord3<T>(x * m, y * m, z * m);
	}

	Coord3 operator*(const Coord3<T>& m) const
	{
		return Coord3<T>(x * m.x, y * m.y, z * m.z);
	}

	Coord3& operator*=(const T& m)
	{
		x *= m;
		y *= m;
		z *= m;
		return *this;
	}

	Coord3& operator*=(const Coord3<T>& m)
	{
		x *= m.x;
		y *= m.y;
		z *= m.z;
		return *this;
	}

	Coord3 operator/(const double m) const
	{
		return Coord3<T>(x / m, y / m, z / m);
	}

	Coord3 operator/(const Coord3<T>& m) const
	{
		return Coord3<T>(x / m.x, y / m.y, z / m.z);
	}
	
	Coord3 operator/=(const T& d)
	{
		x = x / d;
		y = y / d;
		z = z / d;
		return *this;
	}

	Coord3& operator/=(const Coord3<T>& m)
	{
		x = x / m.x;
		y = y / m.y;
		z = z / m.z;
		return *this;
	}

	double len(void)
	{
		return sqrt(x * x + y * y + z * z);
	}

	Coord3 cross(const Coord3<T>& v)
	{
		return Coord3<T>(y * v.z - z * v.y, 
						 z * v.x - x * v.z, 
						 x * v.y - y * v.x);
	}

	Coord3& cross(const Coord3<T>& v1, const Coord3<T>& v2)
	{
		x = v1.y * v2.z - v1.z * v2.y; 
		y = v1.z * v2.x - v1.x * v2.z; 
		z = v1.x * v2.y - v1.y * v2.x;
		return *this;
	}

	double dot(const Coord3<T>& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	double angle(const Coord3<T>& v)
	{
		double dp   = x * v.x + y * v.y + z * v.z;
		double alen = sqrt(x * x + y * y + z * z);
		double blen = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		return acos(dp / (alen + blen));
	}

	Coord3& normalise(void)
	{
		T l = sqrt(x * x + y * y + z * z);
		x /= l;
		y /= l;
		z /= l;
		return *this;
	}

	Coord3 unit(void)
	{
		T l = sqrt(x * x + y * y + z * z);
		return Coord3<T>(x / l, y / l, z / l);
	}

	bool isZero(void) const
	{
		return x == y == z == 0;
	}

	/** linearly interpolates between this Coord and Coord t1, such:
	 *      this = (1 - alpha) * this + alpha * t1 */
	void interpolate(const Coord3<T>& t1, double alpha)
	{
		double malpha = 1 - alpha;
		x = x * malpha + alpha * t1.x;
		y = y * malpha + alpha * t1.y;
		z = z * malpha + alpha * t1.z;
	}
};

template <class T>
Coord3<T> operator*(const Coord3<T>& c, const double m)
{
	return Coord3<T>(c.x * m, c.y * m, c.z * m);
}


template <class T>
std::ostream& operator<<( std::ostream& os, const Coord3<T>& t ){
	os << t.x << " " << t.y << " " << t.z;
	return os;
}

template <class T>
std::istream& operator>>( std::istream& is, Coord3<T>& t ){
	is >> t.x >> t.y >> t.z;
	return is;
}


// 4-dimensional coordinates
template <class T>
struct Coord4 {
	T t, u, v, w;

	Coord4()
		: t(0)
		, u(0)
		, v(0)
		, w(0)
	{ }

	Coord4(T t, T u, T v, T w) 
		: t(t)
		, u(u)
		, v(v)
		, w(w) 
	{ }

	Coord4(const Coord4<T>& orig) 
		: t(orig.t)
		, u(orig.u)
		, v(orig.v)
		, w(orig.w) 
	{ }

	Coord4& operator=(const Coord4<T>& orig)
	{
		t = orig.t; 
        u = orig.u; 
        v = orig.v; 
        w = orig.w; 
        return *this;
	}
    
	Coord4 operator*(const Coord4<T>& m)
	{
        return Coord4<T>(t * m.t, u * m.u, v * m.v, w * m.w);
	}

	Coord4 operator*(T m) const
	{
        return Coord4<T>(t * m, u * m, v * m, w * m);
	}

	Coord4& operator*=(T d)
	{
		t *= d;
		u *= d;
		v *= d;
		w *= d;
		return *this;
	}

    void set(const T& t, const T& u, const T& v, const T& w)
    {
        this->t = t;
        this->u = u;
        this->v = v;
        this->w = w;
    }

};

template <class T>
bool contains(const Coord4<T>& r, const Coord<T>& p)
{
	return p.x > r.t && p.x < r.v && p.y > r.u && p.y < r.w;
}

template <class T>
bool intersects(const Coord4<T>& r1, const Coord4<T>& r2)
{
    return ! ( r2.t > r1.v || r2.v < r1.t || r2.u > r1.w || r2.w < r1.u);
}

template <class T>
std::ostream& operator<<( std::ostream& os, const Coord4<T>& t ){
	os << t.t << " " << t.u << " " << t.v << " " << t.w;
	return os;
}

template <class T>
std::istream& operator>>( std::istream& is, Coord4<T>& t ){
	is >> t.t >> t.u >> t.v >> t.w;
	return is;
}

typedef Coord<int>     Coord2i;
typedef Coord<float>   Coord2f;
typedef Coord<double>  Coord2d;
typedef Coord3<int>    Coord3i;
typedef Coord3<float>  Coord3f;
typedef Coord3<double> Coord3d;
typedef Coord4<int>    Coord4i;
typedef Coord4<float>  Coord4f;
typedef Coord4<double> Coord4d;


#endif
