#pragma once

#include "../defines.hpp"
#include "../string.hpp"
#include "../time.hpp"





class I_Entry
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual ~I_Entry()														= default;
		
		virtual String get_nameWithExtension() const	= 0;
		virtual const String& get_name() const				= 0;
		virtual const String& get_extension() const		= 0;
		virtual bool is_readOnly() const							= 0;
		virtual bool is_directory() const							= 0;
		virtual const Time& get_creationTime() const	= 0;
		virtual uint32 get_clusterNumber() const			= 0;
		virtual uint32 get_sizeInBytes() const				= 0;
};