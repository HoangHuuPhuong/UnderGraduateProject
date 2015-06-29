//Written by Phuong H. Hoang 2013 month: about March to May, dont remember exactly :))
#define _dsPIC30F4012_
#include <p30F4012.h>
#include <dsp.h>
#include "timer3.h"
#include "hardware_descriptors.h"
/***** global variables *****/

/***** function prototype ****/
// functions used to config timer 2
void InitTimer3(unsigned int prescale, unsigned int period);	// init timer 3
void EnableTimer3Interrupt(void);								// enable timer 3 interrupt
void DisableTimer3Interrupt(void);								// disable timer 3 interrupt
void SetTimer3InterruptPriority(unsigned int prior);			// set timer 3 interrupts priority
void Timer3On(void);											// turn timer 3 on
void Timer3Off(void);
/* timer 2 interrupt routine */
void __attribute__ ((interrupt, no_auto_psv)) _T3Interrupt(void);

// timer 2 interrupt routine
void __attribute__ ((interrupt, no_auto_psv)) _T3Interrupt(void)
{
	IFS0bits.T3IF = 0;		
}

// Function implementation
void InitTimer3(unsigned int prescale, unsigned int period)
{
	switch (prescale)								// selecting prescale
	{
		case (1):
			T3CON = T3CON & 0xFFCF;
			break;
		case (8):
			T3CON = T3CON & 0xFFDF;
			T3CON = T3CON | 0x0010;
			break;
		case (64):
			T3CON = T3CON & 0xFFEF;
			T3CON = T3CON | 0xFF20;
			break;
		case (256):
			T3CON = T3CON | 0x0030;
			break;
		default:
			break;					
	}
	PR3 = period;								// setting timer2 period
	T3CONbits.TSIDL = 0;							// disable iddle mode
	T2CONbits.T32	= 0;							// disable 32bit timer mode
	T3CONbits.TCS	= 0;							// select internal source
	DisableTimer3Interrupt();						// enable timer 2 interrupt
        //EnableTimer3Interrupt();
	SetTimer3InterruptPriority(T3_INT_PRIOR);
	Timer3On();
}

void EnableTimer3Interrupt(void)
{
	IEC0bits.T3IE = 1;							// enabling timer1 interrupt
}

void DisableTimer3Interrupt(void)
{
	IEC0bits.T3IE = 0;							// disabling timer1 interrupt
}

void SetTimer3InterruptPriority(unsigned int prior)
{
	IPC1 = IPC1 & 0x0FFF;							// clearing ICP2 bits
	prior = prior << 12;
	IPC1 = IPC1 | prior;
}

void Timer3On(void)
{
	T3CONbits.TON = 1;							// Start timer 1
}
void Timer3Off(void)
{
	T3CONbits.TON = 0;
}
