#pragma once

#include "defines.hpp"
#include "mathBasic.hpp"





template<typename dataType>
class Vector2
{
	public:
		
		dataType x;
		dataType y;
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		constexpr inline Vector2();
		constexpr inline Vector2(dataType x, dataType y);
		constexpr inline Vector2(const Vector2& start, const Vector2& end);
		
		double absolute() const;
		constexpr inline Vector2 positiveVector() const;
		constexpr inline Vector2 sign() const;
		constexpr inline Vector2<double> unitVector() const;
		
		double angle(Vector2 v) const;
		double angleToX() const;
		double angleToY() const;
		
		constexpr inline Vector2& min_x(Vector2& v) const;
		constexpr inline Vector2& max_x(Vector2& v) const;
		
		constexpr inline Vector2& min_y(Vector2& v) const;
		constexpr inline Vector2& max_y(Vector2& v) const;
		
		constexpr inline dataType min() const;
		constexpr inline dataType max() const;
		
		
		constexpr inline bool operator==(const Vector2& v) const;
		constexpr inline bool operator!=(const Vector2& v) const;
		
		constexpr inline Vector2& operator=(const Vector2& v);
		
		constexpr inline Vector2 operator-(const Vector2& v) const;
		constexpr inline Vector2 operator+(const Vector2& v) const;
		
		template<typename dataType2>
		constexpr inline Vector2 operator*(dataType2 scalar) const;
		
		constexpr inline double operator*(Vector2 v) const;
		
		template<typename dataType2>
		constexpr inline Vector2 operator/(dataType2 scalar) const;
		
		constexpr inline Vector2& operator+=(const Vector2& v);
		constexpr inline Vector2& operator-=(const Vector2& v);
		
		template<typename dataType2>
		constexpr inline Vector2& operator*=(dataType2 scalar);
		
		constexpr inline Vector2& operator*=(Vector2 v);
		
		template<typename dataType2>
		constexpr inline Vector2& operator/=(dataType2 scalar);
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
constexpr inline Vector2<dataType>::Vector2()
	:	x(0),
		y(0)
{
	
}


template<typename dataType>
constexpr inline Vector2<dataType>::Vector2(dataType x, dataType y)
	: x((dataType) x),
		y((dataType) y)
{
	
}


template<typename dataType>
constexpr inline Vector2<dataType>::Vector2(const Vector2& start, const Vector2& end)
	:	x((dataType) (end.x - start.x)),
		y((dataType) (end.y - start.y))
{
	
}







template<typename dataType>
double Vector2<dataType>::absolute() const
{
	Vector2<double> temp(x, y);
	double result = sqrt((temp.x * temp.x) + (temp.y * temp.y));
	return(result);
}


template<typename dataType>
constexpr inline Vector2<dataType> Vector2<dataType>::positiveVector() const
{
	Vector2 result(*this);
	if(x < 0)
	{
		result.x *= -1;
	}
	if(y < 0)
	{
		result.y *= -1;
	}
	return(result);
}


template<typename dataType>
constexpr inline Vector2<dataType> Vector2<dataType>::sign() const
{
	Vector2 result(0, 0);
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
	
	return(result);
}


template<typename dataType>
constexpr inline Vector2<double> Vector2<dataType>::unitVector() const
{
	Vector2<double> unit(x, y);
	double length = absolute();
	unit.x /= length;
	unit.y /= length;
	
	return(unit);
}







template<typename dataType>
double Vector2<dataType>::angle(Vector2 v) const
{
	double nominator = *this * v;
	double denominator = absolute() * v.absolute();
	double angle = acos(nominator / denominator);
	return(Math::radToDeg(angle));
}


template<typename dataType>
double Vector2<dataType>::angleToX() const
{
	double angle = 180.0 * asin((double) y / absolute()) / Math::pi<double>;
	
	if(x >= 0 && y >= 0)
	{
		return(angle);
	}
	if(x < 0 && y >= 0)
	{
		return(180.0 - angle);
	}
	if(x < 0 && y < 0)
	{
		return(180.0 - angle);
	}
	if(x >= 0 && y < 0)
	{
		return(360.0 + angle);
	}
	
	return(0);
}


template<typename dataType>
double Vector2<dataType>::angleToY() const
{
	double angle = 180.0 * asin((double) x / absolute()) / Math::pi<double>;
	
	if(x >= 0 && y >= 0)
	{
		return(360.0 - angle);
	}
	if(x < 0 && y >= 0)
	{
		return(-angle);
	}
	if(x < 0 && y < 0)
	{
		return(180.0 + angle);
	}
	if(x >= 0 && y < 0)
	{
		return(180.0 + angle);
	}
	
	return(0);
}







template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::min_x(Vector2& v) const
{
	if(x < v.x)
	{
		return(*this);
	}
	return(v);
}


template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::max_x(Vector2& v) const
{
	if(x > v.x)
	{
		return(*this);
	}
	return(v);
}







template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::min_y(Vector2& v) const
{
	if(y < v.y)
	{
		return(*this);
	}
	return(v);
}


template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::max_y(Vector2& v) const
{
	if(y > v.y)
	{
		return(*this);
	}
	return(v);
}







template<typename dataType>
constexpr inline dataType Vector2<dataType>::min() const
{
	return(Math::min(x, y));
}


template<typename dataType>
constexpr inline dataType Vector2<dataType>::max() const
{
	return(Math::max(x, y));
}







template<typename dataType>
constexpr inline bool Vector2<dataType>::operator==(const Vector2& v) const
{
	if(x == v.x && y == v.y)
	{
		return(true);
	}
	return(false);
}


template<typename dataType>
constexpr inline bool Vector2<dataType>::operator!=(const Vector2& v) const
{
	if(x != v.x || y != v.y)
	{
		return(true);
	}
	return(false);
}







template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::operator=(const Vector2& v)
{
	x = v.x;
	y = v.y;
	
	return(*this);
}







template<typename dataType>
constexpr inline Vector2<dataType> Vector2<dataType>::operator-(const Vector2& v) const
{
	return(Vector2(x - v.x, y - v.y));
}


template<typename dataType>
constexpr inline Vector2<dataType> Vector2<dataType>::operator+(const Vector2& v) const
{
	return(Vector2(x + v.x, y + v.y));
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector2<dataType> Vector2<dataType>::operator*(dataType2 scalar) const
{
	return(Vector2(x * scalar, y * scalar));
}


template<typename dataType>
constexpr inline double Vector2<dataType>::operator*(Vector2 v) const
{
	double scalar_x = ((double) x) * v.x;
	double scalar_y = ((double) y) * v.y;
	return(scalar_x + scalar_y);
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector2<dataType> Vector2<dataType>::operator/(dataType2 scalar) const
{
	return(Vector2(x / scalar, y / scalar));
}







template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::operator+=(const Vector2& v)
{
	*this = *this + v;
	return(*this);
}


template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::operator-=(const Vector2& v)
{
	*this = *this - v;
	return(*this);
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector2<dataType>& Vector2<dataType>::operator*=(dataType2 scalar)
{
	*this = *this * scalar;
	return(*this);
}


template<typename dataType>
constexpr inline Vector2<dataType>& Vector2<dataType>::operator*=(Vector2 v)
{
	*this = *this * v;
	return(*this);
}


template<typename dataType>
template<typename dataType2>
constexpr inline Vector2<dataType>& Vector2<dataType>::operator/=(dataType2 scalar)
{
	*this = *this / scalar;
	return(*this);
}