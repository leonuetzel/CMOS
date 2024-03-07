#pragma once

#include "../../cmos.hpp"
#include "bootsector_fat32.hpp"
#include "file_fat32.hpp"
#include "entry_fat32.hpp"





class Directory_FAT32: public I_Directory
{
	public:
		
		
		
		
		
		
		
	private:
		
		const I_Drive& m_drive;
		I_DMA& m_dma;
		const uint32 m_changeStamp;
		const Bootsector_FAT32& m_bootsector;
		Entry_FAT32 m_self;
		String m_path;
		bool m_rootDirectory;
		
		Array<Entry_FAT32*> m_entries;
		
		void extractDirectory(uint8* directoryData);
		
		
		
		
		
	public:
		
		Directory_FAT32(const I_Drive& drive, I_DMA& dma, const Bootsector_FAT32& bootsector, String path, Entry_FAT32 self);
		inline ~Directory_FAT32()																override;
		
		bool is_valid() const																		override;
		inline const String& get_name() const										override;
		inline const Array<I_Entry*> list() const								override;
		inline uint32 get_numberOfEntries() const								override;
		inline bool is_rootDirectory() const										override;
		inline const String& get_path() const										override;
		inline const I_Directory* get_parent() const						override;
		
		const I_Directory* read(String name) const							override;
		const I_File* read(String name, String extension) const	override;
		
		feedback create(const I_File& file)											override;
		feedback create(const I_Directory& directory)						override;
		
		
		
		/*
		Entry& find(String name, String extension);
		String& load(Entry& file);
		String& load(Entry& file, uint32 clusterNumber);*/
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

inline Directory_FAT32::~Directory_FAT32()
{
	const uint32 numberOfEntries = m_entries.get_size();
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		delete m_entries[i];
	}
}







inline bool Directory_FAT32::is_valid() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(false);
	}
	return(true);
}


inline const String& Directory_FAT32::get_name() const
{
	return(m_self.m_name);
}


inline const Array<I_Entry*> Directory_FAT32::list() const
{
	const uint32 numberOfEntries = m_entries.get_size();
	Array<I_Entry*> entries(nullptr, numberOfEntries);
	
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		entries[i] = m_entries[i];
	}
	return(entries);
}


inline uint32 Directory_FAT32::get_numberOfEntries() const
{
	return(m_entries.get_size());
}


inline bool Directory_FAT32::is_rootDirectory() const
{
	return(m_rootDirectory);
}


inline const String& Directory_FAT32::get_path() const
{
	return(m_path);
}


inline const I_Directory* Directory_FAT32::get_parent() const
{
	return(read(".."));
}