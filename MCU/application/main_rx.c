/* receiver code */

#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vlo_rand.h"

#include "bsp.h"                          // bsp header files
#include "bsp_leds.h"                     // included with simpliciti
#include "bsp_buttons.h"

#include "mrfi.h"                         // simpliciti headers
#include "nwk_types.h"
#include "nwk_api.h"

#define PIN3 1<<3
#define PIN4 1<<4
#define PIN5 1<<5

/* global variables */

typedef struct sensors_struct
{
	char cadc;
	int iadc;
}my_sensors;

struct sensors_struct sensor;

/* bsp related variables */
linkID_t  linkIDTemp;
uint8_t smpl_buffer[MAX_APP_PAYLOAD];
uint8_t len;
uint8_t flag;

addr_t lAddr;
char *Flash_Addr;

static uint8_t sRxCallback(linkID_t);

void createRandomAddress(void);
void Init_LED(void);

/* end globals */

void main(void)
{

	BSP_Init();                              // init bsp first, then simpliciti
	Init_LED();
	/*
  	while(1) {
  		BSP_TURN_OFF_LED1();
  		NWK_DELAY(2000);
  		BSP_TURN_ON_LED1();
  		NWK_DELAY(2000);
  	}
	 */
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

	SMPL_Init(sRxCallback);

	SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, 0); // turn on radio, listen

	while (SMPL_SUCCESS != SMPL_LinkListen(&linkIDTemp))    // link to Tx
	{
		BSP_TOGGLE_LED1();                      // toggle red for not linked
	}

	BSP_TURN_OFF_LED1();                      // red off
	NWK_DELAY(2000);                             // for 2 seconds
	BSP_TURN_ON_LED1();
	BSP_TURN_OFF_LED1();

	_EINT();                                  // Enable Global Interupts

	while (1)
	{
		__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 wait for UART Rx
		/*
		//SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);
		SMPL_Receive(linkIDTemp, (uint8_t *)&smpl_buffer, &len);


		if(strcmp(smpl_buffer, "TEST") == 0) {
			BSP_TURN_ON_LED1();
			NWK_DELAY(4000);
			BSP_TURN_OFF_LED1();
		}
		smpl_buffer[0] = '\n';
		 */
	}

}

static uint8_t sRxCallback(linkID_t linkIDTemp)
{
	SMPL_Receive(linkIDTemp, (uint8_t*)&smpl_buffer, &len);

	if(strstr(smpl_buffer, "TEST") != NULL)
	{
		CCTL0 ^= CCIE;                             // CCR0 interrupt toggle
		P1OUT &= ~(PIN3 | PIN4 | PIN5);            // Toggle P2
		//BSP_TOGGLE_LED1();
	}

	/*
	if(len == sizeof(my_sensors))
	{
  		memcpy(&sensor, smpl_buffer, len);
	} else {
  	__no_operation();
  	BSP_TOGGLE_LED1();
	}
	 */

	// replace with_  return message;
	return 0;
}

void Init_LED(void)
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P2DIR |= (PIN3 | PIN4 | PIN5);              // P2 output
//	CCTL0 = CCIE;                             // CCR0 interrupt toggle
	CCR0 = 50000;
	TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  P2OUT ^= (PIN3 | PIN4 | PIN5);                            // Toggle P1.0
  CCR0 += 50000;                            // Add Offset to CCR0
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
