#pragma once

#include "defines.hpp"
#include "buffer.hpp"





template<typename dataType>
class I_Ringbuffer
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual ~I_Ringbuffer() = default;
		
		virtual feedback write(const dataType& data) = 0;
		virtual dataType read() = 0;
		virtual void clear() = 0;
		
		virtual bool is_dataAvailable() const = 0;
		virtual uint32 get_numberOfUnread() const = 0;
		virtual Buffer::e_state get_state() const = 0;
		virtual uint32 get_size() const = 0;
		virtual bool is_valid() const = 0;
		
		virtual bool contains(const dataType& data) const = 0;
};