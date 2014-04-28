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
#define  PATABLE_VAL        (0xC0)          // 0 dBm output

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

unsigned char myBuffer[9] = {MY_PACKET_LEN, 0x01, 0x02, 0x03, 0x04, 0x05, 0x99, 0xFF, 0x00};

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

/******************* MY UART GLOBALS ************************************************/

unsigned char uartBuffer[MY_PACKET_LEN+1];
int uartBufferIndex = 1;
volatile unsigned char uartReady = 0;

/******************* END MY UART GLOBALS ********************************************/

/******************** MY PIEZO GLOBALS **********************************************/

#define   Num_of_Samples   100
volatile unsigned int results[Num_of_Samples];
volatile unsigned int index = 0;
volatile unsigned char piezoReady = 0;

/******************** END MY PIEZO GLOBALS ******************************************/

/******************** HOLD IDENTIFIERS **********************************************/

#define myID		0x00
#define UART		0
#define PIEZO		1

/******************** END HOLD IDENTIFIERS ******************************************/


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
#if UART == 1
	InitUART();
#endif
#if PIEZO == 1
	InitPiezo();
#endif

	ReceiveOn();
	receiving = 1;

	while (1)
	{
		__bis_SR_register( LPM3_bits + GIE );
		__no_operation();

		if (buttonPressed)                      // Process a button press->transmit
		{
			//			P3OUT |= BIT1;                        // Pulse LED during Transmit
			buttonPressed = 0;
			P1IFG = 0;

			ReceiveOff();
			receiving = 0;
			Transmit( (unsigned char*)myBuffer, sizeof myBuffer);
			transmitting = 1;

			P1IE |= BIT7;                         // Re-enable button press
		} else if(!transmitting) {
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

			ReceiveOn();
			receiving = 1;
		} else if(uartReady) {
			uartReady = 0;
			for(i = 1; i < 9; i++) {
				if(uartBuffer[i] == 0xFF) {
					uartBuffer[i] = 0x00;
				}
			}

			updateStateUart();

			ReceiveOff();
			receiving = 0;
			uartBuffer[0] = MY_PACKET_LEN;
			Transmit( (unsigned char*)uartBuffer, sizeof uartBuffer);
			transmitting = 1;
		} else if(piezoReady) {
			// React appropriately
			piezoReady = 0;
			__no_operation();
		}

		/*
		if(!receiving && !transmitting && !buttonPressed)
		{
			ReceiveOn();
			receiving = 1;
		}
		 */
	}
}

void InitButtonLeds(void)
{
	// Set up the button as interruptible
	/*
	P1DIR &= ~BIT7;
	P1REN |= BIT7;
	P1IES &= BIT7;
	P1IFG = 0;
	P1OUT |= BIT7;
	P1IE  |= BIT7;
	 */
	P1DIR &= ~BIT7;
	P1OUT &= ~BIT7;
	P1DIR &= ~BIT4;
	P1OUT &= ~BIT4;

	// Initialize Port J
	PJOUT = 0x00;
	PJDIR = 0xFF;

	// Set up LED
	P3DIR |= BIT1 + BIT2 + BIT3;                     // P2.0 and P2.2 output
	P3SEL |= BIT1 + BIT2 + BIT3;                     // P2.0 and P2.2 options select
	/*
	P3OUT &= ~BIT1;
	P3DIR |= BIT1;
	P3OUT &= ~BIT2;
	P3DIR |= BIT2;
	P3OUT &= ~BIT3;
	P3DIR |= BIT3;
	 */
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

	PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
	P3MAP1 = PM_TA0CCR1A;                     // Map TA1CCR1 output to P3.0
	P3MAP2 = PM_TA0CCR2A;                     // Map TA1CCR2 output to P3.1
	P3MAP3 = PM_TA0CCR3A;                     // Map TA1CCR3 output to P3.3
	PMAPPWD = 0;                              // Lock port mapping registers

	TA0CCR0 = 256-1;                          // PWM Period
	TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
	TA0CCR1 = 0;                            // CCR1 PWM duty cycle
	TA0CCTL2 = OUTMOD_7;                      // CCR2 reset/set
	TA0CCR2 = 0;                            // CCR2 PWM duty cycle
	TA0CCTL3 = OUTMOD_7;
	TA0CCR3 = 0;
	TA0CTL = TASSEL_2 + MC_1 + TACLR;         // SMCLK, up mode, clear TAR
}

void InitUART(void) {
	PMAPPWD = 0x02D52;                        // Get write-access to port mapping regs
	P1MAP5 = PM_UCA0RXD;                      // Map UCA0RXD output to P1.5
	P1MAP6 = PM_UCA0TXD;                      // Map UCA0TXD output to P1.6
	PMAPPWD = 0;                              // Lock port mapping registers

	P1DIR |= BIT6;                            // Set P1.6 as TX output
	P1SEL |= BIT5 + BIT6;                     // Select P1.5 & P1.6 to UART function

	UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 6;                              // 1MHz 9600 (see User's Guide)
	UCA0BR1 = 0;                              // 1MHz 9600
	UCA0MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
	// over sampling
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}

void InitPiezo(void) {
	/* Initialize ADC12_A */
	ADC12CTL0 = ADC12ON+ADC12SHT0_8+ADC12MSC; // Turn on ADC12, set sampling time
	// set multiple sample conversion
	ADC12CTL1 = ADC12SHP+ADC12CONSEQ_2;       // Use sampling timer, set mode
	ADC12IE = 0x01;                           // Enable ADC12IFG.0
	ADC12CTL0 |= ADC12ENC;                    // Enable conversions
	ADC12CTL0 |= ADC12SC;                     // Start conversion
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

	TA0CCR1 = currState.red;
	TA0CCR2 = currState.green;
	TA0CCR3 = currState.blue;
}

void updateStateUart(void) {
	currState.holdID = uartBuffer[iHoldID];
	currState.powerState = uartBuffer[iPowerState];
	currState.startOn = uartBuffer[iStartOn];
	currState.startDelay = uartBuffer[iStartDelay];
	currState.litTime = uartBuffer[iLitTime];
	currState.red = uartBuffer[iRed];
	currState.green = uartBuffer[iGreen];
	currState.blue = uartBuffer[iBlue];

	TA0CCR1 = currState.red;
	TA0CCR2 = currState.green;
	TA0CCR3 = currState.blue;
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
			//			if(RxBuffer[MY_CRC_LQI_IDX] & MY_CRC_OK) {
			//				P3OUT ^= BIT2;                    // Toggle LED1
			if(RxBuffer[iHoldID] == myID) {
				newState = 1;
			}
			//			}
		}
		else if(transmitting)		    // TX end of packet
		{
			RF1AIE &= ~BIT9;                    // Disable TX end-of-packet interrupt
			//			P3OUT &= ~BIT1;                     // Turn off LED after Transmit
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

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch(__even_in_range(UCA0IV,4))
	{
	case 0:break;                             // Vector 0 - no interrupt
	case 2:                                   // Vector 2 - RXIFG
		if(UCA0RXBUF != 0x0D) {
			uartBuffer[uartBufferIndex] = UCA0RXBUF;
			uartBufferIndex++;
			if(uartBufferIndex >= MY_PACKET_LEN) {
				uartBufferIndex = 1;
			}
		} else {
			uartBufferIndex = 1;
			uartReady = 1;
			__bic_SR_register_on_exit(LPM3_bits); // Exit active
		}
		while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
		UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
		break;
	case 4:break;                             // Vector 4 - TXIFG
	default: break;
	}
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
	static unsigned char index = 0;

	switch(__even_in_range(ADC12IV,34))
	{
	case  0: break;                           // Vector  0:  No interrupt
	case  2: break;                           // Vector  2:  ADC overflow
	case  4: break;                           // Vector  4:  ADC timing overflow
	case  6:                                  // Vector  6:  ADC12IFG0
		results[index] = ADC12MEM0;             // Move results
		if(results[index] < 1000) {
			piezoReady = 1;
			__bic_SR_register_on_exit(LPM3_bits); // Exit active
		}
		index++;                                // Increment results index, modulo; Set Breakpoint1 here
		if (index == Num_of_Samples)
			index = 0;                            // Reset the index; Set Breakpoint here
		break;

	case  8: break;                           // Vector  8:  ADC12IFG1
	case 10: break;                           // Vector 10:  ADC12IFG2
	case 12: break;                           // Vector 12:  ADC12IFG3
	case 14: break;                           // Vector 14:  ADC12IFG4
	case 16: break;                           // Vector 16:  ADC12IFG5
	case 18: break;                           // Vector 18:  ADC12IFG6
	case 20: break;                           // Vector 20:  ADC12IFG7
	case 22: break;                           // Vector 22:  ADC12IFG8
	case 24: break;                           // Vector 24:  ADC12IFG9
	case 26: break;                           // Vector 26:  ADC12IFG10
	case 28: break;                           // Vector 28:  ADC12IFG11
	case 30: break;                           // Vector 30:  ADC12IFG12
	case 32: break;                           // Vector 32:  ADC12IFG13
	case 34: break;                           // Vector 34:  ADC12IFG14
	default: break;
	}
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
