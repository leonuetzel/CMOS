#pragma once

#include "../cmos.hpp"
#include "element.hpp"





class ColumnChart: public Element
{
	public:
		
		
		
		
		
		
		
	private:
		
		static constexpr uint32 c_distance_x = 12;
		static constexpr uint32 c_distance_y = 6;
		static constexpr uint32 c_markerLength = 5;
		static constexpr uint32 c_bar_width = 7;
		static constexpr uint32 c_legend_length = 15;
		static constexpr uint32 c_value_length = 10;
		static constexpr uint32 c_arrow_length = 5;
		
		bool m_initialized;
		String m_capture;
		const Font& m_font;
		Color m_colorText;
		Color m_colorBackground;
		Color m_colorFrame;
		Color m_colorBar;
		Color m_colorAxis;
		
		
		uint8 m_numberOfLabels;
		String* m_labels;
		double** m_data;
		
		Rectangle m_usable;
		
		bool m_fixedAxis;
		Vector2<double> m_fixedAxisBoundaries;
		
		bool m_legendEnabled;
		bool m_axisLabelEnabled;
		String m_axisLabel_x;
		String m_axisLabel_y;
		
		f_element m_function_onUpdate;
		f_element m_function_onCallback;
		f_element m_function_onChangePage;
		f_element m_function_onChangeLayer;
		f_element m_function_onChangePosition;
		f_element m_function_onChangeSize;
		
		static void onUpdate(Element& element);
		static void onCallback(Element& element);
		static void onChangePage(Element& element);
		static void onChangeLayer(Element& element);
		static void onChangePosition(Element& element);
		static void onChangeSize(Element& element);
		
		void draw_axis();
		void draw_legend(uint32 index);
		
		
		
		
		
	public:
		
		ColumnChart(Element element, const Font& font, Color colorText, Color colorBackground, Color colorFrame, Color colorBar, Color colorAxis, bool legend);
		virtual ~ColumnChart();
		
		constexpr inline void				set_function_onUpdate(f_element onUpdateFunction);
		constexpr inline void				set_function_onCallback(f_element onCallbackFunction);
		constexpr inline void				set_function_onChangePage(f_element onChangePageFunction);
		constexpr inline void				set_function_onChangeLayer(f_element onChangeLayerFunction);
		constexpr inline void				set_function_onChangePosition(f_element onChangePositionFunction);
		constexpr inline void				set_function_onChangeSize(f_element onChangeSizeFunction);
		constexpr inline f_element	get_function_onUpdate() const;
		constexpr inline f_element	get_function_onCallback() const;
		constexpr inline f_element	get_function_onChangePage() const;
		constexpr inline f_element	get_function_onChangeLayer() const;
		constexpr inline f_element	get_function_onChangePosition() const;
		constexpr inline f_element	get_function_onChangeSize() const;
		
		inline void set_capture(String capture);
		constexpr inline uint8 get_numberOfLabels() const;
		feedback set_label(uint8 index, String label);
		feedback set_label(String* labels);
		feedback set_data(double* data, uint32 index);
		feedback set_data(double** data);
		feedback set_fixedAxis(bool enable);
		feedback set_fixedAxisBoundaries(double min, double max);
		feedback set_axisLabel(String x, String y);
		feedback set_axisLabel(bool enable);
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

constexpr inline void ColumnChart::set_function_onUpdate(f_element onUpdateFunction)
{
	m_function_onUpdate = onUpdateFunction;
}


constexpr inline void ColumnChart::set_function_onCallback(f_element onCallbackFunction)
{
	m_function_onCallback = onCallbackFunction;
}


constexpr inline void ColumnChart::set_function_onChangePage(f_element onChangePageFunction)
{
	m_function_onChangePage = onChangePageFunction;
}


constexpr inline void ColumnChart::set_function_onChangeLayer(f_element onChangeLayerFunction)
{
	m_function_onChangeLayer = onChangeLayerFunction;
}


constexpr inline void ColumnChart::set_function_onChangePosition(f_element onChangePositionFunction)
{
	m_function_onChangePosition = onChangePositionFunction;
}


constexpr inline void ColumnChart::set_function_onChangeSize(f_element onChangeSizeFunction)
{
	m_function_onChangeSize = onChangeSizeFunction;
}


constexpr inline Element::f_element ColumnChart::get_function_onUpdate() const
{
	return(m_function_onUpdate);
}


constexpr inline Element::f_element ColumnChart::get_function_onCallback() const
{
	return(m_function_onCallback);
}


constexpr inline Element::f_element ColumnChart::get_function_onChangePage() const
{
	return(m_function_onChangePage);
}


constexpr inline Element::f_element ColumnChart::get_function_onChangeLayer() const
{
	return(m_function_onChangeLayer);
}


constexpr inline Element::f_element ColumnChart::get_function_onChangePosition() const
{
	return(m_function_onChangePosition);
}


constexpr inline Element::f_element ColumnChart::get_function_onChangeSize() const
{
	return(m_function_onChangeSize);
}







inline void ColumnChart::set_capture(String capture)
{
	m_capture = capture;
	m_updateRequested = true;
}


constexpr inline uint8 ColumnChart::get_numberOfLabels() const
{
	return(m_numberOfLabels);
}