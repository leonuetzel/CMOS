#pragma once

#include "../cmos.hpp"





class I_PhysicalLayer
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual ~I_PhysicalLayer()													= default;
		
		virtual void tx(const Array<uint8>& packet)					= 0;
		virtual const Array<uint8>& get_macAddress()				= 0;
		virtual uint32 get_maximumTransmissionUnit() const	= 0;
};