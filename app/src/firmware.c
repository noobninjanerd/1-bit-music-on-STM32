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
  
  // super mario bros (overworld) frequency notes and following delays
  uint16_t freqs[] = {660, 660, 660, 510, 660, 770,
                      380, 510, 380, 320, 440, 480,
                      450, 430, 380, 660, 760, 860, 
                      700, 760, 660, 520, 580, 480};

  uint16_t delay_dur[] = {150, 300, 300, 100, 300, 550,
                          575, 450, 400, 500, 300, 330, 
                          150, 300, 200, 200, 150, 300, 
                          150, 350, 300, 150, 150, 500};

  uint32_t start_time = system_get_ticks();
  float duty_ratio = 0.0f;

  timer_pwm_set_duty_ratio_and_frequency(duty_ratio, freqs[0]);
      
  while(1)
  {
    // if(system_get_ticks() - start_time >= 10)
    // {
    //   duty_ratio += 1.0f;
    //   if (duty_ratio > 100.0f)
    //   {
    //     duty_ratio = 0.0f;
    //   }
    //   timer_pwm_set_duty_ratio(duty_ratio);
    //   start_time = system_get_ticks();
    // }

    for (size_t i = 0; i < sizeof(freqs)/sizeof(uint16_t); i++)
    {
      start_time = system_get_ticks();
      duty_ratio = 50.0f;
      timer_pwm_set_duty_ratio_and_frequency(duty_ratio, freqs[i]);
      while (system_get_ticks() - start_time < 100)
      {
        /* spin */
      }

      start_time = system_get_ticks(); 
      duty_ratio = 0.0f;
      timer_pwm_set_duty_ratio_and_frequency(duty_ratio, freqs[i]);  
      while (system_get_ticks() - start_time < (delay_dur[i] - 100))
      {
        /* spin */
      }
    }
  }
        
  // never really return
  return 0;
}