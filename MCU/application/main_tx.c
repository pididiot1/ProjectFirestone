/* transmitter code */

#include "msp430g2553.h"
#include <stdlib.h>

#include "bsp.h"                            // bsp header files
#include "bsp_leds.h"                       // included with simpliciti
#include "bsp_buttons.h"

#include "mrfi.h"                           // simpliciti headers
#include "nwk_types.h"
#include "nwk_api.h"

#include "vlo_rand.h"

/* global variables */

typedef struct sensors_struct
{
	char cadc;
	int iadc;
}my_sensors;

struct sensors_struct sensor;

unsigned int i;

volatile int ADCdata[10];				// dtc address

// for simpliciti tx/rx
linkID_t  linkIDTemp;
uint8_t   msg[8];

addr_t lAddr;
char *Flash_Addr;

/* end globals */

/* prototypes for init functions */
void Init_ADC10(void);
void Init_TIMER0A0(void);
void createRandomAddress(void);

/* end prototypes */

void main(void)
{

	sensor.cadc = 'A';
	sensor.iadc = 125;

  	BSP_Init();                               // init bsp first, then simpliciti

  	BCSCTL3 = LFXT1S_2; 					// aclk = vlo

  	// address check and creation
	Flash_Addr = (char *)0x10F0;              // RF Address = 0x10F0
	if( Flash_Addr[0] == 0xFF &&              // Check if device Address is missing
    	Flash_Addr[1] == 0xFF &&
    	Flash_Addr[2] == 0xFF &&
    	Flash_Addr[3] == 0xFF )
  	{
    	createRandomAddress();                  // Create Random device address at
	}                                         // initial startup if missing
	lAddr.addr[0] = Flash_Addr[0];
	lAddr.addr[1] = Flash_Addr[1];
	lAddr.addr[2] = Flash_Addr[2];
	lAddr.addr[3] = Flash_Addr[3];
	// load address
	SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &lAddr);

  	SMPL_Init(NULL);                          // null callback for TX

  	Init_ADC10();
  	Init_TIMER0A0();

  	do {                                       // wait for button
    if (BSP_BUTTON1())
    {
      break;
    }
  } while (1);

  while (SMPL_SUCCESS != SMPL_Link(&linkIDTemp))    // link to Rx
  {
    BSP_TOGGLE_LED1();                      // toggle red for not linked
  }

  	BSP_TURN_OFF_LED1();                      // red off
  	BSP_Delay(2000);                             // for 2 seconds
  	BSP_TURN_ON_LED1();

  	_EINT();                                  // Enable Global Interupts

  while (1)
  {
	  BSP_TOGGLE_LED2();
    // adc with dtc in use
    ADC10CTL0 &= ~ENC;						// turn off adc10
    while (ADC10CTL1 & BUSY);               // wait if adc10 core is active
    ADC10SA = (unsigned int)ADCdata;    	// data buffer start
    ADC10CTL0 |= ENC + ADC10SC;             // sampling and conversion start

    LPM3;

    // insert sensor calculations here
  	// or
    // send raw adc data from P1.0
  	sensor.iadc = ADCdata[0];

	// turn on radio and tx sensor struct
    SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);
    SMPL_Send(linkIDTemp, (uint8_t *)&sensor, sizeof( my_sensors ));

  }
}

// setup adc10 to use dtc, 1 channels,
void Init_ADC10(void)
{
	ADC10CTL0 &= ~ENC;													// turn off adc for settings
    ADC10CTL0 = ADC10ON + MSC + ADC10SHT_2 + SREF_0;					// turn on; multisample; 16 cycles; vcc/v--
    ADC10CTL1 = CONSEQ_2 + ADC10SSEL_0 + ADC10DIV_0 + SHS_0 + INCH_10;	// repeat single; smclk; /1; int temp sensor
    //ADC10AE0 = 0x01;													// enable adc channel; not needed for int temp
    ADC10DTC0 = ADC10CT;												// continuous transfer
    ADC10DTC1 = 10;														// 10 transfers/block
    ADC10SA = (unsigned int)ADCdata;									// start address for dtc conversions
    ADC10CTL0 |= ENC;
}

void createRandomAddress(void)
{
	unsigned int rand, rand2;
	char *Flash_Addr;
	Flash_Addr = (char *)0x10F0;

	do
	{
    	rand = TI_getRandomIntegerFromADC();    // first byte can not be 0x00 of 0xFF
	}
  	while( (rand & 0xFF00)==0xFF00 || (rand & 0xFF00)==0x0000 );
  	rand2 = TI_getRandomIntegerFromADC();

  	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  	DCOCTL = CALDCO_1MHZ;
  	FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  	FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  	FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

	Flash_Addr[0]=(rand>>8) & 0xFF;
  	Flash_Addr[1]=rand & 0xFF;
  	Flash_Addr[2]=(rand2>>8) & 0xFF;
  	Flash_Addr[3]=rand2 & 0xFF;

  	FCTL1 = FWKEY;                            // Clear WRT bit
  	FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}


// enable Timer0a0 16bit up mode
void Init_TIMER0A0(void)
{
  	TA0CCTL0 = CCIE;	// no capture, toggle
	TA0CCR0 = 12000;    // about 1 sec with vlo
    TA0CTL = TASSEL_1 + ID_0 + MC_1;		// ACLK, /1, up mode
}

// Timer0A0 isr
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0A0(void)
{
	__no_operation();
	// cycle alphabet to show part of struct transfer
	if (sensor.cadc < 90)
	{
		sensor.cadc = 'A';
	} else
	{
		sensor.cadc++;
	}
  	LPM3_EXIT;                                // Exit LPM
}
