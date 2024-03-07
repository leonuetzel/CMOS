#pragma once

#include "ringbufferExternal.hpp"





template<typename dataType>
class I_Serial : protected RingbufferExternal<dataType>
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	protected:
		
		constexpr inline I_Serial();
		inline ~I_Serial();
		
		
		
		
		
	public:
		
		virtual dataType rx() = 0;
		virtual feedback tx() = 0;
		virtual void clear() = 0;
		
		virtual bool is_dataAvailable() const = 0;
		virtual uint32 get_numberOfUnread() const = 0;
		virtual Buffer::e_state get_state() const = 0;
		virtual bool contains(const dataType& data) const = 0;
		
		virtual I_Serial& operator<<(char data) = 0;
		virtual I_Serial& operator<<(uint8 data) = 0;
		virtual I_Serial& operator<<(uint16 data) = 0;
		virtual I_Serial& operator<<(uint32 data) = 0;
		virtual I_Serial& operator<<(int8 data) = 0;
		virtual I_Serial& operator<<(int16 data) = 0;
		virtual I_Serial& operator<<(int32 data) = 0;
		virtual I_Serial& operator<<(float data) = 0;
		virtual I_Serial& operator<<(double data) = 0;
		virtual I_Serial& operator<<(const Array<dataType>& data) = 0;
		virtual I_Serial& operator<<(const String& data) = 0;
		
		virtual I_Serial& operator>>(char& data) = 0;
		virtual I_Serial& operator>>(uint8& data) = 0;
		virtual I_Serial& operator>>(uint16& data) = 0;
		virtual I_Serial& operator>>(uint32& data) = 0;
		virtual I_Serial& operator>>(int8& data) = 0;
		virtual I_Serial& operator>>(int16& data) = 0;
		virtual I_Serial& operator>>(int32& data) = 0;
		virtual I_Serial& operator>>(float& data) = 0;
		virtual I_Serial& operator>>(double& data) = 0;
		virtual I_Serial& operator>>(String& data) = 0;
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

template<typename dataType>
constexpr inline I_Serial<dataType>::I_Serial()
	:	RingbufferExternal<dataType>(nullptr, 0)
{
	
}


template<typename dataType>
inline I_Serial<dataType>::~I_Serial()
{
	
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

