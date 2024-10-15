#include <libopencm3/stm32/rcc.h>   // reset and clock control
#include <libopencm3/stm32/gpio.h>

#include "core/system.h"
#include "core/timer.h"

#define PORT_A    (GPIOA)
#define LED_PIN   (GPIO5)
#define PWM_PIN   (GPIO0)

static void gpio_setup(void)
{
  // by default, every periph that can be used on arm cortex chip is OFF
  rcc_periph_clock_enable(RCC_GPIOA);
  // enabled the clock to the port A with the above function
  gpio_mode_setup(PORT_A, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

  gpio_mode_setup(PORT_A, GPIO_MODE_AF, GPIO_PUPD_NONE, PWM_PIN);
  gpio_set_af(PORT_A, GPIO_AF2, PWM_PIN);
}

int main(void)
{
  system_setup();
  gpio_setup();
  timer_setup();
  
  uint32_t start_time = system_get_ticks();
  volatile float duty_ratio = 0.0f;

  timer_pwm_set_duty_ratio(duty_ratio);
      
  while(1)
  {
    if(system_get_ticks() - start_time >= 10)
    {
      duty_ratio += 1.0f;
      if (duty_ratio > 100.0f)
      {
        duty_ratio = 0.0f;
      }
      timer_pwm_set_duty_ratio(duty_ratio);
      start_time = system_get_ticks();
     }
  }
        

  // never really return
  return 0;
}