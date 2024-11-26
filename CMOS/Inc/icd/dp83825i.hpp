#pragma once

#include "../defines.hpp"
#include "../network/ethernetPhy.hpp"
#include "../i_mdio.hpp"





namespace ICD
{
	class DP83825I: public EthernetPHY
	{
		public:
			
			
			
			
			
			
			
		private:
			
			//	Static Member
			
			
			
			//	Non-static Member
			I_MDIO& m_mdio;
			uint16 m_phyRegisters[numberOfPhyRegisters];
			
			
			//	Constructor and Destructor
			
			
			
			//	Member Functions
			void setBitToValue(uint16 address, uint8 bit, bool value);
			
			
			//	Friends
			
			
			
			
			
			
		public:
			
			DP83825I(I_MDIO& mdio);
			DP83825I(const DP83825I& dp83825i) = delete;
			~DP83825I();
			
			feedback readAllRegisters()																			override;
			
			feedback hardwareReset()																				override;
			feedback softwareResetAllPHYRegisterExceptVendorSpecific()			override;
			feedback softwareRestartPHYExceptRegisters()										override;
			
			feedback set_autoNegotiation(bool enable)												override;
			feedback restartAutoNegotiation()																override;
			feedback powerDown(bool enable)																	override;
			feedback set_speed(e_speed speed)																override;
			feedback invertPolartiyOnBothRxAndTx(bool enable)								override;
			feedback swapRxAndTxPairs(bool enable)													override;
			feedback setInterruptPinPolarity(bool activeLow)								override;
			feedback interruptPinAsPowerDownInputPin(bool enable)						override;
			feedback setRMIIReferenceClock(e_RMIIReferenceClock clock)			override;
			feedback setRMIIRevision(e_RMIIRevision revision)								override;
			feedback setLEDBlinkRate(e_ledBlinkRate blinkRate)							override;
			feedback setLEDPolarity(bool activeHigh)												override;
			feedback setLEDModeToAutomatic()																override;
			feedback setLEDModeToManual()																		override;
			feedback setLEDState(bool enable)																override;
			feedback setJabberDetection(bool enable)												override;

			feedback interruptsEnable(bool enable)													override;
			feedback interruptLinkQualityIndication(bool enable)						override;
			feedback interruptEnergyDetectIndication(bool enable)						override;
			feedback interruptLinkStatus(bool enable)												override;
			feedback interruptSpeedStatus(bool enable)											override;
			feedback interruptDuplexStatus(bool enable)											override;
			feedback interruptAutoNegotiationCompleted(bool enable)					override;
			feedback interruptFalseCarrierCounterHalfFull(bool enable)			override;
			feedback interruptReceiveErrorCounterHalfFull(bool enable)			override;
			feedback interruptEnergyEfficientEthernet(bool enable)					override;
			feedback interruptAutoNegotiationError(bool enable)							override;
			feedback interruptPageReceived(bool enable)											override;
			feedback interruptLoopbackFIFOOverflowUnderflow(bool enable)		override;
			feedback interruptMDICrossoverStatusChange(bool enable)					override;
			feedback interruptSleepModeEvent(bool enable)										override;
			feedback interruptInvertedPolarityOrWakeOnLAN(bool enable)			override;
			feedback interruptJabberDetectEvent(bool enable)								override;

			bool isInterruptPending() const																	override;
			bool wasInterruptLinkQualityIndication() const									override;
			bool wasInterruptEnergyDetectIndication() const									override;
			bool wasInterruptLinkStatus() const															override;
			bool wasInterruptSpeedStatus() const														override;
			bool wasInterruptDuplexStatus() const														override;
			bool wasInterruptAutoNegotiationCompleted() const								override;
			bool wasInterruptFalseCarrierCounterHalfFull() const						override;
			bool wasInterruptReceiveErrorCounterHalfFull() const						override;
			bool wasInterruptEnergyEfficientEthernet() const								override;
			bool wasInterruptAutoNegotiationError() const										override;
			bool wasInterruptPageReceived() const														override;
			bool wasInterruptLoopbackFIFOOverflowUnderflow() const					override;
			bool wasInterruptMDICrossoverStatusChange() const								override;
			bool wasInterruptSleepModeEvent() const													override;
			bool wasInterruptInvertedPolarityOrWakeOnLAN() const						override;
			bool wasInterruptJabberDetectEvent() const											override;

			bool isInvertedPolarityOnRxAndTxDetected() const								override;
			bool isRxAndTxPairSwapDetected() const													override;
			bool wasErrorReceivedSinceLastReadOfErrorCounter() const				override;
			bool wasFalseCarrierDetectedSinceLastReadOfErrorCounter() const	override;
			bool wasRXFIFOOverflowDetected() const													override;
			bool wasRXFIFOUnderflowDetected() const													override;

			e_duplexMode getDuplexMode() const															override;
			e_speed getSpeed() const																				override;
			uint8 getFalseCarrierCounter() const														override;
			uint8 getReceiveErrorCounter() const														override;
			e_RMIIReferenceClock getRMIIReferenceClock() const							override;
			e_RMIIRevision getRMIIRevision() const													override;
			e_ledBlinkRate getLEDBlinkRate() const													override;
			bool isLEDPolarityActiveHigh() const														override;
			bool isLEDModeAutomatic() const																	override;
			bool getLEDState() const																				override;
			uint8 getPHYAddress() const																			override;
			bool isJabberDetectionEnabled() const														override;

			bool isAutoNegotiationComplete() const													override;
			bool isLinkUp() const																						override;
			bool isRemoteFaultDetected() const															override;
			bool isJabberDetected() const																		override;
			bool isExtendedRegistersCapable() const													override;

			bool isPhyAutoNegotiationEnabled() const												override;
			bool isPhyPowerDown() const																			override;
			bool isPhy100BaseT4Capable() const															override;
			bool isPhy10BaseTHalfDuplexCapable() const											override;
			bool isPhy10BaseTFullDuplexCapable() const											override;
			bool isPhy100BaseTHalfDuplexCapable() const											override;
			bool isPhy100BaseTFullDuplexCapable() const											override;
			bool isPhySMIPreambleSuppressionCapable() const									override;
			uint8 getPhyProtocolID() const																	override;

			bool doesLinkPartnerDesireNextPageTransfer() const							override;
			bool doesLinkPartnerAcknowledgeReceiptionOfLinkCodeWord() const	override;
			bool doesLinkPartnerAdvertiseRemoteFaultDetection() const				override;
			bool doesLinkPartnerAdvertiseAsymmetricalPauseAbility() const		override;
			bool doesLinkPartnerAdvertisePauseAbility() const								override;
			bool doesLinkPartnerAdvertise100BaseT4() const									override;
			bool doesLinkPartnerAdvertise100BaseTXFullDuplex() const				override;
			bool doesLinkPartnerAdvertise100BaseTXHalfDuplex() const				override;
			bool doesLinkPartnerAdvertise10BaseTFullDuplex() const					override;
			bool doesLinkPartnerAdvertise10BaseTHalfDuplex() const					override;
			bool doesLinkPartnerSupportAutoNegotiation() const							override;
			uint8 getLinkPartnerProtocolID() const													override;

			uint32 getPhyID() const																					override;
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
	
	
};