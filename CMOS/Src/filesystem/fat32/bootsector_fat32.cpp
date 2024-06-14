#include "../../../Inc/filesystem/fat32/bootsector_fat32.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Bootsector_FAT32::Bootsector_FAT32(const I_Drive& drive, I_DMA& dma, const uint32& changeStamp)
	:	m_drive(drive),
		m_dma(dma),
		m_changeStamp(changeStamp),
		
		m_valid(false)
{
	
}


Bootsector_FAT32::~Bootsector_FAT32()
{
	
}







feedback Bootsector_FAT32::init(const uint8* bootSectorData, uint32 partitionStartSector)
{
	if(m_valid == true || is_valid() == false)
	{
		return(FAIL);
	}
	
	
	//	Check Signature
	uint16 signature = bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[510], false);
	if(signature != 0xAA55)
	{
		return(FAIL);
	}
	
	
	
	m_partitionStartSector = partitionStartSector;
	
	
	
	for(uint32 i = 0; i < 3; i++)
	{
		m_jumpToBootcode[i] = bootSectorData[i];
	}
	m_OEM_name.erase();
	for(uint32 i = 0; i < 8; i++)
	{
		m_OEM_name += (char) bootSectorData[i + 3];
	}
	m_bytesPerSector																= bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[11], false);
	m_sectorsPerCluster															= bootSectorData[13];
	m_numberOfReservedSectors												= bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[14], false);
	m_numberOfFATs																	= bootSectorData[16];
	m_numberOfSectorsPerTrack												= bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[24], false);
	m_numberOfReadWriteHeads												= bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[26], false);
	m_numberOfHiddenSectors													= bit::pack::_4x8Bit_to_1x32Bit(&bootSectorData[28], false);
	m_numberOfTotalSectors													= bit::pack::_4x8Bit_to_1x32Bit(&bootSectorData[32], false);
	m_numberOfSectorsPerFAT													= bit::pack::_4x8Bit_to_1x32Bit(&bootSectorData[36], false);
	m_clusterNumberOfRootDirectory									= bit::pack::_4x8Bit_to_1x32Bit(&bootSectorData[44], false);
	m_positionOfInfoSector_relativToFirstDataSector	= bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[48], false);
	m_backupBootSector															= bit::pack::_2x8Bit_to_1x16Bit(&bootSectorData[50], false);
	m_physicalDiskNumber														= bootSectorData[64];
	m_signature																			= bootSectorData[66];
	m_serialNumber																	= bit::pack::_4x8Bit_to_1x32Bit(&bootSectorData[67], false);
	m_driveName.erase();
	for(uint32 i = 0; i < 11; i++)
	{
		m_driveName += (char) bootSectorData[i + 71];
	}
	m_systemID.erase();
	for(uint32 i = 0; i < 8; i++)
	{
		m_systemID += (char) bootSectorData[i + 82];
	}
	/*
	Cache& cache = System::get().get_cache();																																											//	Cache Maintenance
	Thread& self = System::get().get_thread();
	self.set_priviledgeLevel(Thread::e_priviledgeLevel::ADMIN);
	{
		cache.DCache_invalidate((uint32*) m_bootProgram, sizeof(m_bootProgram));
		cache.DCache_clean((uint32*) &bootSectorData[90], sizeof(m_bootProgram));
	}
	self.set_priviledgeLevel(Thread::e_priviledgeLevel::USER);
	*/
	
	
	//	Copy Boot Program
	m_dma.transfer(&bootSectorData[90], m_bootProgram, I_DMA::e_dataType::UINT8, I_DMA::e_dataType::UINT8, true, true, I_DMA::e_direction::MEMORY_TO_MEMORY, false, sizeof(m_bootProgram));
	
	
	
	m_FAT_startSector = m_partitionStartSector + m_numberOfReservedSectors;
	m_partitionDataStartSector = m_FAT_startSector + (m_numberOfFATs * m_numberOfSectorsPerFAT);
	
	
	
	m_valid = true;
	return(OK);
}


bool Bootsector_FAT32::is_valid() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(false);
	}
	return(true);
}


const uint32 Bootsector_FAT32::get_changeStamp() const
{
	return(m_changeStamp);
}







uint32 Bootsector_FAT32::clusterToLBA(uint32 clusterNumber) const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(0);
	}
	uint32 clusterNumberNew = clusterNumber - m_clusterNumberOfRootDirectory;
	uint32 sectorNumber = m_partitionDataStartSector + (clusterNumberNew * m_sectorsPerCluster);
	return(sectorNumber);
}


uint32 Bootsector_FAT32::followClusterChain(uint32 startCluster, uint32 searchedClusterNumber) const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(0);
	}
	
	uint32 FAT_startSector = get_FAT_startSector();
	uint32 clusterNumber = startCluster;
	uint32 cluster_info = 0;
	
	for(uint32 i = 0; i < searchedClusterNumber; i++)
	{
		uint32* fat_table = (uint32*) m_drive.read(FAT_startSector + (clusterNumber / (m_bytesPerSector / 4)), 1);
		cluster_info = fat_table[clusterNumber % 128];
		
		if(cluster_info >= 0x0FFFFFF8)																																															//	End of File
		{
			return(0);
		}
		if(cluster_info == 0x0FFFFFF7)																																															//	Cluster defect
		{
			return(1);
		}
		if(cluster_info == 0x00000000)																																															//	Cluster unused
		{
			return(2);
		}
		
		clusterNumber = cluster_info;
		delete[] fat_table;
	}
	
	return(clusterNumber);
}







uint32 Bootsector_FAT32::get_sectorsPerCluster() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(0);
	}
	return(m_sectorsPerCluster);
}


uint32 Bootsector_FAT32::get_bytesPerSector() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(0);
	}
	return(m_bytesPerSector);
}


uint32 Bootsector_FAT32::get_FAT_startSector() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(0);
	}
	return(m_FAT_startSector);
}


uint32 Bootsector_FAT32::get_clusterNumberOfRootDirectory() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(0);
	}
	return(m_clusterNumberOfRootDirectory);
}