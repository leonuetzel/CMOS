#pragma once

#include "../defines.hpp"
#include "i_directory.hpp"





class I_Volume
{
	public:
		
		static constexpr char c_label_invalid = '\0';
		
		
		
		
		
	private:
		
		
		
		
		
		
		
	public:
		
		virtual ~I_Volume()																	= default;
		
		virtual bool is_valid() const												= 0;
		virtual const I_Directory* read() const							= 0;
		virtual const I_Directory* read(String path) const	= 0;
		virtual feedback set_label(char label)							= 0;
		virtual char get_label() const											= 0;
};