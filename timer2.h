//Written by Phuong H. Hoang

#ifndef _TIMER2_H_
#define _TIMER2_H_
extern void InitTimer2(unsigned int prescale, unsigned int period);
extern void EnableTimer2Interrupt(void);
extern void DisableTimer2Interrupt(void);
extern void SetTimer2InterruptPriority(unsigned int prior);
extern void Timer2On(void);

/* interrupt routine for Timer 2 module */
extern void _ISR _T2Interrupt(void);

/* exported varibles */
extern unsigned int qei_buf;
extern long int 	error;
extern long int		real_pos;
extern long int		last_pos;
extern fractional 	set_v;
extern fractional	real_v;
extern fractional 	set_i;
extern long int 	pwm_duty;
extern tPID 		strPID_v;
extern tPID			*strPID;


#endif // _TIMER2_H_
