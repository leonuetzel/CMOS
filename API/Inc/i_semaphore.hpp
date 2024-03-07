#pragma once

#include "defines.hpp"





class I_Semaphore
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
	public:
		
		virtual feedback lock()					= 0;
		virtual feedback unlock()				= 0;
		virtual feedback force_unlock()	= 0;
		virtual uint8 get_owner() const	= 0;
};