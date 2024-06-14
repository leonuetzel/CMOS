#include "../../Inc/filesystem/filesystem.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback Filesystem::attach(I_Volume& volume)
{
	if(volume.get_label() != I_Volume::c_label_invalid)
	{
		return(FAIL);
	}
	
	for(uint32 i = 0; i < c_volume_number; i++)
	{
		if(m_volumes[i] == &volume)
		{
			return(FAIL);
		}
	}
	
	for(uint32 i = 0; i < c_volume_number; i++)
	{
		if(m_volumes[i] == nullptr)
		{
			m_volumes[i] = &volume;
			volume.set_label('A' + i);
			return(OK);
		}
	}
	return(FAIL);
}


feedback Filesystem::detach(I_Volume& volume)
{
	char label = volume.get_label();
	if(label != I_Volume::c_label_invalid)
	{
		m_volumes[label - 'A'] = nullptr;
		return(OK);
	}
	
	for(uint32 i = 0; i < c_volume_number; i++)
	{
		if(m_volumes[i] == &volume)
		{
			m_volumes[i] = nullptr;
			return(OK);
		}
	}
	return(FAIL);
}







CODE_RAM const I_Directory* Filesystem::read(String path)
{
	const I_Directory* directory = nullptr;
	if(path[1] != ':')
	{
		return(directory);
	}
	
	char volume = String::upperCase(path[0]);
	I_Volume* partition = m_volumes[(uint32) (volume - 'A')];
	if(partition == nullptr)
	{
		return(directory);
	}
	if(partition->is_valid() == false)
	{
		return(directory);
	}
	
	
	
	path.erase(0, 2);
	directory = partition->read(path);
	return(directory);
}