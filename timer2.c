/*Wrtitten by Phuong H. Hoang 2013*/ 
/* This file contents functions used to config timer 2 module in 16bits mode */
/* Note: this file is only built for configing the timer2/3 module to run in 2 16 timers mode */
#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
#include "typedef.h"
#include "timer2.h"
#include "hardware_descriptors.h"
#include "pwm.h"
#include "hal_uart.h"
/***** global variables *****/

extern BOOL g_bUartFlag;

/***** function prototype ****/
// functions used to config timer 2
void InitTimer2(unsigned int prescale, unsigned int period);	// init timer 2
void EnableTimer2Interrupt(void);								// enable timer 2 interrupt
void DisableTimer2Interrupt(void);								// disable timer 2 interrupt
void SetTimer2InterruptPriority(unsigned int prior);			// set timer 2 interrupts priority
void Timer2On(void);											// turn timer 2 on
/* timer 2 interrupt routine */
void __attribute__ ((interrupt, no_auto_psv)) _T2Interrupt(void);
/********** Tinh toan mach vong toc do  ************/
// timer 2 interrupt routine - 5ms 1 ngat
/* tinh mach vong toc do theo luat PI */
void __attribute__ ((interrupt, no_auto_psv)) _T2Interrupt(void)
{
	// xoa co ngat
	IFS0bits.T2IF = 0;
        g_bUartFlag = ON;
	// tinh vi tri
	real_pos = real_pos - (long int)qei_buf + (long int)POSCNT;	
	qei_buf = POSCNT;
	real_v = (fractional)((real_pos - last_pos)*200);	
	last_pos = real_pos; 
	if (real_v < 0)
		real_v = 0 - real_v;

// tinh toan PID
	strPID_v.measuredOutput = real_v;
	strPID_v.controlReference = set_v;
	PID(&strPID_v);
	set_i = strPID_v.controlOutput;
	/* khau gioi han dong dien */
	if (set_i > 500)
		set_i = 500;
	if (set_i < 0)
		set_i = 0;
}
/***************************************************/
// Function implementation
void InitTimer2(unsigned int prescale, unsigned int period)
{
	switch (prescale)								// selecting prescale
	{
		case (1):
			T2CON = T2CON & 0xFFCF;
			break;
		case (8):
			T2CON = T2CON & 0xFFDF;
			T2CON = T2CON | 0x0010;
			break;
		case (64):
			T2CON = T2CON & 0xFFEF;
			T2CON = T2CON | 0xFF20;
			break;
		case (256):
			T2CON = T2CON | 0x0030;
			break;
		default:
			break;					
	}
	PR2 = period;									// setting timer2 period
	T2CONbits.TSIDL = 0;							// disable iddle mode
	T2CONbits.T32	= 0;							// disable 32bit timer mode
	T2CONbits.TCS	= 0;							// select internal source
	EnableTimer2Interrupt();						// enable timer 2 interrupt
	SetTimer2InterruptPriority(T2_INT_PRIOR);
	Timer2On();
}

void EnableTimer2Interrupt(void)
{
	IEC0bits.T2IE = 1;								// enabling timer1 interrupt
}

void DisableTimer2Interrupt(void)
{
	IEC0bits.T2IE = 0;								// disabling timer1 interrupt
}

void SetTimer2InterruptPriority(unsigned int prior)
{
	IPC1 = IPC1 & 0xF0FF;							// clearing ICP2 bits
	prior = prior << 8;
	IPC1 = IPC1 | prior;
}

void Timer2On(void)
{
	T2CONbits.TON = 1;								// Start timer 1
}
