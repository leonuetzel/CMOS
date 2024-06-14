#pragma once

#include "../../cmos.hpp"
#include "volume_fat32.hpp"





class MasterBootRecord
{
	public:
		
		static constexpr uint32 c_partition_number = 3;
		
		
		enum class e_OSType
		{
			EMPTY											= 0x00,
			FAT12											= 0x01,
			XENIX_1										= 0x02,
			XENIX_2										= 0x03,
			FAT16_MAX32MB							= 0x04,
			EXTDOSPART_MAX2GB					= 0x05,
			FAT16_MAX2GB							= 0x06,
			HPFS_NTFS									= 0x07,
			AIX												= 0x08,
			AIX_BOOTABLE							= 0x09,
			OS2_BOOTMANAGER						= 0x0A,
			FAT32_CHS									= 0x0B,
			FAT32_LBA									= 0x0C,
			FAT16_LBA									= 0x0E,
			EXT_PART_MIN1024ZYL				= 0x0F,
			OPUS											= 0x10,
			HIDDEN_FAT12							= 0x11,
			COMPAQ_DIAGNOST						= 0x12,
			HIDDEN_FAT16_MAX32MB			= 0x14,
			HIDDEN_FAT16 							= 0x16,
			HIDDEN_HPFS_NTFS					= 0x17,
			AST_WINDOWS_SWAP					= 0x18,
			HIDDEN_WIN95_FAT32				= 0x1B,
			HIDDEN_WIN95_FAT32_LBA		= 0x1C,
			HIDDEN_WIN95_FAT16_LBA		= 0x1E,
			NEC_DOS										= 0x24,
			PLAN_9										= 0x39,
			PARTITION_MAGIC						= 0x3C,
			VENIX_80286								= 0x40,
			PPC_PREP_BOOT							= 0x41,
			SFS 											= 0x42,
			QNX4DOTX									= 0x4D,
			QNX4DOTX_2ND_PARTITION		= 0x4E,
			QNX4DOTX_3RD_PARTITION		= 0x4F,
			ONTRACK_DM								= 0x50,
			ONTRACK_DM6_AUX_1					= 0x51,
			CP_M											= 0x52,
			ONTRACK_DM6_AUX_2					= 0x53,
			ONTRACK_DM6								= 0x54,
			EZ_DRIVE									= 0x55,
			GOLDEN_BOW								= 0x56,
			PRIAM_EDISK								= 0x5C,
			SPEED_STOR_1							= 0x61,
			GNU_HURD_OR_SYS						= 0x63,
			NOVELL_NETWARE						= 0x64,
			NOVELL 										= 0x65,
			DISK_SECURE_MULT					= 0x70,
			UNIX_PC_IX								= 0x75,
			AKTIV_OLD_MINIX						= 0x80,
			BOOTEN_VON_LAUFWERK_D			= 0x81,
			LINUX_SWAP								= 0x82,
			LINUX_NATIVE							= 0x83,
			OS2_HIDDEN_C							= 0x84,
			LINUX_EXTENDED						= 0x85,
			NTFS_VOLUME_SET_1					= 0x86,
			NTFS_VOLUME_SET_2					= 0x87,
			LINUX_LVM									= 0x8E,
			AMOEBLA										= 0x93,
			AMOEBLA_BBT								= 0x94,
			BSD_OS										= 0x9F,
			IBM_THINKPAD_HIDDEN				= 0xA0,
			BSD_386										= 0xA5,
			OPEN_BSD									= 0xA6,
			NEXT_STEP									= 0xA7,
			BSDI_FS										= 0xB7,
			BSDI_SWAP									= 0xB8,
			DRDOS_SEC_FAT32						= 0xC1,
			DRDOS_SEC_FAT32_LBA				= 0xC4,
			DRDOS_SEC_FAT16_LBA				= 0xC6,
			SYRINX										= 0xC7,
			NON_FS_DATA								= 0xDA,
			CONCURRENT_DOS_CPM_CTOS		= 0xDB,
			DELL_UTILITY							= 0xDE,
			DOS_ACCESS								= 0xE1,
			DOS_RO										= 0xE3,
			SPEED_STOR_2							= 0xE4,
			BEOS_FS										= 0xEB,
			EFI_GPT										= 0xEE,
			EFI_FAT12_FAT16_FAT32			= 0xEF,
			SPEED_STOR_3							= 0xF1,
			DOS_SECONDARY							= 0xF2,
			SPEED_STOR_4							= 0xF4,
			LINUX_RAID_AUTO						= 0xFD,
			LAN_STEP									= 0xFE,
			BBT 											= 0xFF
		};
		
		
		
		typedef struct
		{
			bool valid;
			
			bool bootable;
			e_OSType OS_type;
			
			uint8 startHead;
			uint8 startSector;
			uint16 startCylinder;
			
			
			uint8 endHead;
			uint8 endSector;
			uint16 endCylinder;
			
			uint32 dataStartSector_LBA;
			uint32 dataSize_inSectors;
			
			I_Volume* partition;
		}s_partition;
		
		
		
		
		
	private:
		
		I_Drive& m_drive;
		I_DMA& m_dma;
		const uint32 m_changeStamp;
		
		uint8 m_bootCode[446];
		s_partition m_partition[c_partition_number];
		
		
		/*
		uint32 fat_reserveCluster(uint32 numberOfClusters);
		uint32* create_entry(Entry& entry);
		*/
		
		
		
		
	public:
		
		MasterBootRecord(I_Drive& drive, I_DMA& dma);
		~MasterBootRecord();
		
		bool is_valid() const;
		const s_partition& get_volume(uint32 number) const;
		
		/*
		feedback create_directory(String path);
		String& file_load(Entry& file);
		String& file_load(Entry& file, uint32 clusterNumber);*/
};