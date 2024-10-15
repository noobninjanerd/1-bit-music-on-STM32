#include "core/timer.h"
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

// processor freq = 84_000_000

// freq = process_freq / (prescaler-1) * (ARR - 1)

#define PRESCALER   (84)
#define ARR_VALUE   (1000)
#define FREQ_COEFF  (84000000/(PRESCALER - 1))

void timer_setup(void)
{
    // a timer is composed of:
    // 1. counter
    // 2. Prescaler (buffered register, can be changed "on the fly")
    // 3. Registers: 
    //      a) Auto Reload Register: Sets the resolution of prescaled clock by setting an upper limit on the counter
    //      b) Capture Compare Register: Controls the duty ratio, as when it's set the output signal changes state (on->off or vice versa)

    rcc_periph_clock_enable(RCC_TIM5);  // give life to the timer by enabling the system clock to it

    // high level timer config
    timer_set_mode(TIM5, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP); 
    // writing all values to the control register 1 (cr1)

    timer_set_oc_mode(TIM5, TIM_OC1, TIM_OCM_PWM1); // output compare mode set up to generate PWM1

    timer_enable_counter(TIM5);                     // enable the counter -> the staircase will go up till ARR

    timer_enable_oc_output(TIM5, TIM_OC1);          // set output channel

    timer_set_prescaler(TIM5, PRESCALER - 1);       // set up slowed frequency which the timer will receive

    timer_set_period(TIM5, ARR_VALUE - 1);          // how many ticks of this slowed frequency before the counter resets
}


void timer_pwm_set_duty_ratio(float duty_ratio)
{
    // duty ratio = (CCR / ARR) * 100
    // CCR = duty ratio * ARR / 100
    const float raw_value = ((float) (ARR_VALUE)) * (duty_ratio / 100.0f);
    timer_set_oc_value(TIM5, TIM_OC1, (uint32_t)raw_value);
}
