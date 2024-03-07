#pragma once

#include "../cmos.hpp"
#include "fat32/masterBootRecord.hpp"





class Filesystem
{
	public:
		
		
		
		
		
		
		
	private:
		
		static constexpr uint32 c_volume_number = 26;
		
		I_Volume* m_volumes[c_volume_number];
		
		
		constexpr inline Filesystem();
		Filesystem(const Filesystem& filesystem) = delete;
		inline ~Filesystem();
		
		
		
		
		
	public:
		
		inline static Filesystem& get();
		
		feedback attach(I_Volume& volume);
		feedback detach(I_Volume& volume);
		
		const I_Directory* read(String path);
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

constexpr inline Filesystem::Filesystem()
	:	m_volumes()
{
	
}


inline Filesystem::~Filesystem()
{
	
}



/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

inline Filesystem& Filesystem::get()
{
	static Filesystem filesystem;
	return(filesystem);
}