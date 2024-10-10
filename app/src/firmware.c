#include <libopencm3/stm32/rcc.h> // reset and clock control
#include <libopencm3/stm32/gpio.h>

#define LED_PORT  (GPIOA)
#define LED_PIN   (GPIO5)

static void rcc_setup(void)
{
 rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

static void gpio_setup(void)
{
  // by default, every periph that can be used on arm cortex chip is OFF
  rcc_periph_clock_enable(RCC_GPIOA);
  // enabled the clock
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

static void delay_cycles(uint32_t cycles)
{
  for (uint32_t i = 0; i < cycles; i++)
  {
    // we have to make sure the compiler doesnt optimize this code
    // given the integer declaration, comparison and increment, this loop may
    // take rougly 4 clock cycles
    __asm__("nop");
  }
}

int main(void)
{
  rcc_setup();
  gpio_setup();

  while(1)
  {
    gpio_toggle(LED_PORT, LED_PIN);
    delay_cycles(84000000 / 4); // so our eyes can perceive the on-off in the LED
  }

  // never really return
  return 0;
}