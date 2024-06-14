#pragma once

#include "../../cmos.hpp"
#include "bootsector_fat32.hpp"
#include "directory_fat32.hpp"
#include "entry_fat32.hpp"





class Volume_FAT32: public I_Volume
{
	public:
		
		
		
		
		
		
		
	private:
		
		const I_Drive& m_drive;
		I_DMA& m_dma;
		
		char m_label;
		Bootsector_FAT32 m_bootsector;
		
		
		
		
		
	public:
		
		Volume_FAT32(const I_Drive& drive, I_DMA& dma, const uint32& changeStamp, uint32 startSector_LBA);
		~Volume_FAT32()															override;
		
		bool is_valid() const												override;
		const I_Directory* read() const							override;
		const I_Directory* read(String path) const	override;
		feedback set_label(char label)							override;
		char get_label() const											override;
};