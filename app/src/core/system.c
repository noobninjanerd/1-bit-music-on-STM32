#include <core/system.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h> // system clock
#include <libopencm3/cm3/vector.h>  // IVT


static volatile uint64_t ticks = 0; 
// volatile keywords instructs the compiler to not optimize the variable away

static void systick_setup(void)
{
  systick_set_frequency(1000, 84000000);
  systick_counter_enable();
  // how does the systick let us know that a unit of time has passed?
  // ans: interrupts
  systick_interrupt_enable(); // program control will go to the system_interrupt_handler 1000 times a second
}

static void rcc_setup(void)
{
 rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_84MHZ]);
}

void sys_tick_handler(void)
{
  // non-weak implementation of the systick handler
  ticks++;
  // the mc is still a 32-bit mc, so 64-bit addition can't happen in one assembly instruction
  // issue: another interrupt could take place during these two addition instructions
  // atomic operations can't be done over 64-bit values
  // potential solution: turn off all interrupts when we enter the sys_tick_handler
}

uint64_t system_get_ticks(void)
{
  return ticks;
}

void system_setup(void)
{
    rcc_setup();
    systick_setup();
}
