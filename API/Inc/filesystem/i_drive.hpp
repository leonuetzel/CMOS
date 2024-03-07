#pragma once

#include "../defines.hpp"





class I_Drive
{
	public:
		
		
		
		
		
		
	private:
		
		
		
		
		
		
	public:
		
		virtual feedback enable()																																= 0;
		virtual feedback disable()																															= 0;
		virtual bool detect() const																															= 0;
		virtual uint32 get_changeStamp() const																									= 0;
		virtual uint8* read(uint32 startSector, uint32 numberOfSectors) const										= 0;
		virtual feedback read(uint32 startSector, uint32 numberOfSectors, uint8* target) const	= 0;
		virtual feedback write(uint32 startSector, uint32 numberOfSectors, uint8* data) const		= 0;
};