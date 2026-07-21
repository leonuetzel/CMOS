#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Directory::onUpdate(Element& element)
{
	Directory& directory = (Directory&) element;
	
	
	
	//	Execute User Update first to be able to display Changes after
	if(directory.m_function_onUpdate != nullptr)
	{
		directory.m_function_onUpdate(element);
	}
	
	
	//	Full Rebuild
	if(directory.isRebuildRequested() == true)
	{
		
	}
	
	
	//	Check for valid directory
	if(directory.m_directory == nullptr)
	{
		directory.clear();
		return;
	}
	if(directory.m_directory->is_valid() == false)
	{
		directory.clear();
		return;
	}
	
	
	//	Get number of entries to show
	const uint32 numberOfEntries = directory.m_directory->get_numberOfEntries() - directory.m_numberOfHiddenEntries;
	
	
	//	Set number of lines in textbox according to number of entries to show + 1 line for parent directory name
	const Array<I_Entry*>& entries = directory.m_directory->list();
	if(directory.get_numberOfLines() != numberOfEntries + 1)
	{
		directory.set_numberOfLines(numberOfEntries + 1);
		
		
		//	Set x offset so that the folder and file icons wont be overwritten
		directory.set_xOffset(c_iconSize + 3);
		directory.set_xOffset(0, 0);
	}
	
	
	//	Write parent directory name
	directory.set_text(directory.m_directory->get_path(), 0);
	
	
	//	Write entries
	for(uint32 i = 0; i < numberOfEntries; i++)
	{
		const I_Entry& entry = *entries[i + directory.m_numberOfHiddenEntries];
		
		
		const uint32 lineNumber = i + 1;
		directory.set_text(entry.get_nameWithExtension(), lineNumber);
		directory.draw_icon(lineNumber, entry);
	}
}


CODE_RAM void Directory::onCallback(Element& element)
{
	Directory& directory = (Directory&) element;
	
	
	//	Check for valid directory
	if(directory.m_directory == nullptr)
	{
		return;
	}
	if(directory.m_directory->is_valid() == false)
	{
		return;
	}
	
	
	//	Read entries from underlying I_Directory
	const uint32 numberOfEntries = directory.m_directory->get_numberOfEntries();
	const Array<I_Entry*>& entries = directory.m_directory->list();
	
	
	//	Get touched line
	const uint32 line = directory.get_lineTouched();
	bool isFileChosen = false;
	if(line < numberOfEntries + 1)
	{
		if(line == 0)
		{
			//	Go one directory level up
			if(directory.m_directory->is_rootDirectory() == false)
			{
				directory.set_directory(directory.m_directory->get_parent()->get_path());
			}
		}
		else
		{
			//	Get touched entry
			const I_Entry& entry = *entries[line - 1 + directory.m_numberOfHiddenEntries];
			
			
			//	Assemble path to chosen entry
			String basePath = directory.m_directory->get_path();
			if(directory.m_directory->is_rootDirectory() == false)
			{
				basePath += "/";
			}
			
			
			//	Check if a subdirectory or a file has been chosen
			if(entry.is_directory() == true)
			{
				//	Chosen entry is a subdirectory
				directory.set_directory(basePath + entry.get_name());
			}
			else
			{
				//	Chosen entry is a file, save file path for higher level applications
				directory.m_pathChosen = basePath + entry.get_nameWithExtension();
				isFileChosen = true;
			}
		}
		
		
		//	Delete touch flags
		if(isFileChosen == false)
		{
			directory.reset_touchFlags();
		}
	}
	
	
	directory.requestRebuild();
	
	
	//	Execute user callback
	if(directory.m_function_onCallback != nullptr)
	{
		directory.m_function_onCallback(element);
		directory.requestUpdate();
	}
}


CODE_RAM void Directory::onChangeMyPage(Element& element)
{
	
}


CODE_RAM void Directory::onChangeShape(Element& element)
{
	
}







CODE_RAM void Directory::draw_icon(uint32 lineNumber, const I_Entry& entry)
{
	const Color* iconData = m_folderSymbol.data;
	if(entry.is_directory() == false)
	{
		iconData = m_fileSymbol.data;
	}
	
	if(iconData == nullptr)
	{
		return;
	}
	
	Vec2 start(1, get_lineCoordinate(lineNumber));
	
	for(uint32 y = 0; y < c_iconSize; y++)
	{
		for(uint32 x = 0; x < c_iconSize; x++)
		{
			Vec2 pixel(x, y);
			set_pixel(start + pixel, *iconData);
			iconData++;
		}
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

Directory::Directory(Textbox textbox, String path, const RectGraphic& folderSymbol, const RectGraphic& fileSymbol)
	:	Textbox(textbox),
		
		m_folderSymbol(folderSymbol),
		m_fileSymbol(fileSymbol),
		
		m_directory(nullptr),
		m_numberOfHiddenEntries(0),
		m_pathChosen(),
		
		m_function_onUpdate(Textbox::get_function_onUpdate()),
		m_function_onCallback(Textbox::get_function_onCallback()),
		m_function_onChangeMyPage(Textbox::get_function_onChangeMyPage()),
		m_function_onChangeShape(Textbox::get_function_onChangeShape())
{
	Textbox::set_function_onUpdate(onUpdate);
	Textbox::set_function_onCallback(onCallback);
	Textbox::set_function_onChangeMyPage(onChangeMyPage);
	Textbox::set_function_onChangeShape(onChangeShape);
	
	set_directory(path);
}


Directory::~Directory()
{
	
}







CODE_RAM feedback Directory::set_directory(String path)
{
	const I_Directory* directory = Filesystem::get().read(path);
	if(directory != nullptr)
	{
		//	Delete old Directory
		if(m_directory != nullptr)
		{
			delete m_directory;
		}
		
		
		//	Set new Directory
		m_directory = directory;
		
		
		//	Hide return-to-higher-Level Entries in Directory
		m_numberOfHiddenEntries = 0;
		for(auto& i: m_directory->list())
		{
			if(i->is_directory() == true)
			{
				const String& name = i->get_name();
				if(name == "." || name == "..")
				{
					m_numberOfHiddenEntries++;
				}
			}
		}
		clear();
		requestRebuild();
		return(OK);
	}
	return(FAIL);
}