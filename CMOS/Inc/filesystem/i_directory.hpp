#pragma once

#include "../defines.hpp"
#include "../array.hpp"
#include "i_file.hpp"
#include "i_entry.hpp"





class I_Directory
{
	public:
		
		
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual ~I_Directory()																					= default;
		
		virtual bool is_valid() const																		= 0;
		virtual const String& get_name() const													= 0;
		virtual const Array<I_Entry*> list() const											= 0;
		virtual uint32 get_numberOfEntries() const											= 0;
		virtual bool is_rootDirectory() const														= 0;
		virtual const String& get_path() const													= 0;
		virtual const I_Directory* get_parent() const										= 0;
		
		virtual const I_Directory* read(String name) const							= 0;
		virtual const I_File* read(String name, String extension) const	= 0;
		
		virtual feedback create(const I_File& file)											= 0;
		virtual feedback create(const I_Directory& directory)						= 0;
};