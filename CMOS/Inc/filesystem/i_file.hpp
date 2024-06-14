#pragma once

#include "../defines.hpp"
#include "../string.hpp"





class I_File
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual ~I_File()																									= default;
		
		virtual bool is_valid() const																			= 0;
		virtual String get_nameWithExtension() const											= 0;
		virtual const String& get_name() const														= 0;
		virtual uint32 get_sizeInBytes() const														= 0;
		virtual const String& get_extension() const												= 0;
		
		virtual String* read() const																			= 0;
		virtual String* read(uint32 position) const												= 0;
		virtual String* read(uint32 position, uint32 length) const				= 0;
		
		virtual feedback write(const String& data) const									= 0;
		virtual feedback write(const String& data, uint32 position) const	= 0;
};