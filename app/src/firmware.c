#include <libopencm3/stm32/rcc.h>   // reset and clock control
#include <libopencm3/stm32/gpio.h>

#include "core/system.h"

#define LED_PORT      (GPIOA)
#define LED_PIN       (GPIO5)

static void gpio_setup(void)
{
  // by default, every periph that can be used on arm cortex chip is OFF
  rcc_periph_clock_enable(RCC_GPIOA);
  // enabled the clock to the port A with the above function
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

int main(void)
{
  system_setup();
  gpio_setup();

  uint64_t start_time = system_get_ticks();

  while(1)
  {
    if (system_get_ticks() - start_time >= 100)
    {
      gpio_toggle(LED_PORT, LED_PIN);
      start_time = system_get_ticks();
    }

    // do useful work
  }

  // never really return
  return 0;
}