#ifndef _PID_FUNCTION_H_
#define _PID_FUNCTION_H_
#include "typedef.h"
/***** exported functions *****/
extern float Pid1Controller(PID_IN str_in);
extern void InitPidInputStruct(PID_IN str_in, float kp, float Ti, float Td, float T);

#endif // _PID_FUNCTION_H_
