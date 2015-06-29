#ifndef _PWM_H_
#define _PWM_H_

/******* Define *******/

#define FREE_MODE 				0x0000
#define SINGLE_SHOT_MODE 		0x0001
#define CONTINUOUS_UP_DOWN_MODE	0x0002
#define DOUBLE_UPDATE_MODE		0x0003

// define PWM pins

#define PWM1H					0x0010
#define PWM2H					0x0020
#define PWM3H					0x0040
#define PWM1L					0x0001
#define PWM2L					0x0002
#define PWM3L					0x0004

#define POVD1L					0x0100
#define POVD1H					0x0200
#define POVD2L					0x0400
#define POVD2H					0x0800
#define POVD3L					0x1000
#define POVD3H					0x2000
 
#define POUT1L					0x0001
#define POUT1H					0x0002
#define POUT2L					0x0004
#define POUT2H					0x0008
#define POUT3L					0x0010
#define POUT3H					0x0020
/***** exported variables *****/


/***** exported functions *****/
extern void InitPwm(unsigned int pwm_freq, unsigned int pwm_duty);
extern void PwmMode(unsigned int pwm_mode);
extern void PwmTimebaseSetting(unsigned int pwm_prescale,unsigned int pwm_period);
extern void SetPwmDutycycle(unsigned int duty_cycle);
extern void SetPwmFreq(unsigned int freq);
extern void PwmEnable(void);
extern void SetPwmDutyCycle(unsigned int pwm_dutycycle);
extern void SetOvdPin(unsigned int pin);
extern void ClearOvdPin(unsigned int pin);
extern void SetPwmPin(unsigned int pin);
extern void ClearPwmPin(unsigned int pin);
extern void SetPwmPinout(void);
extern void RunPositive(void);
extern void RunNegative(void);
extern void RunStop(void);
extern char dong_am;
/* interrupt routine for PWM module is not used in this project*/
#endif
