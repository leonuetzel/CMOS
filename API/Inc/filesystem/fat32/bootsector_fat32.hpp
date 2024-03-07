#pragma once

#include "../../cmos.hpp"





class Bootsector_FAT32
{
	public:
		
		
		
		
		
		
		
	private:
		
		const I_Drive& m_drive;
		I_DMA& m_dma;
		const uint32 m_changeStamp;
		
		bool m_valid;
		uint32 m_partitionStartSector;
		uint32 m_FAT_startSector;
		uint32 m_partitionDataStartSector;
		
		uint8 m_jumpToBootcode[3];
		String m_OEM_name;
		uint16 m_bytesPerSector;
		uint8 m_sectorsPerCluster;
		uint16 m_numberOfReservedSectors;
		uint8 m_numberOfFATs;
		uint16 m_numberOfSectorsPerTrack;
		uint16 m_numberOfReadWriteHeads;
		uint32 m_numberOfHiddenSectors;
		uint32 m_numberOfTotalSectors;
		uint32 m_numberOfSectorsPerFAT;
		uint32 m_clusterNumberOfRootDirectory;
		uint16 m_positionOfInfoSector_relativToFirstDataSector;
		bool m_infoSector;
		uint16 m_backupBootSector;
		uint8 m_physicalDiskNumber;
		uint8 m_signature;
		uint32 m_serialNumber;
		String m_driveName;
		String m_systemID;
		uint8 m_bootProgram[420];
		
		
		
		
		
	public:
		
		Bootsector_FAT32(const I_Drive& drive, I_DMA& dma, const uint32& changeStamp);
		~Bootsector_FAT32();
		
		feedback init(const uint8* bootSectorData, uint32 partitionStartSector);
		bool is_valid() const;
		const uint32 get_changeStamp() const;
		
		uint32 clusterToLBA(uint32 clusterNumber) const;
		uint32 followClusterChain(uint32 startCluster, uint32 searchedClusterNumber) const;
		
		uint32 get_sectorsPerCluster() const;
		uint32 get_bytesPerSector() const;
		uint32 get_FAT_startSector() const;
		uint32 get_clusterNumberOfRootDirectory() const;
};