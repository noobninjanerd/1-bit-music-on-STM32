#include "core/timer.h"
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

// freq = process_freq / (prescaler-1) * (ARR - 1)

#define CPU_FREQ    (84000000)
#define PRESCALER   (84)
#define ARR_VALUE   (1000)
#define FREQ_COEFF  (CPU_FREQ/(PRESCALER - 1))

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

    timer_enable_preload(TIM5);
    timer_enable_oc_preload(TIM5, TIM_OC1);

    timer_set_prescaler(TIM5, PRESCALER - 1);       // set up slowed frequency which the timer will receive
    timer_set_period(TIM5, ARR_VALUE - 1);          // how many ticks of this slowed frequency before the counter resets
}


void timer_pwm_set_duty_ratio_and_frequency(float duty_ratio, uint16_t freq_val)
{
    uint16_t arr_val = (uint16_t)(FREQ_COEFF * 1/freq_val) + 1;
    // uint16_t prescaler_val = (uint16_t)(FREQ_COEFF * 1/freq_val) + 1;
    timer_set_period(TIM5,  arr_val - 1);

    // duty ratio = (CCR / ARR) * 100
    // CCR = duty ratio * ARR / 100
    const float raw_value = ((float) (arr_val)) * (duty_ratio / 100.0f);
    timer_set_oc_value(TIM5, TIM_OC1, (uint32_t)raw_value);
}
