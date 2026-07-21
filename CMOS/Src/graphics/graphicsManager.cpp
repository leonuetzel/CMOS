#include "../../Inc/graphics/graphics.hpp"





/*****************************************************************************/
/*                    Globals and Static Initialization					 						 */
/*****************************************************************************/





/*****************************************************************************/
/*                      						Private	  			 						 						 */
/*****************************************************************************/

CODE_RAM void Graphics::manager()
{
	//	Start display driver with the second framebuffer (frontbuffer)
	const uint16 eventID_vSync = m_displayDriver->get_eventID_vSync();
	CMOS& cmos = CMOS::get();
	cmos.event_subscribe(eventID_vSync);
	
	if(m_displayDriver->set_colorBackground(m_backgroundColor) != OK)
	{
		return;
	}
	RectGraphic frameBufferInitial(m_frameBufferShape, m_frameBufferData[0]);
	if(m_displayDriver->set_layerData(0, frameBufferInitial) != OK)
	{
		return;
	}
	if(m_displayDriver->set_layerBuffer(0, m_frameBufferData[1]) != OK)
	{
		return;
	}
	if(m_displayDriver->start() != OK)
	{
		return;
	}
	
	
	//	Send message to parent thread (waiting in the init-function for response) to show that thread is ready
	cmos.send_mail(cmos.get_parentThreadID(), 0);
	
	
	
	
	
	uint8 fps = 0;
	uint8 second = cmos.get_time().second;
	uint64& frameCounter = m_displayDriver->get_frameCounter();
	while(1)
	{
		//	Calculate Framerate
		fps++;
		const uint8 secondNow = cmos.get_time().second;
		if(secondNow != second)
		{
			second = secondNow;
			m_fps = fps;
			fps = 0;
		}
		
		
		//	Soft-exit mechanism
		while(cmos.is_mailAvailable() == true)
		{
			const Thread::s_mail mail = cmos.read_mail();
			if(mail.type == Thread::e_mailType::EXIT)
			{
				//	Stop the display driver
				m_displayDriver->stop();
				return;
			}
		}
		
		
		//	On-page-change action
		if(m_pageChangedActionNecessary == true)
		{
			for(auto& i: m_elements)
			{
				//	Call "onChangePageActual" function for all elements
				i->onChangePageActual();
				
				
				//	Request update for all elements on the current page
				//	Do this AFTER calling the "onChangePageActual" function
				//	This way elements like the PageSwitchButton can change their page and then show up on the new current page
				if(i->m_page == m_pageCurrent)
				{
					i->requestUpdate();
				}
			}
			
			
			//	Reset flag
			m_pageChangedActionNecessary = false;
		}
		
		
		//	Synchronize the framebuffers of all elements to avoid flickering
		for(auto& i: m_elements)
		{
			i->syncFramebuffers();
		}
		
		
		//	Element updates
		for(auto& i: m_elements)
		{
			if(i->m_visible == true && i->m_function_onUpdate != nullptr && i->m_page == m_pageCurrent)
			{
				bool updateNecessary = false;
				
				
				//	Manual update
				if(i->m_frameCounterAtLastUpdate == 0xFFFFFFFFFFFFFFFF)
				{
					updateNecessary = true;
				}
				else
				{
					//	Periodic update (disabled if "m_updatePeriodInFrames" is 0)
					if(i->m_updatePeriodInFrames > 0)
					{
						const uint64 framesSinceLastUpdate = frameCounter - i->m_frameCounterAtLastUpdate;
						if(framesSinceLastUpdate >= i->m_updatePeriodInFrames)
						{
							updateNecessary = true;
						}
					}
				}
				
				
				//	Execute update function if necessary
				if(updateNecessary == true)
				{
					i->onUpdate();
					
					
					//	Update "m_frameCounterAtLastUpdate" to current frame counter to be able to calculate the time for the next periodic update
					//	DONT DO THIS BEFORE THE "onUpdate" FUNCTION, because the update function might request an update
					//	by calling "requestUpdate", which sets "m_frameCounterAtLastUpdate" to 0xFFFFFFFFFFFFFFFF.
					//	If we set it to the current frame counter before the update function, the element would permanently update itself every frame
					i->m_frameCounterAtLastUpdate = frameCounter;
				}
			}
		}
		
		
		//	Swap front and back buffer
		m_displayDriver->set_layerBuffer(0, m_frameBufferData[m_frameBufferIndex]);
		
		
		//	Sleep until vertical sync
		cmos.event_listen(eventID_vSync);
		cmos.sleep_untilEvent(eventID_vSync);
		
		
		//	Update frame buffer index for next frame
		m_frameBufferIndex = 1 - m_frameBufferIndex;
	}
}



/*****************************************************************************/
/*                      						Public	  			 						 						 */
/*****************************************************************************/

feedback Graphics::init(I_GraphicAccelerator& graphicAccelerator, I_DisplayDriver& displayDriver, Rect frameShape, Color* frameBuffer1, Color* frameBuffer2)
{
	//	Check if desired frame shape fits into display dimensions
	const Vec2 displaySize = displayDriver.get_displayDimensions();
	Rect displayRect(Vec2(0, 0), displaySize);
	if(displayRect.contains(frameShape) == false)
	{
		return(FAIL);
	}
	
	
	//	Check if frame buffers are not nullpointers
	if(frameBuffer1 == nullptr || frameBuffer2 == nullptr)
	{
		return(FAIL);
	}
	
	
	//	Member Initialization
	m_graphicAccelerator					= &graphicAccelerator;
	m_displayDriver								= &displayDriver;
	
	m_frameBufferShape						= frameShape;
	m_frameBufferData[0]					= frameBuffer1;
	m_frameBufferData[1]					= frameBuffer2;
	
	Element::m_graphicAccelerator	= m_graphicAccelerator;
	
	
	//	Create Graphics Manager Thread
	CMOS& cmos = CMOS::get();
	const uint8 threadID = cmos.thread_create(&Graphics::manager, this, "Graphics Manager", 10, 8192);
	cmos.sleep_untilMail(threadID);
	
	return(OK);
}


Graphics& Graphics::get()
{
	static Graphics graphics;
	return(graphics);
}







uint8 Graphics::add_page()
{
	//	Lock semaphore for pages information
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_lock(&m_numberOfPages) != OK)
	{
		return(0xFF);
	}
	
	
	//	Check for maximum number of pages
	if(m_numberOfPages >= 0xFF)
	{
		cmos.semaphore_unlock(&m_numberOfPages);
		return(0xFF);
	}
	
	
	//	Add page and return page number
	const uint8 pageNumber = m_numberOfPages;
	m_numberOfPages++;
	cmos.semaphore_unlock(&m_numberOfPages);
	return(pageNumber);
}


feedback Graphics::remove_page(uint8 pageNumber)
{
	//	Check if there are any pages at all (minimum one page has to exist)
	if(m_numberOfPages < 2 || pageNumber >= m_numberOfPages)
	{
		return(FAIL);
	}
	
	
	//	Check if the page to remove is the current page
	//	If so, set current page to the previous page (except for page 0, then set it to page 1)
	if(pageNumber == m_pageCurrent)
	{
		if(pageNumber == 0)
		{
			set_currentPage(1);
		}
		else
		{
			set_currentPage(pageNumber - 1);
		}
	}
	
	
	//	Wait for the page change to be finished
	CMOS& cmos = CMOS::get();
	while(m_pageChangedActionNecessary == true)
	{
		#if defined(CORTEX_M7)
			cmos.sleep_100us(1);
		#endif
		
		#if defined(CORTEX_M0) || defined(CORTEX_M0P) || defined(CORTEX_M3) || defined(CORTEX_M4)
			cmos.sleep_ms(1);
		#endif
	}
	
	
	//	Lock semaphore for pages information
	if(cmos.semaphore_lock(&m_numberOfPages) != OK)
	{
		return(FAIL);
	}
	
	
	//	We first need to erase all elements on this page
	for(auto& i: m_elements)
	{
		if(i->m_page == pageNumber)
		{
			operator-=(*i);
		}
	}
	
	
	//	Erase page and update page numbers of all elements on higher pages
	for(auto& i: m_elements)
	{
		if(i->m_page > pageNumber)
		{
			i->m_page--;
		}
	}
	
	
	//	Erase page
	m_numberOfPages--;
	
	
	//	Unlock semaphore for pages information
	cmos.semaphore_unlock(&m_numberOfPages);
	return(OK);
}







void Graphics::eraseAllElements()
{
	//	Dont optimize this Code here, because we modify the Array "m_elements" in the "operator()" Function
	//	This leads to incorrect Function if not done like this here!
	while(m_elements.get_size() > 0)
	{
		operator-=(*m_elements[0]);
	}
}







void Graphics::register_touchData(Vec2 touchPoint, I_TouchController::e_touchEvent touchEvent)
{
	if(touchEvent != I_TouchController::e_touchEvent::INVALID)
	{
		//	Make the new touch event visible to all elements
		Element::m_touchEvent = touchEvent;
		
		
		//	Determine which element has been touched
		for(auto& i: m_elements)
		{
			if(i->m_page == m_pageCurrent && i->m_touchable == true && i->m_visible == true)
			{
				const Vec2 touchPoint_relativeToElement(touchPoint - i->position);
				if(i->containsPoint(touchPoint_relativeToElement) == true)
				{
					i->m_touchValid = true;
					if(i->m_function_onCallback != nullptr)
					{
						i->m_touchPosition = touchPoint_relativeToElement;
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
					return;
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
}







Graphics& Graphics::operator+=(Element& element)
{
	//	Check page
	if(element.m_page >= m_numberOfPages)
	{
		return(*this);
	}
	
	
	//	Check minimum dimensions
	if(element.size.x < Element::c_minimumSideLength || element.size.y < Element::c_minimumSideLength)
	{
		return(*this);
	}
	
	
	//	Check if the element fits into the framebuffer's dimensions
	if(element.position.x < 0 || element.position.y < 0)
	{
		return(*this);
	}
	
	const Vec2 topRight(element.get_topRightCorner());
	if(topRight.x >= m_frameBufferShape.size.x || topRight.y >= m_frameBufferShape.size.y)
	{
		return(*this);
	}
	
	
	//	Check overlap with exisiting elements
	for(auto& i: m_elements)
	{
		if(i->m_page == element.m_page)
		{
			if(i->doesOverlap(element) == true)
			{
				return(*this);
			}
		}
	}
	
	
	//	Create semaphore for the new element
	//	This is needed for the element to be able to lock itself on update and callback Function
	CMOS& cmos = CMOS::get();
	if(cmos.semaphore_create(&element) != OK)
	{
		return(*this);
	}
	
	
	//	Initially request update for this element
	element.requestRebuild();
	element.requestUpdate();
	
	
	//	Add element to array
	m_elements += &element;
	return(*this);
}


Graphics& Graphics::operator-=(Element& element)
{
	const uint32 index = m_elements.find(&element);
	if(m_elements.is_valid(index) == true)
	{
		//	Erase element from array
		m_elements.erase(index);
		
		
		//	Set callback function to nullptr to avoid callback execution after the element has been erased
		element.set_function_onCallback(nullptr);
		
		
		//	Sleep shortly to be sure, that no update function is running for this element
		CMOS& cmos = CMOS::get();
		cmos.sleep_ms(10);
		
		
		//	Lock semaphore for this element
		if(cmos.semaphore_lock(&element) != OK)
		{
			return(*this);
		}
		
		
		//	Clear element from both framebuffers
		element.clearFromBothFramebuffers();
		
		
		//	Erase semaphore of this element
		if(cmos.semaphore_erase(&element) != OK)
		{
			return(*this);
		}
	}
	return(*this);
}