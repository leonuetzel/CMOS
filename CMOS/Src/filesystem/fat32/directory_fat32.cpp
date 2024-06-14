#include "../../../Inc/filesystem/fat32/directory_fat32.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Directory_FAT32::extractDirectory(uint8* directoryData)
{
	uint8* data = directoryData;
	const uint32 numberOfEntries = m_entries.get_size();
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		m_entries[i] = new Entry_FAT32();
	}
	
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		Entry_FAT32& entry = *m_entries[i];
		
		
		//	Entry[0] == 0xE5 -> Deleted Entry
		if(data[0] != 0xE5)
		{
			entry.m_attribute = data[11];
			
			//	Entry[11] == 0x0F -> Long File Name (LFN)
			if((entry.m_attribute == 0x0F) && (bit::isSet(data[0], 6)))
			{
				uint32 count = data[0] & 0x3F;
				for(uint32 j = 0; j < count; j++)
				{
					String lfn_part;
					
					for(uint32 k = 0; k < 5; k++)
					{
						lfn_part += (char) data[k * 2 + 1];
					}
					for(uint32 k = 0; k < 6; k++)
					{
						lfn_part += (char) data[k * 2 + 14];
					}
					for(uint32 k = 0; k < 2; k++)
					{
						lfn_part += (char) data[k * 2 + 28];
					}
					
					lfn_part = lfn_part.subWithDelimiter('\0', 0);
					entry.m_name.insert(0, lfn_part);
					data += 32;
				}
			}
			else
			{
				//	Short File Name
				for(uint32 j = 0; j < 8; j++)
				{
					char sign = data[j];
					entry.m_name += sign;
				}
				
				for(uint32 j = 0; j < 3; j++)
				{
					char sign = data[j + 8];
					entry.m_extension += sign;
				}
				
				if(bit::isSet(data[12], 3))
				{
					entry.m_name.lowerCase();
				}
				if(bit::isSet(data[12], 4))
				{
					entry.m_extension.lowerCase();
				}
				
				entry.m_name.erase_trailing(' ');
				entry.m_extension.erase_trailing(' ');
			}
			
			
			//	Get real Attribute (not the one of LFN I_Entry)
			entry.m_attribute = data[11];
			
			
			//	Extract Attributes
			entry.m_readOnly	= bit::isSet(entry.m_attribute, 0);
			entry.m_hidden		= bit::isSet(entry.m_attribute, 1);
			entry.m_system		= bit::isSet(entry.m_attribute, 2);
			entry.m_volume_ID	= bit::isSet(entry.m_attribute, 3);
			entry.m_directory	= bit::isSet(entry.m_attribute, 4);
			entry.m_archive		= bit::isSet(entry.m_attribute, 5);
			
			if(entry.m_directory == true || entry.m_volume_ID == true)
			{
				entry.m_extension.erase();
			}
			
			
			//	Get Time Information
			{
				entry.m_time_creation.second	= (	data[13] / 100)					+ ((data[14] & 0x1F) * 2);
				entry.m_time_creation.minute	= ((data[14] & 0xE0) >> 5)	+ ((data[15] & 0x07) << 3);
				entry.m_time_creation.hour		= (	data[15] & 0xF8) >> 3;
				entry.m_time_creation.day			= 	data[16] & 0x1F;
				entry.m_time_creation.month		= ((data[16] & 0xE0) >> 5)	+ ((data[17] & 0x01) << 3);
				entry.m_time_creation.year		= ((data[17] & 0xFE) >> 1)	+ 1980;
				
				entry.m_time_access.day				= 	data[18] & 0x1F;
				entry.m_time_access.month			= ((data[18] & 0xE0) >> 5)	+ ((data[19] & 0x01) << 3);
				entry.m_time_access.year			= ((data[19] & 0xFE) >> 1)	+ 1980;
				
				entry.m_time_update.second		= (	data[22] & 0x1F) * 2;
				entry.m_time_update.minute		= ((data[22] & 0xE0) >> 5)	+ ((data[23] & 0x07) << 3);
				entry.m_time_update.hour			= (	data[23] & 0xF8) >> 3;
				entry.m_time_update.day				= 	data[24] & 0x1F;
				entry.m_time_update.month			= ((data[24] & 0xE0) >> 5)	+ ((data[25] & 0x01) << 3);
				entry.m_time_update.year			= ((data[25] & 0xFE) >> 1)	+ 1980;
			}
			
			entry.m_clusterNumber = ((bit::pack::_2x8Bit_to_1x16Bit(&data[20], false) << 16) + bit::pack::_2x8Bit_to_1x16Bit(&data[26], false)) & 0x0FFFFFFF;
			if(entry.m_clusterNumber < 2)
			{
				entry.m_clusterNumber = 2;
			}
			entry.m_sizeInBytes = bit::pack::_4x8Bit_to_1x32Bit(&data[28], false);
			
			if(entry.m_name.contains('.') == true && entry.m_directory == false)
			{
				Array nameExpoded = entry.m_name.explode('.');
				entry.m_extension	= nameExpoded[1];
				entry.m_name			= nameExpoded[0];
			}
		}
		else
		{
			i--;
		}
		data += 32;
	}
	
	
	//	Delete all Entries that are hidden or a Volume ID
	for(uint32 i = 0; i < m_entries.get_size(); i++)
	{
		Entry_FAT32& entry = *m_entries[i];
		if(entry.m_hidden == true || entry.m_volume_ID == true)
		{
			delete &entry;
			m_entries.erase(i);
			i--;
		}
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

CODE_RAM Directory_FAT32::Directory_FAT32(const I_Drive& drive, I_DMA& dma, const Bootsector_FAT32& bootsector, String path, Entry_FAT32 self)
	:	m_drive(drive),
		m_dma(dma),
		m_changeStamp(bootsector.get_changeStamp()),
		m_bootsector(bootsector),
		m_self(self),
		m_path(path),
		m_rootDirectory(false)
{
	if(m_drive.detect() == false)
	{
		return;
	}
	if(m_self.m_clusterNumber == 2)
	{
		m_rootDirectory = true;
	}
	uint32 clusterNumber = m_self.m_clusterNumber;
	uint32 sectorsPerCluster = m_bootsector.get_sectorsPerCluster();
	uint32 numberOfTotalSectors = 1;
	
	
	//	Read the Number of Entries in that Directory
	uint8 *sector = m_drive.read(m_bootsector.clusterToLBA(clusterNumber), 1);
	uint32 numberOfEntries = 0;
	for(int32 i = 0; sector[i * 32] != 0x00; i++)
	{
		//	Entry[0]	== 0xE5 -> Deleted Entry
		//	Entry[11]	== 0x0F -> LongFileName
		if(sector[i * 32] != 0xE5 && sector[i * 32 + 11] != 0x0F)
		{
			numberOfEntries++;
		}
		
		if(i == 15)
		{
			if(numberOfTotalSectors % sectorsPerCluster == 0)
			{
				clusterNumber = m_bootsector.followClusterChain(clusterNumber, 1);
			}
			m_drive.read(m_bootsector.clusterToLBA(clusterNumber) + (numberOfTotalSectors % sectorsPerCluster), 1, sector);
			numberOfTotalSectors++;
			i = -1;
		}
	}
	delete[] sector;
	
	
	
	uint32 bytesPerSector = m_bootsector.get_bytesPerSector();
	
	//	Store the whole Directory Information so reload isnt needed + 1 for empty I_Entry
	m_entries.set_size(numberOfEntries);
	uint8* data = new uint8[numberOfTotalSectors * bytesPerSector];
	
	clusterNumber = m_self.m_clusterNumber;
	uint32 sectorNumber = m_bootsector.clusterToLBA(clusterNumber);
	for(uint32 i = 0; i < numberOfTotalSectors; i++)
	{
		if(i % sectorsPerCluster == 0 && i != 0)
		{
			clusterNumber = m_bootsector.followClusterChain(clusterNumber, 1);
			sectorNumber = m_bootsector.clusterToLBA(clusterNumber);
		}
		m_drive.read(sectorNumber + (i % sectorsPerCluster), 1, data + i * bytesPerSector);
	}
	
	
	
	extractDirectory(data);
	delete[] data;
}







CODE_RAM const I_Directory* Directory_FAT32::read(String name) const
{
	//	Check for valid Directory
	if(is_valid() == false)
	{
		return(nullptr);
	}
	
	
	//	Cant go a Level higher if we are already the Root Directory
	if(name == ".." && m_rootDirectory == true)
	{
		return(nullptr);
	}
	
	
	
	const uint32 numberOfEntries = m_entries.get_size();
	I_Directory* directory = nullptr;
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		const Entry_FAT32& entry = *m_entries[i];
		if(entry.m_directory == true)
		{
			if(entry.m_name == name)
			{
				//	Reading myself
				if(name == ".")
				{
					return(this);
				}
				
				
				//	Assemble Path to new Directory
				String newPath(m_path);
				if(name == "..")
				{
					//	Go a Level higher
					newPath.eraseFromEnd(m_self.m_name.get_size());
					if(newPath[newPath.get_size() - 1] == '/')
					{
						newPath.eraseFromEnd(1);
					}
				}
				else
				{
					//	Go into the Subdirectory
					if(m_rootDirectory == false)
					{
						newPath += "/";
					}
					newPath += entry.m_name;
				}
				directory = new Directory_FAT32(m_drive, m_dma, m_bootsector, newPath, entry);
				return(directory);
			}
		}
	}
	return(directory);
}


CODE_RAM const I_File* Directory_FAT32::read(String name, String extension) const
{
	I_File* file = nullptr;
	if(is_valid() == false)
	{
		return(file);
	}
	
	const uint32 numberOfEntries = m_entries.get_size();
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		const Entry_FAT32& entry = *m_entries[i];
		if(entry.m_directory == false)
		{
			if(entry.m_name == name && entry.m_extension == extension)
			{
				file = new File_FAT32(m_drive, m_dma, m_bootsector, entry);
				return(file);
			}
		}
	}
	return(file);
}







CODE_RAM feedback Directory_FAT32::create(const I_File& file)
{
	return(FAIL);
}


CODE_RAM feedback Directory_FAT32::create(const I_Directory& directory)
{
	return(FAIL);
}






/*
I_Entry& Directory_FAT32::find(String name, String extension)
{
	for(uint32 i = 0; i < m_numberOfEntries; i++)
	{
		if(m_entries[i]->m_name == name && m_entries[i]->m_extension == extension)
		{
			return(*m_entries[i]);
		}
	}
	return((I_Entry&) *((I_Entry*) NULL));
}


String& Directory_FAT32::load(I_Entry& file)
{
	uint32 bytesPerSector = m_bootsector.get_bytesPerSector();
	uint32 sectorsPerCluster = m_bootsector.get_sectorsPerCluster();
	uint32 bytesPerCluster = bytesPerSector * sectorsPerCluster;
	
	uint32 numberOfClusters = (file.m_sizeInBytes / bytesPerCluster) + 1;
	
	String* data = new String;
	if(data->expand(file.m_sizeInBytes) != OK)
	{
		delete data;
		return((String&) *((String*) NULL));
	}
	
	
	
	uint8* data_raw = (uint8*) data->get_string();
	uint32 clusterNumber = 0;
	for(uint32 i = 0; i < numberOfClusters; i++)
	{
		uint32 clusterNumber_absolut = followClusterChain(file.m_clusterNumber, clusterNumber);																		//	Get new Clusternumber and read the Cluster
		if(clusterNumber_absolut <= 2)
		{
			delete data;
			return((String&) *((String*) NULL));
		}
		
		if(m_drive.read(m_bootsector.clusterToLBA(clusterNumber_absolut), sectorsPerCluster, data_raw + i * bytesPerCluster) != OK)
		{
			delete data;
			return((String&) *((String*) NULL));
		}
	}
	
	data->set_length(file.m_sizeInBytes);
	return(*data);
}


String& Directory_FAT32::load(I_Entry& file, uint32 clusterNumber)
{
	String* data = new String;
	uint32 clusterNumber_absolut = followClusterChain(file.m_clusterNumber, clusterNumber);																			//	Get Clusternumber of searched Cluster
	if(clusterNumber_absolut <= 2)
	{
		return(*data);
	}
	
	
	uint32 bytesPerSector = m_bootsector.get_bytesPerSector();
	uint32 sectorsPerCluster = m_bootsector.get_sectorsPerCluster();
	data->expand(bytesPerSector * sectorsPerCluster);																																							//	Expand Data String so it doesnt need to be copied everytime its expanded a bit
	
	m_drive.read(m_bootsector.clusterToLBA(clusterNumber_absolut), sectorsPerCluster, (uint8*) data->get_string());
	data->set_length(bytesPerSector * sectorsPerCluster);
	return(*data);
}*/