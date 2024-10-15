#ifndef INC_TIMER_H
#define INC_TIMER_H

#include "common-defines.h"

void timer_setup(void);
void timer_pwm_set_duty_ratio(float duty_ratio);    // 0 < duty_cycle < 1

#endif // INC_TIMER_H