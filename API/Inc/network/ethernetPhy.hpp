#pragma once

#include "../cmos.hpp"





class EthernetPHY
{
	public:
		
		enum class e_speed
		{
			_10M,
			_100M
		};
		
		enum class e_duplexMode
		{
			HALF,
			FULL
		};
		
		enum class e_RMIIReferenceClock
		{
			CRYSTAL_OR_CMOS_25MHZ,
			CMOS_50MHZ
		};
		
		enum class e_RMIIRevision
		{
			RMII_REV_1v0,
			RMII_REV_1v2
		};
		
		enum class e_ledBlinkRate
		{
			_2Hz,
			_5Hz,
			_10Hz,
			_20Hz
		};
		
		
		
		
		
	private:
		
		//	Static Member
		
		
		
		//	Non-static Member
		
		
		
		//	Constructor and Destructor
		
		
		
		//	Member Functions
		
		
		
		//	Friends
		
		
		
		
		
		
	protected:
		
		static constexpr uint32 numberOfPhyRegisters = 32;
		
		
		
		
		
	public:
		
		virtual feedback readAllRegisters()																			= 0;

		virtual feedback hardwareReset()																				= 0;
		virtual feedback softwareResetAllPHYRegisterExceptVendorSpecific()			= 0;
		virtual feedback softwareRestartPHYExceptRegisters()										= 0;
		
		virtual feedback set_autoNegotiation(bool enable)												= 0;
		virtual feedback restartAutoNegotiation()																= 0;
		virtual feedback powerDown(bool enable)																	= 0;
		virtual feedback set_speed(e_speed speed)																= 0;
		virtual feedback invertPolartiyOnBothRxAndTx(bool enable)								= 0;
		virtual feedback swapRxAndTxPairs(bool enable)													= 0;
		virtual feedback setInterruptPinPolarity(bool activeLow)								= 0;
		virtual feedback interruptPinAsPowerDownInputPin(bool enable)						= 0;
		virtual feedback setRMIIReferenceClock(e_RMIIReferenceClock clock)			= 0;
		virtual feedback setRMIIRevision(e_RMIIRevision revision)								= 0;
		virtual feedback setLEDBlinkRate(e_ledBlinkRate blinkRate)							= 0;
		virtual feedback setLEDPolarity(bool activeHigh)												= 0;
		virtual feedback setLEDModeToAutomatic()																= 0;
		virtual feedback setLEDModeToManual()																		= 0;
		virtual feedback setLEDState(bool enable)																= 0;
		virtual feedback setJabberDetection(bool enable)												= 0;

		virtual feedback interruptsEnable(bool enable)													= 0;
		virtual feedback interruptLinkQualityIndication(bool enable)						= 0;
		virtual feedback interruptEnergyDetectIndication(bool enable)						= 0;
		virtual feedback interruptLinkStatus(bool enable)												= 0;
		virtual feedback interruptSpeedStatus(bool enable)											= 0;
		virtual feedback interruptDuplexStatus(bool enable)											= 0;
		virtual feedback interruptAutoNegotiationCompleted(bool enable)					= 0;
		virtual feedback interruptFalseCarrierCounterHalfFull(bool enable)			= 0;
		virtual feedback interruptReceiveErrorCounterHalfFull(bool enable)			= 0;
		virtual feedback interruptEnergyEfficientEthernet(bool enable)					= 0;
		virtual feedback interruptAutoNegotiationError(bool enable)							= 0;
		virtual feedback interruptPageReceived(bool enable)											= 0;
		virtual feedback interruptLoopbackFIFOOverflowUnderflow(bool enable)		= 0;
		virtual feedback interruptMDICrossoverStatusChange(bool enable)					= 0;
		virtual feedback interruptSleepModeEvent(bool enable)										= 0;
		virtual feedback interruptInvertedPolarityOrWakeOnLAN(bool enable)			= 0;
		virtual feedback interruptJabberDetectEvent(bool enable)								= 0;

		virtual bool isInterruptPending() const																	= 0;
		virtual bool wasInterruptLinkQualityIndication() const									= 0;
		virtual bool wasInterruptEnergyDetectIndication() const									= 0;
		virtual bool wasInterruptLinkStatus() const															= 0;
		virtual bool wasInterruptSpeedStatus() const														= 0;
		virtual bool wasInterruptDuplexStatus() const														= 0;
		virtual bool wasInterruptAutoNegotiationCompleted() const								= 0;
		virtual bool wasInterruptFalseCarrierCounterHalfFull() const						= 0;
		virtual bool wasInterruptReceiveErrorCounterHalfFull() const						= 0;
		virtual bool wasInterruptEnergyEfficientEthernet() const								= 0;
		virtual bool wasInterruptAutoNegotiationError() const										= 0;
		virtual bool wasInterruptPageReceived() const														= 0;
		virtual bool wasInterruptLoopbackFIFOOverflowUnderflow() const					= 0;
		virtual bool wasInterruptMDICrossoverStatusChange() const								= 0;
		virtual bool wasInterruptSleepModeEvent() const													= 0;
		virtual bool wasInterruptInvertedPolarityOrWakeOnLAN() const						= 0;
		virtual bool wasInterruptJabberDetectEvent() const											= 0;

		virtual bool isInvertedPolarityOnRxAndTxDetected() const								= 0;
		virtual bool isRxAndTxPairSwapDetected() const													= 0;
		virtual bool wasErrorReceivedSinceLastReadOfErrorCounter() const				= 0;
		virtual bool wasFalseCarrierDetectedSinceLastReadOfErrorCounter() const	= 0;
		virtual bool wasRXFIFOOverflowDetected() const													= 0;
		virtual bool wasRXFIFOUnderflowDetected() const													= 0;

		virtual e_duplexMode getDuplexMode() const															= 0;
		virtual e_speed getSpeed() const																				= 0;
		virtual uint8 getFalseCarrierCounter() const														= 0;
		virtual uint8 getReceiveErrorCounter() const														= 0;
		virtual e_RMIIReferenceClock getRMIIReferenceClock() const							= 0;
		virtual e_RMIIRevision getRMIIRevision() const													= 0;
		virtual e_ledBlinkRate getLEDBlinkRate() const													= 0;
		virtual bool isLEDPolarityActiveHigh() const														= 0;
		virtual bool isLEDModeAutomatic() const																	= 0;
		virtual bool getLEDState() const																				= 0;
		virtual uint8 getPHYAddress() const																			= 0;
		virtual bool isJabberDetectionEnabled() const														= 0;

		virtual bool isAutoNegotiationComplete() const													= 0;
		virtual bool isLinkUp() const																						= 0;
		virtual bool isRemoteFaultDetected() const															= 0;
		virtual bool isJabberDetected() const																		= 0;
		virtual bool isExtendedRegistersCapable() const													= 0;

		virtual bool isPhyAutoNegotiationEnabled() const												= 0;
		virtual bool isPhyPowerDown() const																			= 0;
		virtual bool isPhy100BaseT4Capable() const															= 0;
		virtual bool isPhy10BaseTHalfDuplexCapable() const											= 0;
		virtual bool isPhy10BaseTFullDuplexCapable() const											= 0;
		virtual bool isPhy100BaseTHalfDuplexCapable() const											= 0;
		virtual bool isPhy100BaseTFullDuplexCapable() const											= 0;
		virtual bool isPhySMIPreambleSuppressionCapable() const									= 0;
		virtual uint8 getPhyProtocolID() const																	= 0;

		virtual bool doesLinkPartnerDesireNextPageTransfer() const							= 0;
		virtual bool doesLinkPartnerAcknowledgeReceiptionOfLinkCodeWord() const	= 0;
		virtual bool doesLinkPartnerAdvertiseRemoteFaultDetection() const				= 0;
		virtual bool doesLinkPartnerAdvertiseAsymmetricalPauseAbility() const		= 0;
		virtual bool doesLinkPartnerAdvertisePauseAbility() const								= 0;
		virtual bool doesLinkPartnerAdvertise100BaseT4() const									= 0;
		virtual bool doesLinkPartnerAdvertise100BaseTXFullDuplex() const				= 0;
		virtual bool doesLinkPartnerAdvertise100BaseTXHalfDuplex() const				= 0;
		virtual bool doesLinkPartnerAdvertise10BaseTFullDuplex() const					= 0;
		virtual bool doesLinkPartnerAdvertise10BaseTHalfDuplex() const					= 0;
		virtual bool doesLinkPartnerSupportAutoNegotiation() const							= 0;
		virtual uint8 getLinkPartnerProtocolID() const													= 0;

		virtual uint32 getPhyID() const																					= 0;
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

