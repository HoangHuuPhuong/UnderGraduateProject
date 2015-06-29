/*Written by Phuong H. Hoang */
/* This file contents functions used to config Timer 1 module */
/* Timer 1 is used for the current loop - 5ms per interrupt*/
#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
#include "typedef.h"
#include "hal_uart.h"
#include "timer1.h"
#include "hardware_descriptors.h"
/***** global variables declaration *****/
long int max_v;
extern double g_dKpp;
/***** functionss prototype *****/
void InitTimer1(unsigned int prescale, unsigned int period);
void EnableTimer1Interrupt(void);
void DisableTimer1Interrupt(void);
void SetTimer1InterruptPriority(unsigned int prior);
void Timer1On();
/* timer 1 interrupt routine  */
void __attribute__ ((interrupt, no_auto_psv)) _T1Interrupt(void);
/* note: the interrupts routine here is composed for testing timer1 module and UART module */

/***** functions implementation *****/

/* Ham thuc hien mach vong vi tri theo thuat toan P */
void __attribute__ ((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	//long int set_v1;
	//long int last_error;
	// xoa co ngat
	IFS0bits.T1IF = 0;
	// tinh vi tri hien thoi
	real_pos = real_pos - (long int)qei_buf + (long int)POSCNT;
	qei_buf =  POSCNT;
	// tinh sai lech
	//last_error = error;
	error = set_pos - real_pos;
	if (error < 0)
	{
		OVDCON = 0xFD00;
		error = 0 - error;	
	}
	else
        {
		OVDCON = 0xFE00;
	        //PDC1 = 0;
        }
	// tinh gia tri dat cho w.
	//set_v = (fractional)(g_dKpp * error);
        //set_v = (fractional)(KPP * error);
	if ((error < 30) && (error > -30))
	{
		set_v = 0;
		strPID_v.controlOutput = 0;
		strPID_i.controlOutput = 0;
	}
		
	else 
		//set_v = g_dKpp * (error);
            set_v = (fractional)(KPP * error);
	//if (set_v < 0)
	//	set_v = 0 - set_v;

	// gioi han toc do
	//if (set_v > 800)
	//	set_v = 800;
	//if (set_v < 0)
		//set_v = 0;
	//set_v = (fractional)set_v;
}

void InitTimer1(unsigned int prescale, unsigned int period)
{
	switch (prescale)							// selecting prescale
	{
		case (1):
			T1CON = T1CON & 0xFFCF;
			break;
		case (8):
			T1CON = T1CON & 0xFFDF;
			T1CON = T1CON | 0x0010;
			break;
		case (64):
			T1CON = T1CON & 0xFFEF;
			T1CON = T1CON | 0xFF20;
			break;
		case (256):
			T1CON = T1CON | 0x0030;
			break;
		default:
			break;					
	}
	PR1 = period;								// setting timer1 period
	T1CONbits.TSIDL = 0;
	T1CONbits.TCS	= 0;
	EnableTimer1Interrupt();
	SetTimer1InterruptPriority(T1_INT_PRIOR);
	Timer1On();	
}

void EnableTimer1Interrupt(void)
{
	IEC0bits.T1IE = 1;							// enabling timer1 interrupt
}

void DisableTimer1Interrupt(void)
{
	IEC0bits.T1IE = 0;							// disabling timer1 interrupt
}

void Timer1On(void)
{
	T1CONbits.TON = 1;							// Start timer 1
}
void SetTimer1InterruptPriority(unsigned int prior)
{
	prior 	= prior << 12;
	IPC0 	= IPC0 & 0x0FFF;					// clearing T1IP bits
	IPC0 	= IPC0 | prior;
}
