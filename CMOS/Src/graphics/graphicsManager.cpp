#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Graphics::manager()
{
	CMOS& cmos = CMOS::get();
	
	
	//	Ticks for periodic Element Update
	constexpr uint32 c_ticks_per_ms = CMOS::c_clock_systick / 1000;
	
	
	//	Start Display Driver
	const uint16 eventID_frameFinished = m_displayDriver->get_eventID_frameFinished();
	cmos.event_subscribe(eventID_frameFinished);
	if(m_displayDriver->start() != OK)
	{
		return;
	}
	
	
	//	Init Graphic Accelerator Interrupt
	const uint16 c_interrupt_graphicAccelerator	= m_graphicAccelerator->get_wakeUpInterrupt();
	
	NVIC& nvic = cmos.get_nvic();														
	nvic.setPriority(c_interrupt_graphicAccelerator, 14);
	nvic.enable(c_interrupt_graphicAccelerator);
	
	
	//	Clear Backbuffer and set Background Color in Display Driver to Black
	for(uint32 i = 0; i < c_numberOfLayers; i++)
	{
		clear_layer(i);
	}
	set_backgroundColor(Colors::black);
	
	
	
	
	
	//	Send Message to Parent Thread (waiting in the Init-Function for Response) to show that Thread is ready
	cmos.send_mail(cmos.get_parentThreadID(), 0);
	
	uint8 fps = 0;
	uint8 second = cmos.get_time().second;
	Array<bool> change(false, c_numberOfLayers);
	
	
	
	
	
	while(1)
	{
		for(uint32 i = 0; i < c_numberOfLayers; i++)
		{
			change[i] = false;
		}
		
		
		//	Layer Reload System
		while(cmos.is_mailAvailable() == true)
		{
			Vec2 touchPoint;
			
			Thread::s_mail mail = cmos.read_mail();
			if(mail.type == Thread::e_mailType::NORMAL)
			{
				if(mail.data == c_numberOfLayers)
				{
					for(auto& i: change)
					{
						i = true;
					}
				}
				else
				{
					change[mail.data] = true;
				}
			}
		}
		
		
		//	Display Reload requested
		if(m_reloadRequested == true)
		{
			//	Clear Backbuffer
			for(uint32 i = 0; i < c_numberOfLayers; i++)
			{
				change[i] = true;
				clear_layer(i);
			}
			
			
			//	Update all Elements that are on the actual Page
			for(uint32 i = 0; i < m_elements.get_size(); i++)
			{
				Element& element = *m_elements[i];
				element.onChangePageActual();
				if(element.m_page == m_pageActual)
				{
					element.m_ticks = cmos.get_ticks();
					
					element.m_rebuildRequested = true;
					element.m_updateRequested = true;
					element.onUpdate();
				}
			}
			m_reloadRequested = false;
		}
		
		
		//	Periodic Element Update
		for(uint32 i = 0; i < m_elements.get_size(); i++)
		{
			Element& element = *m_elements[i];
			if(element.m_page == m_pageActual)
			{
				//	Check if Element needs to be updated
				bool updateManual = false;
				bool updatePeriodic = false;
				{
					if(element.m_rebuildRequested == true || element.m_updateRequested == true)
					{
						updateManual = true;
					}
					else
					{
						if(element.m_updateRate_ms > 0)
						{
							if(cmos.get_ticks() - element.m_ticks > element.m_updateRate_ms * c_ticks_per_ms)
							{
								element.m_updateRequested = true;
								updatePeriodic = true;
							}
						}
					}
				}
				
				
				//	Update Element if needed
				if(updateManual == true || updatePeriodic == true)
				{
					change[element.m_layer] = true;
					element.onUpdate();
					if(updatePeriodic == true)
					{
						element.m_ticks = cmos.get_ticks();
					}
				}
			}
		}
		
		
		//	Sleep until vertical Sync
		cmos.event_listen(eventID_frameFinished);
		cmos.sleep_untilEvent(eventID_frameFinished);
		
		
		//	Calculate actual Framerate
		fps++;
		if(cmos.get_time().second != second)
		{
			second = cmos.get_time().second;
			m_fps = fps;
			fps = 0;
		}
		
		
		//	Blend Layer 0 with Background if available
		if(change[0] == true)
		{
			if(m_background.data != nullptr && m_background_visible == true)
			{
				m_graphicAccelerator->draw_rectangleFull(m_displayDriver->get_layerData(0), Element::m_backBuffer[0], m_background);
			}
			else
			{
				m_graphicAccelerator->draw_rectangleFull(m_displayDriver->get_layerData(0), Element::m_backBuffer[0]);
			}
		}
		
		
		//	Transfer all other Layers
		for(uint32 i = 1; i < c_numberOfLayers; i++)
		{
			if(change[i] == true)
			{
				m_graphicAccelerator->draw_rectangleFull(m_displayDriver->get_layerData(i), Element::m_backBuffer[i]);
			}
		}
		
		
		//	Wait for Transfers to be finished
		while(m_graphicAccelerator->is_available() == false)
		{
			#if defined(CORTEX_M7)
				cmos.sleep_100us(1);
			#endif
			
			#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
				cmos.sleep_ms(1);
			#endif
		}
	}
}


CODE_RAM feedback Graphics::clear_layer(uint8 layer)
{
	if(layer >= c_numberOfLayers)
	{
		return(FAIL);
	}
	
	CMOS& cmos = CMOS::get();
	m_graphicAccelerator->draw_rectangleFull(Element::m_backBuffer[layer], Colors::transparent);
	while(m_graphicAccelerator->is_available() == false)
	{
		#if defined(CORTEX_M7)
			cmos.sleep_100us(1);
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
			cmos.sleep_ms(1);
		#endif
	}
	return(OK);
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback Graphics::init(I_GraphicAccelerator& graphicAccelerator, I_DisplayDriver& displayDriver, I_Semaphore& semaphore, uint8 numberOfPages, const Array<Color*>& backBuffer, Color* backgroundBuffer)
{
	//	Check double Initialization
	if(m_initialized == true)
	{
		return(FAIL);
	}
	
	
	//	Member Initialization
	m_graphicAccelerator					= &graphicAccelerator;
	m_displayDriver								= &displayDriver;
	
	c_numberOfLayers							= displayDriver.get_numberOfLayers();
	c_numberOfPages								= numberOfPages;
	
	Element::m_graphicAccelerator	= m_graphicAccelerator;
	Element::m_backBuffer					= new RectGraphic[c_numberOfLayers];
	
	
	//	Init Software Background that will be blended in by the Graphic Accelerator
	m_background = RectGraphic(m_displayDriver->get_layerData(0), backgroundBuffer);
	if(backgroundBuffer != nullptr)
	{
		m_background_visible = true;
	}
	
	
	//	Set Backbuffer Pointer
	for(uint32 i = 0; i < c_numberOfLayers; i++)
	{
		Rect rect = m_displayDriver->get_layerData(i);
		Color* data = backBuffer[i];
		
		Element::m_backBuffer[i] = RectGraphic(rect, data);
	}
	
	
	//	Layer Touchability (all Layer touchable by default)
	m_layerTouchability.set_size(c_numberOfLayers, true, true);
	
	
	//	Create Graphics Manager Thread
	CMOS& cmos = CMOS::get();
	m_thread_ID = cmos.thread_create(&Graphics::manager, this, "Graphics Manager", 10, 4096);
	cmos.sleep_untilMail(m_thread_ID);
	
	m_initialized = true;
	return(OK);
}


Graphics& Graphics::get()
{
	static Graphics graphics;
	return(graphics);
}







void Graphics::erase()
{
	//	Dont optimize this Code here, because we modify the Array "m_elements" in the "operator()" Function
	//	This leads to incorrect Function if not done like this here!
	while(m_elements.get_size() > 0)
	{
		(*this) -= m_elements[0];
	}
}







void Graphics::register_touchData(Vec2 touchPoint, e_touchEvent touchEvent)
{
	if(m_initialized == true && touchEvent != e_touchEvent::INVALID)
	{
		//	Make the new Touch Event visible to all Elements
		Element::m_touchEvent = touchEvent;
		
		
		//	Determine which Element has been touched
		for(int32 layer = c_numberOfLayers - 1; layer >= 0; layer--)
		{
			if(m_layerTouchability[layer] == true)
			{
				for(auto& i: m_elements)
				{
					if(i->m_page == m_pageActual)
					{
						if(i->m_layer == layer)
						{
							if(i->m_touchable == true && i->m_visible == true)
							{
								const Vec2 touchPoint_relativeToLayer(touchPoint - Element::m_backBuffer[i->m_layer].position);
								if(i->containsPoint(touchPoint_relativeToLayer) == true)
								{
									i->m_touchValid = true;
									if(i->m_function_onCallback != nullptr)
									{
										i->m_touchPosition = touchPoint_relativeToLayer - i->position;
										const String callbackThreadName = "Graphics Callback " + String((uint32) i);
										CMOS& cmos = CMOS::get();
										if(cmos.thread_doesExist(callbackThreadName) == false)
										{
											const uint8 thread_ID = cmos.thread_create(&Element::onCallback, i, callbackThreadName, 150);
											if(thread_ID != CMOS::threadID_invalid)
											{
												cmos.thread_detachChildThread(thread_ID);
												cmos.send_mail(thread_ID, (uint32) i);
											}
										}
									}
									layer = -1;
									break;
								}
								else
								{
									i->m_touchValid = false;
								}
							}
							else
							{
								i->m_touchValid = false;
							}
						}
					}
					else
					{
						i->m_touchValid = false;
					}
				}
			}
		}
	}
}







Graphics& Graphics::operator+=(Element& element)
{
	//	Check UpdateFunction, Page and Layer
	if(element.m_function_onUpdate == nullptr || element.m_page >= c_numberOfPages || element.m_layer >= c_numberOfLayers)
	{
		return(*this);
	}
	
	
	//	Check Minimum Dimensions
	if(element.size.x < Element::c_minimumSideLength || element.size.y < Element::c_minimumSideLength)
	{
		return(*this);
	}
	
	
	//	Check Display Borders
	if(element.position.x < 0 || element.position.y < 0)
	{
		return(*this);
	}
	
	Vec2 topRight(element.get_topRightCorner());
	if(topRight.x >= Element::m_backBuffer[element.m_layer].size.x || topRight.y >= Element::m_backBuffer[element.m_layer].size.y)
	{
		return(*this);
	}
	
	
	//	Check Overlap with exisiting Elements
	for(uint32 i = 0; i < m_elements.get_size(); i++)
	{
		Element& element2 = *m_elements[i];
		
		if(element2.get_page() == element.get_page() && element2.get_layer() == element.get_layer())
		{
			if(element2.doesOverlap(element) == true)
			{
				return(*this);
			}
		}
	}
	
	
	//	Create Semaphore for the new Element
	//	This is needed for the Element to be able to lock itself on Update and Callback Function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_create(&element) != OK)
	{
		return(*this);
	}
	
	
	//	Add Element to Array
	m_elements += &element;
	element.m_rebuildRequested = true;
	element.m_updateRequested = true;
	
	return(*this);
}


Graphics& Graphics::operator-=(Element& element)
{
	//	Save Update Rate to not change the Element in Case it is just disabled temporarily
	const uint16 updateRate_ms = element.m_updateRate_ms;
	
	
	//	Wait for an eventual Update to be finished, but dont wait if we are executing this Function from the Update Function (wait forever...)
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(&element) != OK)
	{
		return(*this);
	}
	
	
	//	Dont update this Element anymore
	element.m_updateRate_ms = 0;
	element.m_updateRequested = false;
	element.m_rebuildRequested = false;
	
	
	const uint32 index = m_elements.find(&element);
	if(m_elements.is_valid(index) == true)
	{
		//	Clear Element from Screen
		element.clear();
		
		
		//	Erase Element from Array
		m_elements.erase(index);
		
		
		//	Erase Semaphore of this Element
		if(cmos.semaphore_erase(&element) != OK)
		{
			return(*this);
		}
		
		
		//	Reload Layer to Frontbuffer
		transferLayerToFrontbuffer(element.m_layer);
	}
	else
	{
		cmos.semaphore_unlock(&element);
	}
	
	
	//	Restore changed Properties
	element.m_updateRate_ms = updateRate_ms;
	return(*this);
}