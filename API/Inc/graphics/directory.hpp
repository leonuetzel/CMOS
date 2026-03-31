#pragma once

#include "../cmos.hpp"
#include "../filesystem/filesystem.hpp"
#include "textbox.hpp"





class Directory: public Textbox
{
	public:
		
		static constexpr uint32 c_iconSize = 20;
		
		
		
		
		
	private:
		
		
		const RectGraphic& m_folderSymbol;
		const RectGraphic& m_fileSymbol;
		
		const I_Directory* m_directory;
		uint8 m_numberOfHiddenEntries;
		String m_pathChosen;
		
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeShape;
		f_element m_function_onChangePageActual;
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeShape(Element& element);
		static void onChangePageActual(Element& element);
		
		void draw_icon(uint32 lineNumber, const I_Entry& entry);
		
		
		
		
		
	public:
		
		Directory(Textbox textbox, String path, const RectGraphic& folderSymbol, const RectGraphic& fileSymbol);
		virtual ~Directory();
		
		constexpr inline void				set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangePage(f_element onChangePageFunction);
		constexpr inline void				set_function_onChangeShape(f_element onChangeShapeFunction);
		constexpr inline void				set_function_onChangePageActual(f_element onChangePageActualFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangePage() const;
		constexpr inline f_element	get_function_onChangeShape() const;
		constexpr inline f_element	get_function_onChangePageActual() const;
		
		feedback set_directory(String path);
		
		inline const String& get_path() const;
		inline const String& get_pathChosen() const;
};



/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      					Protected	  			 						 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

constexpr inline void Directory::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void Directory::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void Directory::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void Directory::set_function_onChangeShape(f_element onChangeShapeFunction)
{
	m_function_onChangeShape = onChangeShapeFunction;
}


constexpr inline Element::f_element Directory::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element Directory::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element Directory::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element Directory::get_function_onChangeShape() const
{
	return(m_function_onChangeShape);
}







inline const String& Directory::get_path() const
{
	return(m_directory->get_path());
}


inline const String& Directory::get_pathChosen() const
{
	return(m_pathChosen);
}