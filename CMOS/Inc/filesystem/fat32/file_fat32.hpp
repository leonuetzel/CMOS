#pragma once

#include "../../cmos.hpp"
#include "bootsector_fat32.hpp"
#include "entry_fat32.hpp"





class File_FAT32: public I_File
{
	public:
		
		
		
		
		
		
		
	private:
		
		const I_Drive& m_drive;
		I_DMA& m_dma;
		const uint32 m_changeStamp;
		const Bootsector_FAT32& m_bootsector;
		const Entry_FAT32 m_self;
		
		
		
		
		
	public:
		
		File_FAT32(const I_Drive& drive, I_DMA& dma, const Bootsector_FAT32& bootsector, const Entry_FAT32& self);
		~File_FAT32()																							override;
		
		bool is_valid() const																			override;
		String get_nameWithExtension() const											override;
		const String& get_name() const														override;
		uint32 get_sizeInBytes() const														override;
		const String& get_extension() const												override;
		
		String* read() const																			override;
		String* read(uint32 position) const												override;
		String* read(uint32 position, uint32 length) const				override;
		
		feedback write(const String& data) const									override;
		feedback write(const String& data, uint32 position) const	override;
};