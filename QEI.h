#ifndef _QEI_H_
#define _QEI_H_
// define macro
#define INDEXMODEx2	0x0400
#define MODEx2		0x0500
#define INDEXMODEx4	0x0600
#define MODEx4		0x0700
// exported function
extern void InitQei(void);

/* interrupt routine for QEI moudule */
extern void _ISR _QEIInterrupt(void);

/* exported variables */
extern unsigned int qei_buf;
extern long int		set_pos;
extern long int		real_pos;
extern long int		last_pos;
#endif // _QEI_H_
