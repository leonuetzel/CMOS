#pragma once

#include "../../cmos.hpp"





class Entry_FAT32: public I_Entry
{
	public:
		
		
		
		
		
		
		
	private:
		
		String m_name;
		String m_extension;
		uint8 m_attribute;
		bool m_readOnly;
		bool m_hidden;
		bool m_system;
		bool m_volume_ID;
		bool m_directory;
		bool m_archive;
		Time m_time_creation;
		Time m_time_access;
		Time m_time_update;
		uint32 m_clusterNumber;
		uint32 m_sizeInBytes;
		
		friend class Volume_FAT32;
		friend class Directory_FAT32;
		friend class File_FAT32;
		
		
		
		
		
	public:
		
		Entry_FAT32();
		~Entry_FAT32()												override;
		
		String get_nameWithExtension() const	override;
		const String& get_name() const				override;
		const String& get_extension() const		override;
		bool is_readOnly() const							override;
		bool is_directory() const							override;
		const Time& get_creationTime() const	override;
		uint32 get_clusterNumber() const			override;
		uint32 get_sizeInBytes() const				override;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

