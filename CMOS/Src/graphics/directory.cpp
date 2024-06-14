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
	if(directory.m_rebuildRequested == true)
	{
		
	}
	
	
	//	Normal Update
	if(directory.m_updateRequested == true)
	{
		//	Check for valid Directory
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
		
		
		//	Get Number of Entries to show
		const uint32 numberOfEntries = directory.m_directory->get_numberOfEntries() - directory.m_numberOfHiddenEntries;
		
		
		//	Set Number of Lines in Textbox according to Number of Entries to show + 1 Line for Parent Directory Name
		const Array<I_Entry*>& entries = directory.m_directory->list();
		if(directory.get_numberOfLines() != numberOfEntries + 1)
		{
			directory.set_numberOfLines(numberOfEntries + 1);
			
			
			//	Set x Offset so that the Folder and File Icons wont be overwritten
			directory.set_xOffset(c_iconSize + 3);
			directory.set_xOffset(0, 0);
		}
		
		
		//	Write Parent Directory Name
		directory.set_text(directory.m_directory->get_path(), 0);
		
		
		//	Write Entries
		for(uint32 i = 0; i < numberOfEntries; i++)
		{
			const I_Entry& entry = *entries[i + directory.m_numberOfHiddenEntries];
			
			
			const uint32 lineNumber = i + 1;
			directory.set_text(entry.get_nameWithExtension(), lineNumber);
			directory.draw_icon(lineNumber, entry);
		}
	}
}


CODE_RAM void Directory::onCallback(Element& element)
{
	Directory& directory = (Directory&) element;
	
	
	//	Check for valid Directory
	if(directory.m_directory == nullptr)
	{
		return;
	}
	if(directory.m_directory->is_valid() == false)
	{
		return;
	}
	
	
	//	Read Entries from underlying I_Directory
	const uint32 numberOfEntries = directory.m_directory->get_numberOfEntries();
	const Array<I_Entry*>& entries = directory.m_directory->list();
	
	
	//	Get touched Line
	const uint32 line = directory.get_lineTouched();
	bool isFileChosen = false;
	if(line < numberOfEntries + 1)
	{
		if(line == 0)
		{
			//	Go one Directory Level up
			if(directory.m_directory->is_rootDirectory() == false)
			{
				directory.set_directory(directory.m_directory->get_parent()->get_path());
			}
		}
		else
		{
			//	Get touched Entry
			const I_Entry& entry = *entries[line - 1 + directory.m_numberOfHiddenEntries];
			
			
			//	Assemble Path to chosen Entry
			String basePath = directory.m_directory->get_path();
			if(directory.m_directory->is_rootDirectory() == false)
			{
				basePath += "/";
			}
			
			
			//	Check if a Subdirectory or a File has been chosen
			if(entry.is_directory() == true)
			{
				//	Chosen Entry is a Subdirectory
				directory.set_directory(basePath + entry.get_name());
			}
			else
			{
				//	Chosen Entry is a File, save File Path for higher Level Applications
				directory.m_pathChosen = basePath + entry.get_nameWithExtension();
				isFileChosen = true;
			}
		}
		
		
		//	Delete Touch Flags
		if(isFileChosen == false)
		{
			directory.reset_touchFlags();
		}
	}
	
	
	directory.m_rebuildRequested = true;
	
	
	//	Execute User Callback
	if(directory.m_function_onCallback != nullptr)
	{
		directory.m_function_onCallback(element);
		directory.m_updateRequested = true;
	}
}


CODE_RAM void Directory::onChangePage(Element& element)
{
	
}


CODE_RAM void Directory::onChangeLayer(Element& element)
{
	
}


CODE_RAM void Directory::onChangePosition(Element& element)
{
	
}


CODE_RAM void Directory::onChangeSize(Element& element)
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
		m_function_onChangePage(Textbox::get_function_onChangePage()),
		m_function_onChangeLayer(Textbox::get_function_onChangeLayer()),
		m_function_onChangePosition(Textbox::get_function_onChangePosition()),
		m_function_onChangeSize(Textbox::get_function_onChangeSize())
{
	Textbox::set_function_onUpdate(onUpdate);
	Textbox::set_function_onCallback(onCallback);
	Textbox::set_function_onChangePage(onChangePage);
	Textbox::set_function_onChangeLayer(onChangeLayer);
	Textbox::set_function_onChangePosition(onChangePosition);
	Textbox::set_function_onChangeSize(onChangeSize);
	
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
		m_rebuildRequested = true;
		return(OK);
	}
	return(FAIL);
}