#include "../../Inc/cmos.hpp"





namespace ICD
{
	/*****************************************************************************/
	/*                    Globals and Static Initialization					 						 */
	/*****************************************************************************/
	
	
	
	
	
	
	/*****************************************************************************/
	/*                      						Private	  			 						 						 */
	/*****************************************************************************/
	
	void DP83825I::setBitToValue(uint16 address, uint8 bit, bool value)
	{
		uint16 reg = m_mdio.readRegister(address);
		if(value == true)
		{
			bit::set(reg, bit);
		}
		else
		{
			bit::clear(reg, bit);
		}
		m_mdio.writeRegister(address, reg);
	}
	
	
	
	/*****************************************************************************/
	/*                      						Public	  			 						 						 */
	/*****************************************************************************/
	
	DP83825I::DP83825I(I_MDIO& mdio)
		:	m_mdio(mdio)
	{
		
	}
	
	
	DP83825I::~DP83825I()
	{
		
	}
	
	
	
	
	
	
	
	feedback DP83825I::readAllRegisters()
	{
		for(uint32 i = 0; i < numberOfPhyRegisters; i++)
		{
			m_phyRegisters[i] = m_mdio.readRegister(i);
		}
		return(OK);
	}
	
	
	
	
	
	
	
	feedback DP83825I::hardwareReset()
	{
		setBitToValue(0x1F, 15, true);
		while(bit::isSet(m_mdio.readRegister(0x1F), 15) == true)
		{
			//	Wait for reset to complete
			CMOS::get().sleep_ms(1);
		}
		return(readAllRegisters());
	}
	
	
	feedback DP83825I::softwareResetAllPHYRegisterExceptVendorSpecific()
	{
		setBitToValue(0x00, 15, true);
		while(bit::isSet(m_mdio.readRegister(0x0), 15) == true)
		{
			//	Wait for reset to complete
			CMOS::get().sleep_ms(1);
		}
		return(readAllRegisters());
	}
	
	
	feedback DP83825I::softwareRestartPHYExceptRegisters()
	{
		setBitToValue(0x1F, 14, true);
		while(bit::isSet(m_mdio.readRegister(0x1F), 14) == true)
		{
			//	Wait for reset to complete
			CMOS::get().sleep_ms(1);
		}
		return(readAllRegisters());
	}
	
	
	
	
	
	
	
	feedback DP83825I::set_autoNegotiation(bool enable)
	{
		setBitToValue(0x00, 12, enable);
		return(OK);
	}
	
	
	feedback DP83825I::restartAutoNegotiation()
	{
		setBitToValue(0x00, 9, true);
		return(OK);
	}
	
	
	feedback DP83825I::powerDown(bool enable)
	{
		setBitToValue(0x00, 11, enable);
		return(OK);
	}
	
	
	feedback DP83825I::set_speed(e_speed speed)
	{
		switch(speed)
		{
			case e_speed::_10M:
			{
				setBitToValue(0x00, 13, false);
			}
			break;
			
			case e_speed::_100M:
			{
				setBitToValue(0x00, 13, true);
			}
			break;
			
			default:
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback DP83825I::invertPolartiyOnBothRxAndTx(bool enable)
	{
		setBitToValue(0x0B, 6, enable);
		return(OK);
	}
	
	
	feedback DP83825I::swapRxAndTxPairs(bool enable)
	{
		setBitToValue(0x0B, 5, enable);
		return(OK);
	}
	
	
	feedback DP83825I::setInterruptPinPolarity(bool activeLow)
	{
		setBitToValue(0x11, 3, activeLow);
		return(OK);
	}
	
	
	feedback DP83825I::interruptPinAsPowerDownInputPin(bool enable)
	{
		setBitToValue(0x11, 0, !enable);
		return(OK);
	}
	
	
	feedback DP83825I::setRMIIReferenceClock(e_RMIIReferenceClock clock)
	{
		switch(clock)
		{
			case e_RMIIReferenceClock::CRYSTAL_OR_CMOS_25MHZ:
			{
				setBitToValue(0x17, 7, false);
			}
			break;
			
			case e_RMIIReferenceClock::CMOS_50MHZ:
			{
				setBitToValue(0x17, 7, true);
			}
			break;
			
			default:
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback DP83825I::setRMIIRevision(e_RMIIRevision revision)
	{
		switch(revision)
		{
			case e_RMIIRevision::RMII_REV_1v0:
			{
				setBitToValue(0x17, 4, true);
			}
			break;
			
			case e_RMIIRevision::RMII_REV_1v2:
			{
				setBitToValue(0x17, 4, false);
			}
			break;
			
			default:
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback DP83825I::setLEDBlinkRate(e_ledBlinkRate blinkRate)
	{
		switch(blinkRate)
		{
			case e_ledBlinkRate::_2Hz:
			{
				setBitToValue(0x18,  9, true);
				setBitToValue(0x18, 10, true);
			}
			break;
			
			case e_ledBlinkRate::_5Hz:
			{
				setBitToValue(0x18,  9, false);
				setBitToValue(0x18, 10, true);
			}
			break;
			
			case e_ledBlinkRate::_10Hz:
			{
				setBitToValue(0x18,  9, true);
				setBitToValue(0x18, 10, false);
			}
			break;
			
			case e_ledBlinkRate::_20Hz:
			{
				setBitToValue(0x18,  9, false);
				setBitToValue(0x18, 10, false);
			}
			break;
			
			default:
			{
				return(FAIL);
			}
		}
		return(OK);
	}
	
	
	feedback DP83825I::setLEDPolarity(bool activeHigh)
	{
		setBitToValue(0x18, 7, activeHigh);
		return(OK);
	}
	
	
	feedback DP83825I::setLEDModeToAutomatic()
	{
		setBitToValue(0x18, 4, false);
		return(OK);
	}
	
	
	feedback DP83825I::setLEDModeToManual()
	{
		setBitToValue(0x18, 4, true);
		return(OK);
	}
	
	
	feedback DP83825I::setLEDState(bool enable)
	{
		setBitToValue(0x18, 1, enable);
		return(OK);
	}
	
	
	feedback DP83825I::setJabberDetection(bool enable)
	{
		setBitToValue(0x1A, 0, !enable);
		return(OK);
	}
	
	
	
	
	
	
	
	feedback DP83825I::interruptsEnable(bool enable)
	{
		setBitToValue(0x11, 1, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptLinkQualityIndication(bool enable)
	{
		setBitToValue(0x12, 7, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptEnergyDetectIndication(bool enable)
	{
		setBitToValue(0x12, 6, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptLinkStatus(bool enable)
	{
		setBitToValue(0x12, 5, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptSpeedStatus(bool enable)
	{
		setBitToValue(0x12, 4, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptDuplexStatus(bool enable)
	{
		setBitToValue(0x12, 3, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptAutoNegotiationCompleted(bool enable)
	{
		setBitToValue(0x12, 2, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptFalseCarrierCounterHalfFull(bool enable)
	{
		setBitToValue(0x12, 1, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptReceiveErrorCounterHalfFull(bool enable)
	{
		setBitToValue(0x12, 0, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptEnergyEfficientEthernet(bool enable)
	{
		setBitToValue(0x13, 7, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptAutoNegotiationError(bool enable)
	{
		setBitToValue(0x13, 6, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptPageReceived(bool enable)
	{
		setBitToValue(0x13, 5, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptLoopbackFIFOOverflowUnderflow(bool enable)
	{
		setBitToValue(0x13, 4, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptMDICrossoverStatusChange(bool enable)
	{
		setBitToValue(0x13, 3, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptSleepModeEvent(bool enable)
	{
		setBitToValue(0x13, 2, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptInvertedPolarityOrWakeOnLAN(bool enable)
	{
		setBitToValue(0x13, 1, enable);
		return(OK);
	}
	
	
	feedback DP83825I::interruptJabberDetectEvent(bool enable)
	{
		setBitToValue(0x13, 0, enable);
		return(OK);
	}
	
	
	
	
	
	
	
	bool DP83825I::isInterruptPending() const
	{
		return(bit::isSet(m_phyRegisters[0x10], 7));
	}
	
	
	bool DP83825I::wasInterruptLinkQualityIndication() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 15));
	}
	
	
	bool DP83825I::wasInterruptEnergyDetectIndication() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 14));
	}
	
	
	bool DP83825I::wasInterruptLinkStatus() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 13));
	}
	
	
	bool DP83825I::wasInterruptSpeedStatus() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 12));
	}
	
	
	bool DP83825I::wasInterruptDuplexStatus() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 11));
	}
	
	
	bool DP83825I::wasInterruptAutoNegotiationCompleted() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 10));
	}
	
	
	bool DP83825I::wasInterruptFalseCarrierCounterHalfFull() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 9));
	}
	
	
	bool DP83825I::wasInterruptReceiveErrorCounterHalfFull() const
	{
		return(bit::isSet(m_phyRegisters[0x12], 8));
	}
	
	
	bool DP83825I::wasInterruptEnergyEfficientEthernet() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 15));
	}
	
	
	bool DP83825I::wasInterruptAutoNegotiationError() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 14));
	}
	
	
	bool DP83825I::wasInterruptPageReceived() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 13));
	}
	
	
	bool DP83825I::wasInterruptLoopbackFIFOOverflowUnderflow() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 12));
	}
	
	
	bool DP83825I::wasInterruptMDICrossoverStatusChange() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 11));
	}
	
	
	bool DP83825I::wasInterruptSleepModeEvent() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 10));
	}
	
	
	bool DP83825I::wasInterruptInvertedPolarityOrWakeOnLAN() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 9));
	}
	
	
	bool DP83825I::wasInterruptJabberDetectEvent() const
	{
		return(bit::isSet(m_phyRegisters[0x13], 8));
	}
	
	
	
	
	
	
	
	bool DP83825I::isInvertedPolarityOnRxAndTxDetected() const
	{
		return(bit::isSet(m_phyRegisters[0x10], 12));
	}
	
	
	bool DP83825I::isRxAndTxPairSwapDetected() const
	{
		return(bit::isSet(m_phyRegisters[0x10], 14));
	}
	
	
	bool DP83825I::wasErrorReceivedSinceLastReadOfErrorCounter() const
	{
		return(bit::isSet(m_phyRegisters[0x10], 13));
	}
	
	
	bool DP83825I::wasFalseCarrierDetectedSinceLastReadOfErrorCounter() const
	{
		return(bit::isSet(m_phyRegisters[0x10], 11));
	}
	
	
	bool DP83825I::wasRXFIFOOverflowDetected() const
	{
		return(bit::isSet(m_phyRegisters[0x17], 3));
	}
	
	
	bool DP83825I::wasRXFIFOUnderflowDetected() const
	{
		return(bit::isSet(m_phyRegisters[0x17], 2));
	}
	
	
	
	
	
	
	
	DP83825I::e_duplexMode DP83825I::getDuplexMode() const
	{
		if(bit::isSet(m_phyRegisters[0x10], 2) == true)
		{
			return(e_duplexMode::FULL);
		}
		return(e_duplexMode::HALF);
	}
	
	
	DP83825I::e_speed DP83825I::getSpeed() const
	{
		if(bit::isSet(m_phyRegisters[0x10], 1) == true)
		{
			return(e_speed::_10M);
		}
		return(e_speed::_100M);
	}
	
	
	uint8 DP83825I::getFalseCarrierCounter() const
	{
		return(m_phyRegisters[0x14] & 0x00FF);
	}
	
	
	uint8 DP83825I::getReceiveErrorCounter() const
	{
		return(m_phyRegisters[0x15]);
	}
	
	
	DP83825I::e_RMIIReferenceClock DP83825I::getRMIIReferenceClock() const
	{
		if(bit::isSet(m_phyRegisters[0x17], 7) == true)
		{
			return(e_RMIIReferenceClock::CMOS_50MHZ);
		}
		return(e_RMIIReferenceClock::CRYSTAL_OR_CMOS_25MHZ);
	}
	
	
	DP83825I::e_RMIIRevision DP83825I::getRMIIRevision() const
	{
		if(bit::isSet(m_phyRegisters[0x17], 4) == true)
		{
			return(e_RMIIRevision::RMII_REV_1v0);
		}
		return(e_RMIIRevision::RMII_REV_1v2);
	}
	
	
	DP83825I::e_ledBlinkRate DP83825I::getLEDBlinkRate() const
	{
		if(bit::isSet(m_phyRegisters[0x18], 9) == true)
		{
			if(bit::isSet(m_phyRegisters[0x18], 10) == true)
			{
				return(e_ledBlinkRate::_2Hz);
			}
			return(e_ledBlinkRate::_10Hz);
		}
		if(bit::isSet(m_phyRegisters[0x18], 10) == true)
		{
			return(e_ledBlinkRate::_5Hz);
		}
		return(e_ledBlinkRate::_20Hz);
	}
	
	
	bool DP83825I::isLEDPolarityActiveHigh() const
	{
		return(bit::isSet(m_phyRegisters[0x18], 7));
	}
	
	
	bool DP83825I::isLEDModeAutomatic() const
	{
		return(!bit::isSet(m_phyRegisters[0x18], 4));
	}
	
	
	bool DP83825I::getLEDState() const
	{
		return(bit::isSet(m_phyRegisters[0x18], 1));
	}
	
	
	uint8 DP83825I::getPHYAddress() const
	{
		return(m_phyRegisters[0x19] & 0x001F);
	}
	
	
	bool DP83825I::isJabberDetectionEnabled() const
	{
		return(bit::isCleared(m_phyRegisters[0x1A], 0));
	}
	
	
	
	
	
	
	
	bool DP83825I::isAutoNegotiationComplete() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 5));
	}
	
	
	bool DP83825I::isLinkUp() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 2));
	}
	
	
	bool DP83825I::isRemoteFaultDetected() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 4));
	}
	
	
	bool DP83825I::isJabberDetected() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 1));
	}
	
	
	bool DP83825I::isExtendedRegistersCapable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 0));
	}
	
	
	
	
	
	
	
	bool DP83825I::isPhyAutoNegotiationEnabled() const
	{
		return(bit::isSet(m_phyRegisters[0x00], 12));
	}
	
	
	bool DP83825I::isPhyPowerDown() const
	{
		return(bit::isSet(m_phyRegisters[0x00], 11));
	}
	
	
	bool DP83825I::isPhy100BaseT4Capable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 15));
	}
	
	
	bool DP83825I::isPhy10BaseTHalfDuplexCapable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 11));
	}
	
	
	bool DP83825I::isPhy10BaseTFullDuplexCapable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 12));
	}
	
	
	bool DP83825I::isPhy100BaseTHalfDuplexCapable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 13));
	}
	
	
	bool DP83825I::isPhy100BaseTFullDuplexCapable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 14));
	}
	
	
	bool DP83825I::isPhySMIPreambleSuppressionCapable() const
	{
		return(bit::isSet(m_phyRegisters[0x01], 6));
	}
	
	
	uint8 DP83825I::getPhyProtocolID() const
	{
		return(m_phyRegisters[0x04] & 0x001F);
	}
	
	
	
	
	
	
	
	bool DP83825I::doesLinkPartnerDesireNextPageTransfer() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 15));
	}
	
	
	bool DP83825I::doesLinkPartnerAcknowledgeReceiptionOfLinkCodeWord() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 14));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertiseRemoteFaultDetection() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 13));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertiseAsymmetricalPauseAbility() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 11));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertisePauseAbility() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 10));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertise100BaseT4() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 9));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertise100BaseTXFullDuplex() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 8));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertise100BaseTXHalfDuplex() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 7));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertise10BaseTFullDuplex() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 6));
	}
	
	
	bool DP83825I::doesLinkPartnerAdvertise10BaseTHalfDuplex() const
	{
		return(bit::isSet(m_phyRegisters[0x05], 5));
	}
	
	
	bool DP83825I::doesLinkPartnerSupportAutoNegotiation() const
	{
		return(bit::isSet(m_phyRegisters[0x06], 0));
	}
	
	
	uint8 DP83825I::getLinkPartnerProtocolID() const
	{
		return(m_phyRegisters[0x05] & 0x001F);
	}
	
	
	
	
	
	
	
	uint32 DP83825I::getPhyID() const
	{
		return((m_phyRegisters[0x02] << 16) | m_phyRegisters[0x03]);
	}
};