#pragma once

#include "defines.hpp"
#include "pair.hpp"





template<typename dataType_1, typename dataType_2, typename dataType_3>
class Triplet : public Pair<dataType_1, dataType_2>
{
	public:
		
		
		
		
		
		
		
	private:
		
		dataType_3 m_third;
		
		
		
		
		
	public:
		
		constexpr inline Triplet();
		constexpr inline Triplet(const dataType_1& first, const dataType_2& second, const dataType_3& third);
		constexpr inline Triplet(const Triplet& array);
		inline ~Triplet();
		
		
		//	Getter
		constexpr inline dataType_3& third();
		constexpr inline dataType_3 third() const;
		
		
		//	Operators
		inline Triplet& operator=(const Triplet<dataType_1, dataType_2, dataType_3>& triplet);
		inline bool operator==(const Triplet<dataType_1, dataType_2, dataType_3>& triplet) const;
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

template<typename dataType_1, typename dataType_2, typename dataType_3>
constexpr inline Triplet<dataType_1, dataType_2, dataType_3>::Triplet()
	:	Pair<dataType_1, dataType_2>(),
		m_third(dataType_3())
{
	
}


template<typename dataType_1, typename dataType_2, typename dataType_3>
constexpr inline Triplet<dataType_1, dataType_2, dataType_3>::Triplet(const dataType_1& first, const dataType_2& second, const dataType_3& third)
	:	Pair<dataType_1, dataType_2>(first, second),
		m_third(third)
{
	
}


template<typename dataType_1, typename dataType_2, typename dataType_3>
constexpr inline Triplet<dataType_1, dataType_2, dataType_3>::Triplet(const Triplet<dataType_1, dataType_2, dataType_3>& triplet)
	:	Triplet()
{
	operator=(triplet);
}


template<typename dataType_1, typename dataType_2, typename dataType_3>
inline Triplet<dataType_1, dataType_2, dataType_3>::~Triplet()
{
	
}







template<typename dataType_1, typename dataType_2, typename dataType_3>
constexpr inline dataType_3& Triplet<dataType_1, dataType_2, dataType_3>::third()
{
	return(m_third);
}


template<typename dataType_1, typename dataType_2, typename dataType_3>
constexpr inline dataType_3 Triplet<dataType_1, dataType_2, dataType_3>::third() const
{
	return(m_third);
}







template<typename dataType_1, typename dataType_2, typename dataType_3>
inline Triplet<dataType_1, dataType_2, dataType_3>& Triplet<dataType_1, dataType_2, dataType_3>::operator=(const Triplet& triplet)
{
	Pair<dataType_1, dataType_2>::operator=(triplet);
	m_third = triplet.m_third;
	return(*this);
}


template<typename dataType_1, typename dataType_2, typename dataType_3>
inline bool Triplet<dataType_1, dataType_2, dataType_3>::operator==(const Triplet& triplet) const
{
	if(m_third != triplet.m_third)
	{
		return(false);
	}
	return(Pair<dataType_1, dataType_2>::operator==(triplet));
}