#pragma once

#include "defines.hpp"
#include "mathBasic.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "rectangle.hpp"





namespace Math
{
	/*
		< 0: Point is left of		the Line
		= 0: Point is on				the Line
		> 0: Point is right of	the Line
	*/
	template<typename dataType>
	dataType isPointRightOrLeft(Vector2<dataType> A, Vector2<dataType> B, Vector2<dataType> point);
};



template<typename dataType>
dataType Math::isPointRightOrLeft(Vector2<dataType> A, Vector2<dataType> B, Vector2<dataType> point)
{
	dataType temp_1 = B.y - A.y;
	dataType temp_2 = point.x - A.x;
	dataType temp_3 = B.x - A.x;
	dataType temp_4 = point.y - A.y;
	
	double temp_5 = ((double) temp_1) * ((double) temp_2);
	double temp_6 = ((double) temp_3) * ((double) temp_4);
	
	return(temp_5 - temp_6);
}