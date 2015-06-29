#ifndef _TIMER3_H_
#define _TIMER3_H_
extern void InitTimer3(unsigned int prescale, unsigned int period);
extern void EnableTimer3Interrupt(void);
extern void DisableTimer3Interrupt(void);
extern void SetTimer3InterruptPriority(unsigned int prior);
extern void Timer3On(void);

/* interrupt routine for Timer 2 module */
extern void _ISR _T3Interrupt(void);
#endif // _TIMER2_H_
