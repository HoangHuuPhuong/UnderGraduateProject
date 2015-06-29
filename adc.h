// Written by Phuong H. Hoang and Ha M. Doan, April 2013
#ifndef _ADC_H_
#define _ADC_H_
 /**** constant definition *****/

// constant for pin selection
#define PIN_B0 0x0001
#define PIN_B1 0x0002
#define PIN_B2 0x0004
#define PIN_B3 0x0008
#define PIN_B4 0x0010
#define PIN_B5 0x0020
#define PIN_B6 0x0040
#define PIN_B7 0x0080
#define PIN_B8 0x0100

// constant for reference voltage selection
#define AVDD_AVSS		0x0000
#define VREF_AVSS		0x2000
#define AVDD_VREF		0x4000
#define VREF_VREF		0x6000

// constant for data type selection
#define UNSIGNED_INT 	0x0000
#define SIGNED_INT		0x0100
#define UNSIGNED_FRAC	0x0200
#define SIGNED_FRAC		0x0300

// constant for channel selection
#define CHANNEL_0		0x0000
#define CHANNEL_01		0x0100
#define CHANNEL_0123	0x0300

// ADC clock source
#define INTERNAL		0x0080
#define EXTERNAL		0xFF7F

/***** exported functions *****/
extern void TurnAdcOn(void);
extern void TurnAdcOff(void);
extern void AdcSoftwareStartSample(void);
extern void AdcSoftwareStartConversion(void);
extern void EnableAdcInterrupt(void);
extern void DisableAdcInterrupt(void);
extern void SetAdcInterruptPriority(unsigned int prior);
extern unsigned int ReadAdcBuffer (unsigned int buffer);
extern void InitAdc();
// interrupt routine for ADC module
extern void _ISR _ADCInterrupt(void);

/***** extern variables *****/
extern long int	 	pwm_duty;
extern long int 	set_pos;
extern long int 	real_pos;
extern fractional 	real_i;
extern fractional 	set_i;
extern fractional 	real_v;
extern long int 	error;
extern unsigned int qei_buf;
extern char 		output_buffer[8];
extern tPID			strPID_i;
extern tPID			*strPID;

#endif
