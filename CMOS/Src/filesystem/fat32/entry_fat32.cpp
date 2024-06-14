#include "../../../Inc/filesystem/fat32/entry_fat32.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Entry_FAT32::Entry_FAT32()
{
	
}


Entry_FAT32::~Entry_FAT32()
{
	
}







String Entry_FAT32::get_nameWithExtension() const
{
	String nameWithExtension(m_name);
	if(m_directory == false)
	{
		nameWithExtension += String(".") + m_extension;
	}
	return(nameWithExtension);
}


const String& Entry_FAT32::get_name() const
{
	return(m_name);
}


const String& Entry_FAT32::get_extension() const
{
	return(m_extension);
}


bool Entry_FAT32::is_readOnly() const
{
	return(m_readOnly);
}


bool Entry_FAT32::is_directory() const
{
	return(m_directory);
}


const Time& Entry_FAT32::get_creationTime() const
{
	return(m_time_creation);
}


uint32 Entry_FAT32::get_clusterNumber() const
{
	return(m_clusterNumber);
}


uint32 Entry_FAT32::get_sizeInBytes() const
{
	return(m_sizeInBytes);
}