#include <libopencm3/stm32/rcc.h>   // reset and clock control
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h> // system clock
#include <libopencm3/cm3/vector.h>  // IVT

#define LED_PORT      (GPIOA)
#define LED_PIN       (GPIO5)

#define CPU_FREQ      (84000000)
#define SYSTICK_FREQ  (1000)

volatile uint64_t ticks = 0;        // volatile keywords instructs the compiler to not optimize the variable away
void sys_tick_handler(void)
{
  // non-weak implementation of the systick handler
  ticks++;
  // the mc is still a 32-bit mc, so 64-bit addition can't happen in one assembly instruction
  // issue: another interrupt could take place during these two addition instructions
  // atomic operations can't be done over 64-bit values
  // potential solution: turn off all interrupts when we enter the sys_tick_handler
}

static uint64_t get_ticks(void)
{
  return ticks;
}

static void rcc_setup(void)
{
 rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

static void gpio_setup(void)
{
  // by default, every periph that can be used on arm cortex chip is OFF
  rcc_periph_clock_enable(RCC_GPIOA);
  // enabled the clock to the port A with the above function
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

static void systick_setup(void)
{
  systick_set_frequency(1000, 84000000);
  systick_counter_enable();
  // how does the systick let us know that a unit of time has passed?
  // ans: interrupts
  systick_interrupt_enable(); // program control will go to the system_interrupt_handler 1000 times a second
}

int main(void)
{
  rcc_setup();
  gpio_setup();
  systick_setup();

  uint64_t start_time = get_ticks();

  while(1)
  {
    if (get_ticks() - start_time >= 1000)
    {
      gpio_toggle(LED_PORT, LED_PIN);
      start_time = get_ticks();
    }

    // do useful work
  }

  // never really return
  return 0;
}