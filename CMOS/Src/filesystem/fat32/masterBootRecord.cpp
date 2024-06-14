#include "../../../Inc/filesystem/fat32/masterBootRecord.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/
/*
uint32 MasterBootRecord::fat_reserveCluster(uint32 numberOfClusters)
{
	if(numberOfClusters == 0)
	{
		return(0);
	}
	
	uint32 chain_index = 0;
	uint32* chain = new uint32[numberOfClusters];
	
	uint32 *fat_table = (uint32*) NULL;
	
	for(uint32 i = 0; i < m_bootSector.sectorsOccupiedByOneFAT; i++)																														//	Search free Clusters and save the numbers to chain-Array
	{
		fat_table = (uint32*) m_drive.read(m_FAT_startSector + i);
		for(uint32 j = 0; j < m_bootSector.bytesPerSector / 4; j++)
		{
			if(fat_table[j] == 0)
			{
				chain[chain_index] = i * m_bootSector.bytesPerSector / 4 + j;
				chain_index++;
				if(chain_index == numberOfClusters)
				{
					break;
				}
			}
		}
		if(chain_index == numberOfClusters)
		{
			break;
		}
	}
	
	
	
	for(uint32 i = 0; i < chain_index; i++)
	{
		fat_table = (uint32*) m_drive.read(m_FAT_startSector + chain[i] / 128);
		
		uint32 value = 0;
		if(i == chain_index - 1)
		{
			value = 0x0FFFFFFF;
		}
		else
		{
			value = chain[i + 1];
		}
		fat_table[chain[i] % 128] = value;
		m_write(m_FAT_startSector + chain[i] / 128, (uint8*) fat_table);
	}
	
	uint32 returnValue = chain[0];
	delete[] chain;
	return(returnValue);
}


uint32* MasterBootRecord::create_entry(Entry& entry)
{
	uint32* data = new uint32[32];
}
*/


/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

MasterBootRecord::MasterBootRecord(I_Drive& drive, I_DMA& dma)
	:	m_drive(drive),
		m_dma(dma),
		m_changeStamp(drive.get_changeStamp())
{
	if(m_drive.enable() != OK || is_valid() == false)
	{
		return;
	}
	
	
	//	Read MBR from Sector 0
	uint8 *sector = m_drive.read(0, 1);
	
	
	//	Check Signature
	uint16 signature = bit::pack::_2x8Bit_to_1x16Bit(&sector[510], false);
	if(signature != 0xAA55)
	{
		for(uint32 i = 0; i < c_partition_number; i++)
		{
			m_partition[i].valid = false;
		}
		delete[] sector;
		m_drive.disable();
		return;
	}
	
	/*
	Cache& cache = System::get().get_cache();																																										//	Cache Maintenance
	Thread& self = System::get().get_thread();
	self.set_priviledgeLevel(Thread::e_priviledgeLevel::ADMIN);
	{
		cache.DCache_invalidate((uint32*) m_bootCode, sizeof(m_bootCode));
		cache.DCache_clean((uint32*) sector, sizeof(m_bootCode));
	}
	self.set_priviledgeLevel(Thread::e_priviledgeLevel::USER);
	*/
	
	
	//	Copy Boot Code
	m_dma.transfer(sector, m_bootCode, I_DMA::e_dataType::UINT8, I_DMA::e_dataType::UINT8, true, true, I_DMA::e_direction::MEMORY_TO_MEMORY, false, sizeof(m_bootCode));
	
	
	
	for(uint32 i = 0; i < c_partition_number; i++)																																						//	Initialize Partitions
	{
		uint8* data = &sector[i * 16 + 446];
		m_partition[i].valid = false;
		
		
		
		
		
		if(data[0] == 0x80)
		{
			m_partition[i].bootable = true;
		}
		else
		{
			m_partition[i].bootable = false;
		}
		
		m_partition[i].startHead = data[1];
		m_partition[i].startSector = data[2] & 0x3F;
		m_partition[i].startCylinder = ((data[2] & 0xC0) << 8) | data[3];
		
		m_partition[i].OS_type = (e_OSType) data[4];
		
		m_partition[i].endHead = data[5];
		m_partition[i].endSector = data[6] & 0x3F;
		m_partition[i].endCylinder = ((data[6] & 0xC) << 8) | data[7];
		
		m_partition[i].dataStartSector_LBA = bit::pack::_4x8Bit_to_1x32Bit(&data[8], false);
		m_partition[i].dataSize_inSectors = bit::pack::_4x8Bit_to_1x32Bit(&data[12], false);
		
		
		
		switch(m_partition[i].OS_type)
		{
			case e_OSType::FAT32_LBA:
				m_partition[i].partition = new Volume_FAT32(m_drive, m_dma, m_changeStamp, m_partition[i].dataStartSector_LBA);
				break;
				
			default:
				break;
		}
		
		
		
		if(m_partition[i].dataStartSector_LBA > 0 && m_partition[i].partition != nullptr)
		{
			m_partition[i].valid = true;
		}
	}
	delete[] sector;
}


MasterBootRecord::~MasterBootRecord()
{
	m_drive.disable();
	for(uint32 i = 0; i < c_partition_number; i++)
	{
		if(m_partition[i].partition != nullptr)
		{
			delete m_partition[i].partition;
		}
	}
}







bool MasterBootRecord::is_valid() const
{
	if((m_changeStamp != m_drive.get_changeStamp()) || (m_drive.detect() != true))
	{
		return(false);
	}
	return(true);
}


const MasterBootRecord::s_partition& MasterBootRecord::get_volume(uint32 number) const
{
	if(number < c_partition_number)
	{
		return(m_partition[number]);
	}
	return((s_partition&) *((s_partition*) nullptr));
}







/*
feedback MasterBootRecord::create_directory(String path)
{
	s_element* directory = read_rootDirectory();
	
	uint32 numberOfSubdirectories = path.count_sign('/');
	if(numberOfSubdirectories == 0)
	{
		return(OK);
	}
	numberOfSubdirectories--;
	for(uint32 i = 0; i < numberOfSubdirectories; i++)
	{
		String subdirectory = path.substring('/', i + 1);
		bool found = false;
		for(uint32 j = 0; directory[j].name.get_length() != 0; j++)
		{
			if(subdirectory.compare(directory[j].name) == true)
			{
				s_element temp = directory[j];
				delete[] directory;
				directory = read_directory(&temp);
				found = true;
				break;
			}
		}
		if(found == false)
		{
			break;
		}
	}
	
	
	
	uint32 freeCluster = fat_reserveCluster(1);
	
	// Create Subdirectory in directory
	*/
	/*
		Freien Cluster für Subdirectorydaten suchen und in FAT reservieren
		Clusterdaten (.. und .) erstellen und schreiben
		Prototyp-Entry des zu erstellen Entrys erstellen
		Sector mit Directory Entries laden
		Schauen ob in Cluster Platz frei
			Ja:		Prototyp-Entry anfügen
			Nein:	Freien Cluster suchen und in FAT reservieren
						FAT-Wert des Parentdirectorys mit Adresse des reservierten Clusters beschreiben
						FAT-Wert des reservierten Clusters mit 0x0FFFFFF8 (End of File) beschreiben
						Prototyp-Entry in reservierten Cluster schreiben
	*//*
	return(OK);
}








String& MasterBootRecord::file_load(Entry& file)
{
	uint32 bytesPerCluster = m_bootSector.bytesPerSector * m_bootSector.sectorsPerCluster;
	uint32 numberOfClusters = file.fileSize_byte / bytesPerCluster;
	numberOfClusters++;
	
	String* data = new String;
	if(data->expand(file.fileSize_byte) == FAIL)
	{
		return((String*) NULL);
	}
	
	
	
	uint8* data_raw = (uint8*) data->get_string();
	uint32 clusterNumber = 0;
	for(uint32 i = 0; i < numberOfClusters; i++)
	{
		uint32 clusterNumber_absolut = fat_followClusterChain(file.clusterNumber, clusterNumber);
		if(clusterNumber_absolut <= 2)
		{
			delete data;
			return((String*) NULL);
		}
		
		for(uint32 j = 0; j < m_bootSector.sectorsPerCluster; j++)
		{
			uint8* sector = m_drive.read(cluster_to_LBA(clusterNumber_absolut) + j);
			
			uint32 writtenBytes = i * bytesPerCluster - j * m_bootSector.bytesPerSector;
			if(file.fileSize_byte - writtenBytes < m_bootSector.bytesPerSector)
			{
				m_thread.mdma_copy(sector, data_raw + writtenBytes, file.fileSize_byte - writtenBytes);
				break;
			}
			m_thread.mdma_copy(sector, data_raw + writtenBytes, m_bootSector.bytesPerSector);
		}
	}
	
	data->set_length(file.fileSize_byte);
	return(data);
}


String& MasterBootRecord::file_load(Entry& file, uint32 clusterNumber)
{
	String* data = new String;
	uint32 clusterNumber_absolut = fat_followClusterChain(file.clusterNumber, clusterNumber);
	if(clusterNumber_absolut <= 2)
	{
		return(data);
	}
	
	
	uint32 bytesPerSector = m_bootSector.bytesPerSector;
	uint32 sectorsPerCluster = m_bootSector.sectorsPerCluster;
	data->expand(bytesPerSector * sectorsPerCluster);
	uint8* data_raw = (uint8*) data->get_string();
	
	for(uint32 i = 0; i < sectorsPerCluster; i++)
	{
		uint8* sector = m_drive.read(cluster_to_LBA(clusterNumber_absolut) + i);
		m_thread.mdma_copy(sector, data_raw + i * bytesPerSector, bytesPerSector);
	}
	data->set_length(bytesPerSector * sectorsPerCluster);
	return(data);
}*/