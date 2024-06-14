#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/
/*
void ColumnChart::onUpdate(Element& element)
{
	Diagram& diagram = (Diagram&) element;
	
	diagram.draw_background(diagram.m_colorBackground);
	diagram.draw_frame(diagram.m_colorFrame);
	diagram.draw_axis();
	diagram.draw_string(diagram.m_capture, e_align::TOP_CENTER, diagram.m_fontSize, diagram.m_colorText);
	
	Vec2 fontSize = Font::get_fontSize(diagram.m_fontSize);
	
	
	if(diagram.m_legendEnabled == true)
	{
		for(uint32 i = 0; i < diagram.m_numberOfLabels; i++)
		{
			diagram.draw_legend(i);
		}
	}
	
	if(diagram.m_axisLabelEnabled == true)
	{
		Vec2 position;
		position.x = diagram.m_usable.position.x + diagram.m_numberOfLabels * (c_distance_x + fontSize.x) + (c_distance_x + fontSize.x) / 2;
		position.y = diagram.m_usable.position.y - 1 - c_markerLength - fontSize.y - 3;
		
		diagram.draw_string(diagram.m_axisLabel_x, position, diagram.m_fontSize, diagram.m_colorText);
		
		position.x = diagram.m_usable.position.x + c_markerLength;
		position.y = diagram.m_usable.position.y + diagram.m_usable.size.y;
		diagram.draw_string(diagram.m_axisLabel_y, position, diagram.m_fontSize, diagram.m_colorText);
	}
	
	
	
	double min = 0;
	double max = 0;
	if(diagram.m_fixedAxis == true)
	{
		min = diagram.m_fixedAxisBoundaries.x;
		max = diagram.m_fixedAxisBoundaries.y;
	}
	else
	{
		for(uint32 i = 0; i < diagram.m_numberOfLabels; i++)
		{
			if(diagram.m_data[i] != nullptr)
			{
				min = Math::min(*(diagram.m_data[i]), min);
				max = Math::max(*(diagram.m_data[i]), max);
			}
		}
	}
	
	
	
	double pixel = diagram.m_usable.size.y - (fontSize.y + c_distance_y);
	double resolution = pixel / (max - min);
	
	for(uint32 i = 0; i < diagram.m_usable.size.y / (fontSize.y + c_distance_y); i++)																						//	Y-Values
	{
		double value = i * (fontSize.y + c_distance_y);
		value /= pixel;
		value *= (max - min);
		value += min;
		String label(value);
		
		Vec2 labelPosition;
		labelPosition.x = diagram.m_usable.position.x - 1 - c_markerLength - label.get_length() * (fontSize.x + 1) - 3;
		labelPosition.y = diagram.m_usable.position.y + i * (fontSize.y + c_distance_y) - fontSize.y / 2;
		diagram.draw_string(label, labelPosition, diagram.m_fontSize, diagram.m_colorText);
	}
	
	for(uint32 i = 0; i < diagram.m_numberOfLabels; i++)																																				//	Bars
	{
		if(diagram.m_data[i] != nullptr)
		{
			Rectangle bar;
			bar.position.x = diagram.m_usable.position.x + i * (c_distance_x + fontSize.x) + (c_distance_x + fontSize.x) / 2 - (c_bar_width - 1) / 2;
			bar.position.y = diagram.m_usable.position.y;
			
			bar.size.x = c_bar_width;
			bar.size.y = (*(diagram.m_data[i]) - min) * resolution + 1;																																//	Draw new Bar
			if(bar.size.y > diagram.m_usable.size.y)
			{
				bar.size.y = diagram.m_usable.size.y;
			}
			diagram.draw_rectangleFull(bar, diagram.m_colorBar);
		}
	}
	
	
	if(diagram.m_update != nullptr)
	{
		diagram.m_update(diagram);
	}
}


void ColumnChart::onCallback(Element& element)
{
	Diagram& diagram = (Diagram&) element;
	
	if(diagram.m_callback != nullptr)
	{
		diagram.m_callback(element);
		diagram.set_updateFlag();
	}
}


void ColumnChart::onChangePage(Element& element)
{
	
}


void ColumnChart::onChangeLayer(Element& element)
{
	
}


void ColumnChart::onChangePosition(Element& element)
{
	
}


void ColumnChart::onChangeSize(Element& element)
{
	
}







void ColumnChart::draw_axis()
{
	Vec2 fontSize = Font::get_fontSize(m_fontSize);
	
	Vec2 start(m_usable.position.x - 1, m_usable.position.y - 1);																															//	X-Axis
	Vec2 end(m_usable.get_topRightCorner().x, start.y);
	draw_line(start, end, m_colorAxis);
	
	Vec2 arrow_x(end.x - c_arrow_length, end.y - c_arrow_length);																															//	X-Arrow
	draw_line(arrow_x, end, m_colorAxis);
	arrow_x.y = end.y + c_arrow_length;
	draw_line(arrow_x, end, m_colorAxis);
	
	
	
	end.x = start.x;																																																							//	Y-Axis
	end.y = m_usable.position.y + m_usable.size.y;
	draw_line(start, end, m_colorAxis);
	
	Vec2 arrow_y(end.x - c_arrow_length, end.y - c_arrow_length);																															//	Y-Arrow
	draw_line(arrow_y, end, m_colorAxis);
	arrow_y.x = end.x + c_arrow_length;
	draw_line(arrow_y, end, m_colorAxis);
	
	
	
	for(uint32 i = 0; i < m_numberOfLabels; i++)																																								//	X-Marker
	{
		String label(i);
		
		Vec2 point_1;
		point_1.x = m_usable.position.x + i * (c_distance_x + fontSize.x) + (c_distance_x + fontSize.x) / 2;
		point_1.y = m_usable.position.y - 1;
		
		Vec2 point_2;
		point_2.x = point_1.x;
		point_2.y = point_1.y - c_markerLength;
		
		draw_line(point_1, point_2, m_colorAxis);
		
		Vec2 labelPosition;
		labelPosition.x = point_2.x - (label.get_length() * fontSize.x) / 2;
		labelPosition.y = point_2.y - fontSize.y - 3;
		draw_string(label, labelPosition, m_fontSize, m_colorText);
	}
	
	for(uint32 i = 0; i < m_usable.size.y / (fontSize.y + c_distance_y); i++)																										//	Y-Marker
	{
		Vec2 point_1(m_usable.position.x - 1, m_usable.position.y + i * (fontSize.y + c_distance_y));
		Vec2 point_2(point_1.x - c_markerLength, point_1.y);
		draw_line(point_1, point_2, m_colorAxis);
	}
}


void ColumnChart::draw_legend(uint32 index)
{
	if(index >= m_numberOfLabels)
	{
		return;
	}
	
	Vec2 fontSize = Font::get_fontSize(m_fontSize);
	
	Vec2 position;
	position.x = get_size().x - c_legend_length * (fontSize.x + 1) - 3;
	position.y = m_usable.get_topRightCorner().y - (index + 1) * (fontSize.y + 5);
	
	String legend = String(index) + ": ";
	legend += m_labels[index];
	draw_string(legend, position, m_fontSize, m_colorText);
}
*/


/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/
/*
ColumnChart::ColumnChart(Element element, Font::e_fontSize fontSize, Color colorText, Color colorBackground, Color colorFrame, Color colorBar, Color colorAxis, bool legend)
	:	Element(element),
		m_initialized(false),
		m_fontSize(fontSize),
		m_colorText(colorText),
		m_colorBackground(colorBackground),
		m_colorFrame(colorFrame),
		m_colorBar(colorBar),
		m_colorAxis(colorAxis),
		
		m_fixedAxis(false),
		m_legendEnabled(legend),
		m_axisLabelEnabled(true),
		
		m_function_onUpdate(Element::get_function_onUpdate()),
		m_function_onCallback(Element::get_function_onCallback()),
		m_function_onChangePage(Element::get_function_onChangePage()),
		m_function_onChangeLayer(Element::get_function_onChangeLayer()),
		m_function_onChangePosition(Element::get_function_onChangePosition()),
		m_function_onChangeSize(Element::get_function_onChangeSize())
{
	Element::set_function_onUpdate(onUpdate);
	Element::set_function_onCallback(onCallback);
	Element::set_function_onChangePage(onChangePage);
	Element::set_function_onChangeLayer(onChangeLayer);
	Element::set_function_onChangePosition(onChangePosition);
	Element::set_function_onChangeSize(onChangeSize);
	
	
	
	Vec2 size = get_size();
	Vec2 font = Font::get_fontSize(m_fontSize);
	
	m_usable.position.x = c_value_length * (font.x + 1) + c_markerLength + 5;
	m_usable.position.y = font.y + c_markerLength + 5;
	m_usable.size.y = size.y - 2 * m_usable.position.y;
	if(m_legendEnabled == true)
	{
		m_usable.size.x = size.x - c_legend_length * (font.x + 2) - m_usable.position.x;
	}
	else
	{
		m_usable.size.x = size.x - m_usable.position.x - 5;
	}
	
	m_numberOfLabels = m_usable.size.x / (c_distance_x + font.x) - 1;
	m_labels = new String[m_numberOfLabels];
	m_data = new double*[m_numberOfLabels];
	for(uint32 i = 0; i < m_numberOfLabels; i++)
	{
		m_labels[i] = "";
		m_data[i] = nullptr;
	}
}


ColumnChart::~ColumnChart()
{
	delete[] m_labels;
	delete[] m_data;
}












feedback ColumnChart::set_label(uint8 index, String label)
{
	if(index < m_numberOfLabels)
	{
		uint32 length = label.get_length();
		if(length > c_legend_length)
		{
			label.remove(c_legend_length - 3, length - c_legend_length + 3);
			label += "...";
		}
		
		m_labels[index] = label;
		return(OK);
	}
	set_updateFlag();
	
	return(FAIL);
}


feedback ColumnChart::set_label(String* labels)
{
	for(uint32 i = 0; i < m_numberOfLabels; i++)
	{
		labels[i].insert(String(i) + ": ", 0);
		uint32 length = labels[i].get_length();
		if(length > c_legend_length)
		{
			labels[i].remove(c_legend_length, length - c_legend_length);
		}
		
		m_labels[i] = labels[i];
	}
	set_updateFlag();
	
	return(OK);
}


feedback ColumnChart::set_data(double* data, uint32 index)
{
	if(index < m_numberOfLabels)
	{
		m_data[index] = data;
		return(OK);
	}
	set_updateFlag();
	
	return(FAIL);
}


feedback ColumnChart::set_data(double** data)
{
	for(uint32 i = 0; i < m_numberOfLabels; i++)
	{
		m_data[i] = data[i];
	}
	set_updateFlag();
	
	return(OK);
}


feedback ColumnChart::set_fixedAxis(bool enable)
{
	m_fixedAxis = enable;
	set_updateFlag();
	
	return(OK);
}


feedback ColumnChart::set_fixedAxisBoundaries(double min, double max)
{
	m_fixedAxisBoundaries.x = min;
	m_fixedAxisBoundaries.y = max;
	
	if(m_fixedAxis == true)
	{
		set_updateFlag();
	}
	
	return(OK);
}


feedback ColumnChart::set_axisLabel(String x, String y)
{
	uint32 length = x.get_length();
	if(length > c_legend_length)
	{
		x.remove(c_value_length, length - c_value_length);
	}
	length = y.get_length();
	if(length > c_legend_length)
	{
		y.remove(c_value_length, length - c_value_length);
	}
	m_axisLabel_x = x;
	m_axisLabel_y = y;
	set_updateFlag();
	
	return(OK);
}


feedback ColumnChart::set_axisLabel(bool enable)
{
	m_axisLabelEnabled = enable;
	set_updateFlag();
	
	return(OK);
}*/