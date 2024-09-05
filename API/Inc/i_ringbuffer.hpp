#pragma once

#include "defines.hpp"





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
		virtual void reset() = 0;
		
		virtual uint32 get_numberOfUnread() const = 0;
		virtual bool is_empty() const = 0;
		virtual bool is_full() const = 0;
		virtual uint32 get_size() const = 0;
		virtual bool is_valid() const = 0;
		
		virtual bool contains(const dataType& data) const = 0;
};