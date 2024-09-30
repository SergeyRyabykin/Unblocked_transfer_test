#include "led.h"
#include "pin.h"
#include "common.h"
#include "print_service.h"

#define COUNT_VAL 0xfffff

static const char * string = "Hello from the DMA1 and UART2 to test unblocked data transfer.\n";

static void pin_config(void);

int main(void) 
{
  pin_config();
  print_service_init();

  while(1) {
    volatile unsigned long i = 0;

    /* Pause */
    for(i = 0; i < COUNT_VAL; i++) {}

    put_str((char *)string);

    __asm__ ("wfi");
  }

  return 0;
}	

void SystemInit(void)
{
  /* Just a stub to satisfy startup file */
  /* There must be clock settings but the clock remains 8 MHz */
}

static void pin_config(void)
{
  /* Enable PORTA clocking */
  RCC(RCC_APB2ENR) |= IOPAEN;
  
  /* TX - PA2 Alternate function push-pull */
  pin_init(PORTA_BASE, 2, ALT_PP);
  /* RX - PA3 Input floating/Input pull-up */
  pin_init(PORTA_BASE, 3, IN_FLT);
}

