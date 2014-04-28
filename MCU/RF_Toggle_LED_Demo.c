/******************************************************************************
 * CC430 RF Code Example - TX and RX (variable packet length =< FIFO size)
 *
 * Simple RF Link to Toggle Receiver's LED by pressing Transmitter's Button
 * Warning: This RF code example is setup to operate at either 868 or 915 MHz,
 * which might be out of allowable range of operation in certain countries.
 * The frequency of operation is selectable as an active build configuration
 * in the project menu.
 *
 * Please refer to the appropriate legal sources before performing tests with
 * this code example.
 *
 * This code example can be loaded to 2 CC430 devices. Each device will transmit
 * a small packet upon a button pressed. Each device will also toggle its LED
 * upon receiving the packet.
 *
 * The RF packet engine settings specify variable-length-mode with CRC check
 * enabled. The RX packet also appends 2 status bytes regarding CRC check, RSSI
 * and LQI info. For specific register settings please refer to the comments for
 * each register in RfRegSettings.c, the CC430x513x User's Guide, and/or
 * SmartRF Studio.
 *
 * G. Larmore
 * Texas Instruments Inc.
 * June 2012
 * Built with IAR v5.40.1 and CCS v5.2
 ******************************************************************************/

#include "RF_Toggle_LED_Demo.h"

/***************** RADIO GLOBALS *********************************************/

#define  PACKET_LEN         (0x05)	    // PACKET_LEN <= 61
#define  RSSI_IDX           (PACKET_LEN+1)  // Index of appended RSSI 
#define  CRC_LQI_IDX        (PACKET_LEN+2)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit 
#define  PATABLE_VAL        (0x51)          // 0 dBm output 

extern RF_SETTINGS rfSettings;

unsigned char packetReceived;
unsigned char packetTransmit; 

unsigned char RxBuffer[64];
unsigned char RxBufferLength = 0;
const unsigned char TxBuffer[6]= {PACKET_LEN, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
unsigned char buttonPressed = 0;
unsigned int i = 0; 

unsigned char transmitting = 0; 
unsigned char receiving = 0; 

/****************** END RADIO GLOBALS ******************************************/

/******************* MY RADIO GLOBALS ************************************************/

#define	 MY_PACKET_LEN 		(0x08)
#define  MY_RSSI_IDX        (MY_PACKET_LEN+1)  // Index of appended RSSI
#define  MY_CRC_LQI_IDX     (MY_PACKET_LEN+2)  // Index of appended LQI, checksum
#define  MY_CRC_OK          (BIT7)          // CRC_OK bit
#define  MY_PATABLE_VAL     (0x51)          // 0 dBm output

unsigned char myBuffer[9] = {MY_PACKET_LEN, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

/******************* END MY RADIO GLOBALS ********************************************/

/******************* MY PARAM GLOBALS ****************************************************/

// Receive parameters
#define iHoldID		1
#define iPowerState 2
#define iStartOn	3
#define iStartDelay	4
#define iLitTime	5
#define iRed		6
#define iGreen		7
#define iBlue		8

typedef enum {OFF, ACTIVE, TX} powerState_t;

typedef struct {
	int holdID;
	powerState_t powerState;
	unsigned char startOn;
	int startDelay;
	int litTime;
	int red;
	int green;
	int blue;
} currState_t;

volatile currState_t currState;
volatile unsigned char newState = 0;

/******************* END MY PARAM GLOBALS ************************************************/

volatile unsigned char redOn = 0;
volatile unsigned char greenOn = 0;
volatile unsigned char blueOn = 0;

void main( void )
{  
	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW + WDTHOLD;

	// Increase PMMCOREV level to 2 for proper radio operation
	SetVCore(2);

	ResetRadioCore();
	InitRadio();
	InitButtonLeds();
	InitLEDTimer();

	ReceiveOn();
	receiving = 1;

	while (1)
	{
		__bis_SR_register( LPM3_bits + GIE );
		__no_operation();

		if (buttonPressed)                      // Process a button press->transmit
		{
			P3OUT |= BIT1;                        // Pulse LED during Transmit
			buttonPressed = 0;
			P1IFG = 0;

			ReceiveOff();
			receiving = 0;
			Transmit( (unsigned char*)myBuffer, sizeof myBuffer);
			transmitting = 1;

			P1IE |= BIT7;                         // Re-enable button press
		}
		else if(!transmitting)
		{
			ReceiveOn();
			receiving = 1;
		}

		if(newState) {
			newState = 0;
			// Turn receiving off to grab data
			ReceiveOff();
			receiving = 0;

			// Update current state from RxBuffer
			updateState();
			__no_operation();
			// Turn receiving back on again
			ReceiveOn();
			receiving = 1;
		}

	}
}

void InitButtonLeds(void)
{
	// Set up the button as interruptible
	P1DIR &= ~BIT7;
	P1REN |= BIT7;
	P1IES &= BIT7;
	P1IFG = 0;
	P1OUT |= BIT7;
	P1IE  |= BIT7;

	// Initialize Port J
	PJOUT = 0x00;
	PJDIR = 0xFF;

	// Set up LED
	P3OUT &= ~BIT1;
	P3DIR |= BIT1;
	P3OUT &= ~BIT2;
	P3DIR |= BIT2;
	P3OUT &= ~BIT3;
	P3DIR |= BIT3;
}

void InitRadio(void)
{
	// Set the High-Power Mode Request Enable bit so LPM3 can be entered
	// with active radio enabled
	PMMCTL0_H = 0xA5;
	PMMCTL0_L |= PMMHPMRE_L;
	PMMCTL0_H = 0x00;

	WriteRfSettings(&rfSettings);

	WriteSinglePATable(PATABLE_VAL);
}

void InitLEDTimer(void) {

	TA1CCTL0 = OUTMOD_4 + CCIE; // CCR0 toggle, interrupt enabled
	TA1CCTL1 = OUTMOD_4 + CCIE; // CCR1 toggle, interrupt enabled
	TA1CCTL2 = OUTMOD_4 + CCIE; // CCR2 toggle, interrupt enabled
	TA1CTL = TASSEL_2 + MC_2 + TAIE; // SMCLK, Contmode, int enabled
	/*
	TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
	TA1CCR0 = 50000;
	TA1CCTL1 = CCIE;
	TA1CCR1 = 50000;

	TA1CTL = TASSEL_2 + MC_2 + TACLR;         // SMCLK, contmode, clear TAR
	 */
}

void updateState(void) {
	currState.holdID = RxBuffer[iHoldID];
	currState.powerState = RxBuffer[iPowerState];
	currState.startOn = RxBuffer[iStartOn];
	currState.startDelay = RxBuffer[iStartDelay];
	currState.litTime = RxBuffer[iLitTime];
	currState.red = RxBuffer[iRed];
	currState.green = RxBuffer[iGreen];
	currState.blue = RxBuffer[iBlue];
}

void Transmit(unsigned char *buffer, unsigned char length)
{
	RF1AIES |= BIT9;
	RF1AIFG &= ~BIT9;                         // Clear pending interrupts
	RF1AIE |= BIT9;                           // Enable TX end-of-packet interrupt

	WriteBurstReg(RF_TXFIFOWR, buffer, length);

	Strobe( RF_STX );                         // Strobe STX
}

void ReceiveOn(void)
{  
	RF1AIES |= BIT9;                          // Falling edge of RFIFG9
	RF1AIFG &= ~BIT9;                         // Clear a pending interrupt
	RF1AIE  |= BIT9;                          // Enable the interrupt

	// Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
	Strobe( RF_SRX );
}

void ReceiveOff(void)
{
	RF1AIE &= ~BIT9;                          // Disable RX interrupts
	RF1AIFG &= ~BIT9;                         // Clear pending IFG

	// It is possible that ReceiveOff is called while radio is receiving a packet.
	// Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe
	// such that the RXFIFO is empty prior to receiving a packet.
	Strobe( RF_SIDLE );
	Strobe( RF_SFRX  );
}

#pragma vector=CC1101_VECTOR
__interrupt void CC1101_ISR(void)
{
	switch(__even_in_range(RF1AIV,32))        // Prioritizing Radio Core Interrupt
	{
	case  0: break;                         // No RF core interrupt pending
	case  2: break;                         // RFIFG0
	case  4: break;                         // RFIFG1
	case  6: break;                         // RFIFG2
	case  8: break;                         // RFIFG3
	case 10: break;                         // RFIFG4
	case 12: break;                         // RFIFG5
	case 14: break;                         // RFIFG6
	case 16: break;                         // RFIFG7
	case 18: break;                         // RFIFG8
	case 20:                                // RFIFG9
		if(receiving)			    // RX end of packet
		{
			// Read the length byte from the FIFO
			RxBufferLength = ReadSingleReg( RXBYTES );
			ReadBurstReg(RF_RXFIFORD, RxBuffer, RxBufferLength);

			// Stop here to see contents of RxBuffer
			__no_operation();

			// Check the CRC results
			if(RxBuffer[MY_CRC_LQI_IDX] & MY_CRC_OK) {
				P3OUT ^= BIT2;                    // Toggle LED1
				newState = 1;
			}
		}
		else if(transmitting)		    // TX end of packet
		{
			RF1AIE &= ~BIT9;                    // Disable TX end-of-packet interrupt
			P3OUT &= ~BIT1;                     // Turn off LED after Transmit
			transmitting = 0;
		}
		else while(1); 			    // trap
		break;
	case 22: break;                         // RFIFG10
	case 24: break;                         // RFIFG11
	case 26: break;                         // RFIFG12
	case 28: break;                         // RFIFG13
	case 30: break;                         // RFIFG14
	case 32: break;                         // RFIFG15
	}
	__bic_SR_register_on_exit(LPM3_bits);
}

// Red interrupt service routine
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
	if(!redOn) // If output currently high
	{
		TA1CCR0 += (255 - currState.red); // 25% high
		P3OUT |= BIT1;
		redOn = 1;
	}
	else
	{
		TA1CCR0 += (currState.red + 1); // 75% low
		P3OUT &= ~BIT1;
		redOn = 0;
	}

	/*
	P3OUT ^= BIT1;                            // Toggle P1.0
	TA1CCR0 += 50000;                         // Add Offset to CCR0
	 */
}
// Green interrupt service routine
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
	switch( TA1IV )
	{
	case 2: if(!greenOn) // If output currently high
	{
		TA1CCR1 += (255-currState.green); // 12.5% high
		P3OUT |= BIT2;
		greenOn = 1;
	}
	else
	{
		TA1CCR1 += (currState.green + 1); // 87.5% low
		P3OUT &= ~BIT2;
		greenOn = 0;
	}
	break;
	case 4: if(!blueOn) // If output currently high
	{
		TA1CCR2 += (255-currState.blue); // 60% high
		P3OUT |= BIT3;
		blueOn = 1;
	}
	else
	{
		TA1CCR2 += (currState.blue + 1); // 40% low
		P3OUT &= ~BIT3;
		blueOn = 0;
	}
	break;
	case 10: P1OUT ^= 0x01; // Timer overflow
	break;
	default: break;
	}

	/*
	P3OUT ^= BIT2;                            // Toggle P1.0
	TA1CCR1 += 50000;                         // Add Offset to CCR0
	 */
}


#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	switch(__even_in_range(P1IV, 16))
	{
	case  0: break;
	case  2: break;                         // P1.0 IFG
	case  4: break;                         // P1.1 IFG
	case  6: break;                         // P1.2 IFG
	case  8: break;                         // P1.3 IFG
	case 10: break;                         // P1.4 IFG
	case 12: break;                         // P1.5 IFG
	case 14: break;                         // P1.6 IFG
	case 16:                                // P1.7 IFG
		P1IE = 0;                             // Debounce by disabling buttons
		buttonPressed = 1;
		__bic_SR_register_on_exit(LPM3_bits); // Exit active
		break;
	}
}
