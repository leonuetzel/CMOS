#include "../../../Inc/filesystem/fat32/volume_fat32.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Volume_FAT32::Volume_FAT32(const I_Drive& drive, I_DMA& dma, const uint32& changeStamp, uint32 startSector_LBA)
	:	m_drive(drive),
		m_dma(dma),
		m_label(c_label_invalid),
		m_bootsector(m_drive, m_dma, changeStamp)
{
	if(m_bootsector.is_valid() == false)
	{
		return;
	}
	
	uint8* bootSector = m_drive.read(startSector_LBA, 1);
	if(m_bootsector.init(bootSector, startSector_LBA) != OK)
	{
		return;
	}
	delete[] bootSector;
}


Volume_FAT32::~Volume_FAT32()
{
	
}







CODE_RAM bool Volume_FAT32::is_valid() const
{
	return(m_bootsector.is_valid());
}


CODE_RAM const I_Directory* Volume_FAT32::read() const
{
	//	Check for valid Directory
	if(is_valid() == false)
	{
		return(nullptr);
	}
	
	
	//	Create Root Directory Entry
	Time timestamp = CMOS::get().get_time();
	
	Entry_FAT32 root;
	root.m_name = String(m_label);
	root.m_extension = "";
	root.m_attribute = 0;
	root.m_readOnly = true;
	root.m_hidden = false;
	root.m_system = false;
	root.m_volume_ID = false;
	root.m_directory = true;
	root.m_archive = false;
	root.m_time_creation = timestamp;
	root.m_time_access = timestamp;
	root.m_time_update = timestamp;
	root.m_clusterNumber = 2;
	root.m_sizeInBytes = 0;
	
	return(new Directory_FAT32(m_drive, m_dma, m_bootsector, String(m_label) + ":", root));
}


CODE_RAM const I_Directory* Volume_FAT32::read(String path) const
{
	//	Check for valid Directory
	if(is_valid() == false)
	{
		return(nullptr);
	}
	
	//	Read Root Directory
	const I_Directory* directory_parent = read();
	
	
	//	Root Directory
	if(path.get_size() == 0)
	{
		return(directory_parent);
	}
	
	const uint32 numberOfDirectoryLevels = path.count('/') + 1;
	for(uint32 i = 0; i < numberOfDirectoryLevels; i++)
	{
		const I_Directory* directory_child = directory_parent->read(path.subWithDelimiter('/', i));
		delete directory_parent;
		directory_parent = directory_child;
	}
	return(directory_parent);
}


feedback Volume_FAT32::set_label(char label)
{
	m_label = label;
	return(OK);
}


char Volume_FAT32::get_label() const
{
	return(m_label);
}