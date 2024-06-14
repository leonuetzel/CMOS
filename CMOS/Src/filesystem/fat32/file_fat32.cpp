#include "../../../Inc/filesystem/fat32/file_fat32.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CODE_RAM File_FAT32::File_FAT32(const I_Drive& drive, I_DMA& dma, const Bootsector_FAT32& bootsector, const Entry_FAT32& self)
	:	m_drive(drive),
		m_dma(dma),
		m_changeStamp(bootsector.get_changeStamp()),
		m_bootsector(bootsector),
		m_self(self)
{
	
}


CODE_RAM File_FAT32::~File_FAT32()
{
	
}







CODE_RAM bool File_FAT32::is_valid() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(false);
	}
	return(true);
}


CODE_RAM String File_FAT32::get_nameWithExtension() const
{
	return(m_self.get_nameWithExtension());
}


CODE_RAM const String& File_FAT32::get_name() const
{
	return(m_self.m_name);
}


CODE_RAM const String& File_FAT32::get_extension() const
{
	return(m_self.m_extension);
}


CODE_RAM uint32 File_FAT32::get_sizeInBytes() const
{
	return(m_self.m_sizeInBytes);
}







CODE_RAM String* File_FAT32::read() const
{
	return(read(0, m_self.m_sizeInBytes));
}


CODE_RAM String* File_FAT32::read(uint32 position) const
{
	return(read(position, m_self.m_sizeInBytes - position));
}


CODE_RAM String* File_FAT32::read(uint32 position, uint32 length) const
{
	if(is_valid() == false)
	{
		return(nullptr);
	}
	
	String* result = new String;
	if(m_self.m_sizeInBytes <= position)
	{
		return(result);
	}
	
	if(position + length > m_self.m_sizeInBytes)
	{
		length = m_self.m_sizeInBytes - position;
	}
	
	uint32 bytesPerSector = m_bootsector.get_bytesPerSector();
	uint32 sectorsPerCluster = m_bootsector.get_sectorsPerCluster();
	uint32 numberOfSectors = length / bytesPerSector;
	if(length % bytesPerSector != 0)
	{
		numberOfSectors++;
	}
	
	
	
	if(result->set_size(numberOfSectors * bytesPerSector, false) == OK)
	{
		uint8* data = (uint8*) result->get_string();
		
		
		
		uint32 startSector = position / bytesPerSector;
		uint32 clusterNumber = m_bootsector.followClusterChain(m_self.m_clusterNumber, startSector / sectorsPerCluster);
		uint32 sectorNumber = m_bootsector.clusterToLBA(clusterNumber);
		for(uint32 i = 0; i < numberOfSectors; i++)
		{
			if(i % sectorsPerCluster == 0 && i != 0)
			{
				clusterNumber = m_bootsector.followClusterChain(clusterNumber, 1);
				sectorNumber = m_bootsector.clusterToLBA(clusterNumber);
			}
			m_drive.read(sectorNumber + ((i + startSector) % sectorsPerCluster), 1, data + i * bytesPerSector);
		}
		result->erase(0, position % bytesPerSector);
		result->erase(length);
	}
	
	return(result);
}






		
CODE_RAM feedback File_FAT32::write(const String& data) const
{
	return(FAIL);
}


CODE_RAM feedback File_FAT32::write(const String& data, uint32 position) const
{
	return(FAIL);
}