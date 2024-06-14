#pragma once

#include "defines.hpp"
#include "mathBasic.hpp"





template<typename dataType>
class Vector3
{
	public:
		
		dataType x;
		dataType y;
		dataType z;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		constexpr Vector3();
		constexpr Vector3(dataType x, dataType y, dataType z);
		constexpr Vector3(const Vector3& start, const Vector3& end);
		
		double absolute() const;
		constexpr inline Vector3 positiveVector() const;
		constexpr inline Vector3 sign() const;
		constexpr inline Vector3<double> unitVector() const;
		
		double angle(Vector3 v) const;
		
		constexpr inline Vector3& min_x(Vector3& v) const;
		constexpr inline Vector3& max_x(Vector3& v) const;
		
		constexpr inline Vector3& min_y(Vector3& v) const;
		constexpr inline Vector3& max_y(Vector3& v) const;
		
		constexpr inline Vector3& min_z(Vector3& v) const;
		constexpr inline Vector3& max_z(Vector3& v) const;
		
		constexpr inline dataType min() const;
		constexpr inline dataType max() const;
		
		constexpr inline bool operator==(const Vector3& v) const;
		constexpr inline bool operator!=(const Vector3& v) const;
		
		constexpr inline Vector3& operator=(const Vector3& v);
		
		constexpr inline Vector3 operator-(const Vector3& v) const;
		constexpr inline Vector3 operator+(const Vector3& v) const;
		
		template<typename dataType2>
		constexpr inline Vector3 operator*(dataType2 scalar) const;
		
		constexpr inline double operator*(Vector3 v) const;
		
		template<typename dataType2>
		constexpr inline Vector3 operator/(dataType2 scalar) const;
		
		constexpr inline Vector3& operator+=(const Vector3& v);
		constexpr inline Vector3& operator-=(const Vector3& v);
		
		template<typename dataType2>
		constexpr inline Vector3& operator*=(dataType2 scalar);
		
		constexpr inline Vector3& operator*=(Vector3 v);
		
		template<typename dataType2>
		constexpr inline Vector3& operator/=(dataType2 scalar);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

template<typename dataType>
constexpr inline Vector3<dataType>::Vector3()
	:	x(0),
		y(0),
		z(0)
{
	
}

template<typename dataType>
constexpr inline Vector3<dataType>::Vector3(dataType x, dataType y, dataType z)
	: x((dataType) x),
		y((dataType) y),
		z((dataType) z)
{
	
}

template<typename dataType>
constexpr inline Vector3<dataType>::Vector3(const Vector3& start, const Vector3& end)
	:	x((dataType) (end.x - start.x)),
		y((dataType) (end.y - start.y)),
		z((dataType) (end.z - start.z))
{
	
}







template<typename dataType>
double Vector3<dataType>::absolute() const
{
	Vector3<double> temp(x, y, z);
	double result = sqrt((temp.x * temp.x) + (temp.y * temp.y) + (temp.z * temp.z));
	return(result);
}


template<typename dataType>
constexpr inline Vector3<dataType> Vector3<dataType>::positiveVector() const
{
	Vector3 result(*this);
	if(x < 0)
	{
		result.x *= -1;
	}
	if(y < 0)
	{
		result.y *= -1;
	}
	if(z < 0)
	{
		result.z *= -1;
	}
	return(result);
}


template<typename dataType>
constexpr inline Vector3<dataType> Vector3<dataType>::sign() const
{
	Vector3 result(0, 0, 0);
	if(x < 0)
	{
		result.x = -1;
	}
	if(x > 0)
	{
		result.x = 1;
	}
	
	if(y < 0)
	{
		result.y = -1;
	}
	if(y > 0)
	{
		result.y = 1;
	}
	
	if(z < 0)
	{
		result.z = -1;
	}
	if(z > 0)
	{
		result.z = 1;
	}
	
	return(result);
}


template<typename dataType>
constexpr inline Vector3<double> Vector3<dataType>::unitVector() const
{
	Vector3<double> unit(x, y, z);
	double length = absolute();
	unit.x /= length;
	unit.y /= length;
	unit.z /= length;
	
	return(unit);
}


template<typename dataType>
double Vector3<dataType>::angle(Vector3 v) const
{
	double nominator = *this * v;
	double denominator = absolute() * v.absolute();
	double angle = acos(nominator / denominator);
	return(Math::radToDeg(angle));
}







template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::min_x(Vector3& v) const
{
	dataType min = Math::min<dataType>(x, v.x);
	if(min == v.x)
	{
		return(v);
	}
	return(*this);
}


template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::max_x(Vector3& v) const
{
	dataType max = Math::max<dataType>(x, v.x);
	if(max == v.x)
	{
		return(v);
	}
	return(*this);
}







template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::min_y(Vector3& v) const
{
	dataType min = Math::min<dataType>(y, v.y);
	if(min == v.y)
	{
		return(v);
	}
	return(*this);
}


template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::max_y(Vector3& v) const
{
	dataType max = Math::max<dataType>(y, v.y);
	if(max == v.y)
	{
		return(v);
	}
	return(*this);
}







template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::min_z(Vector3& v) const
{
	dataType min = Math::min<dataType>(z, v.z);
	if(min == v.z)
	{
		return(v);
	}
	return(*this);
}


template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::max_z(Vector3& v) const
{
	dataType max = Math::max<dataType>(z, v.z);
	if(max == v.z)
	{
		return(v);
	}
	return(*this);
}







template<typename dataType>
constexpr inline dataType Vector3<dataType>::min() const
{
	return(Math::min(x, y, z));
}


template<typename dataType>
constexpr inline dataType Vector3<dataType>::max() const
{
	return(Math::max(x, y, z));
}







template<typename dataType>
constexpr inline bool Vector3<dataType>::operator==(const Vector3& v) const
{
	if(x == v.x && y == v.y && z == v.z)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
constexpr inline bool Vector3<dataType>::operator!=(const Vector3& v) const
{
	if(x != v.x || y != v.y || z != v.z)
	{
		return(true);
	}
	return(false);
}







template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::operator=(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	
	return(*this);
}







template<typename dataType>
constexpr inline Vector3<dataType> Vector3<dataType>::operator-(const Vector3& v) const
{
	return(Vector3(x - v.x, y - v.y, z - v.z));
}


template<typename dataType>
constexpr inline Vector3<dataType> Vector3<dataType>::operator+(const Vector3& v) const
{
	return(Vector3(x + v.x, y + v.y, z + v.z));
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector3<dataType> Vector3<dataType>::operator*(dataType2 scalar) const
{
	return(Vector3(x * scalar, y * scalar, z * scalar));
}


template<typename dataType>
constexpr inline double Vector3<dataType>::operator*(Vector3 v) const
{
	double scalar_x = ((double) x) * v.x;
	double scalar_y = ((double) y) * v.y;
	double scalar_z = ((double) z) * v.z;
	return(scalar_x + scalar_y + scalar_z);
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector3<dataType> Vector3<dataType>::operator/(dataType2 scalar) const
{
	return(Vector3(x / scalar, y / scalar, z / scalar));
}







template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::operator+=(const Vector3& v)
{
	*this = *this + v;
	return(*this);
}


template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::operator-=(const Vector3& v)
{
	*this = *this - v;
	return(*this);
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector3<dataType>& Vector3<dataType>::operator*=(dataType2 scalar)
{
	*this = *this * scalar;
	return(*this);
}


template<typename dataType>
constexpr inline Vector3<dataType>& Vector3<dataType>::operator*=(Vector3 v)
{
	*this = *this * v;
	return(*this);
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector3<dataType>& Vector3<dataType>::operator/=(dataType2 scalar)
{
	*this = *this / scalar;
	return(*this);
}