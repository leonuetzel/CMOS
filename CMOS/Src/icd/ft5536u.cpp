#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	uint8 FT5536U::register_read(REG address) const
	{
		//	Send Start Condition
		if(m_i2c.start(slaveAddress, true, 1) != OK)
		{
			return(0);
		}
		
		
		//	Transmit Register Address
		if(m_i2c.tx((uint8) address) != OK)
		{
			return(0);
		}
		
		
		//	Readout Register
		if(m_i2c.start(slaveAddress, false, 1) != OK)
		{
			return(0);
		}
		return(m_i2c.rx());
	}
	
	
	feedback FT5536U::register_write(REG address, uint8 data)
	{
		//	Send Start Condition
		if(m_i2c.start(slaveAddress, true, 2) != OK)
		{
			return(FAIL);
		}
		
		
		//	Transmit Register Address
		if(m_i2c.tx((uint8) address) != OK)
		{
			return(FAIL);
		}
		
		
		//	Transmit Data
		return(m_i2c.tx(data));
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	FT5536U::FT5536U(I_I2C& i2c)
		:	m_i2c(i2c)
	{
		
	}
	
	
	FT5536U::~FT5536U()
	{
		
	}
	
	
	
	
	
	
	
	I_TouchController::e_touchGuesture FT5536U::getTouchGuesture()
	{
		const uint8 GUESTURE = register_read(REG::GUESTURE);
		switch(GUESTURE)
		{
			case 0x00:
			{
				return(e_touchGuesture::NONE);
			}
			break;
			
			case 0x10:
			{
				return(e_touchGuesture::MOVE_UP);
			}
			break;
			
			case 0x14:
			{
				return(e_touchGuesture::MOVE_RIGHT);
			}
			break;
			
			case 0x18:
			{
				return(e_touchGuesture::MOVE_DOWN);
			}
			break;
			
			case 0x1C:
			{
				return(e_touchGuesture::MOVE_LEFT);
			}
			break;
			
			case 0x48:
			{
				return(e_touchGuesture::ZOOM_IN);
			}
			break;
			
			case 0x49:
			{
				return(e_touchGuesture::ZOOM_OUT);
			}
			break;
			
			default:
			{
				return(e_touchGuesture::INVALID);
			}
			break;
		}
		return(e_touchGuesture::INVALID);
	}
	
	
	uint8 FT5536U::getNumberOfTouchPoints()
	{
		const uint8 CUR_POINT = register_read(REG::CUR_POINT);
		if(CUR_POINT > maximumNumberOfTouchPoints)
		{
			return(0);
		}
		return(CUR_POINT);
	}
	
	
	Pair<Vec2, I_TouchController::e_touchEvent> FT5536U::getTouchPointInformation(uint8 index)
	{
		Pair<Vec2, e_touchEvent> touchPointInformation;
		touchPointInformation.first() = Vec2(0, 0);
		touchPointInformation.second() = e_touchEvent::INVALID;
		
		if(index >= maximumNumberOfTouchPoints)
		{
			return(touchPointInformation);
		}
		
		
		//	Read Touch Data from Touch Controller
		//	Set Read Address
		if(m_i2c.start(slaveAddress, true, 1) != OK)
		{
			return(touchPointInformation);
		}
		m_i2c.tx((uint8) (((uint8) REG::TOUCH_0_X_HIGH) + (index * 6)));
		
		
		//	Setup Read from Touch Controller
		if(m_i2c.start(slaveAddress, false, 4) != OK)
		{
			return(touchPointInformation);
		}
		
		
		//	Read Data
		uint8 data[4];
		for(auto& i: data)
		{
			i = m_i2c.rx();
		}
		
		
		//	Read Touch Event
		const uint8 event = (data[0] >> 6) & 0x03;
		switch(event)
		{
			case 0x00:
			{
				touchPointInformation.second() = e_touchEvent::TOUCH;
			}
			break;
			
			case 0x01:
			{
				touchPointInformation.second() = e_touchEvent::RELEASE;
			}
			break;
			
			case 0x02:
			{
				touchPointInformation.second() = e_touchEvent::CONTACT;
			}
			break;
			
			default:
			{
				touchPointInformation.second() = e_touchEvent::INVALID;
			}
			break;
		}
		
		
		//	Read Touch Coordinates
		touchPointInformation.first().x = ((data[0] & 0x0F) << 8) | data[1];
		touchPointInformation.first().y = ((data[2] & 0x0F) << 8) | data[3];
		return(touchPointInformation);
	}
};