#ifndef _TIMER1_H_
#define _TIMER1_H_

/***** exported variables *****/
extern unsigned int i;

/***** exported functions *****/
extern void InitTimer1(unsigned int prescale, unsigned int period);
extern void EnableTimer1Interrupt(void);
extern void DisableTimer1Interrupt(void);
extern void SetTimer1InterruptPriority(unsigned int prior);
extern void Timer1On();
/* interrupt routine for Timer 1 module */
extern void _ISR _T1Interrupt(void);

/***** extern variables *****/
extern unsigned int qei_buf;
extern long int 	error;
extern long int 	begin_pos;
extern long int 	already;
extern long int	 	set_pos;
extern long int		real_pos;
extern fractional	set_v;
extern tPID			strPID_v;
extern tPID			strPID_i;
#endif
