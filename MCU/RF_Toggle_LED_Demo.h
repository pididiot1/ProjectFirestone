#include "cc430x513x.h"
#include "RF1A.h"
#include "hal_pmm.h"
#include <intrinsics.h>

/*******************
 * Function Definition
 */
void Transmit(unsigned char *buffer, unsigned char length);
void ReceiveOn(void);
void ReceiveOff(void);

void InitButtonLeds(void);
void InitRadio(void);

void InitLEDs(void);
void setLEDs(unsigned char);

void InitUART(void);
void setUART(unsigned char);

void InitPiezo(void);
void setPiezo(unsigned char);

void InitRTC(void);
void setRTC(unsigned char);

void InitWDT(void);

void updateState(void);
void updateStateUart(void);
