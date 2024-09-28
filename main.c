#include "led.h"
#include "pin.h"
#include "common.h"
#include "print_service.h"

#define COUNT_VAL 0xfffff

#define USART2EN (1 << 17)
#define IOPAEN   (1 << 2)

static const char * string = "Hello from the DMA1\n";

int main(void) 
{
  /* Enable PORTA clocking */
  RCC(RCC_APB2ENR) |= IOPAEN;
  /* Enable USART2 clocking */
  RCC(RCC_APB1ENR) |= USART2EN;
  /* TX - PA2 Alternate function push-pull */
  pin_init(PORTA_BASE, 2, ALT_PP);
  /* RX - PA3 Input floating/Input pull-up */
  pin_init(PORTA_BASE, 3, IN_FLT);

  led_init();
  print_service_init();
 

  while(1) {
    volatile unsigned long i = 0;

    for(i = 0; i < COUNT_VAL; ) {
      i++;
    }
    /* uart_putc(USART2_BASE, 0x31); */
    put_str((char *)string);
    
    led_toggle();
    __asm__ ("wfi");
  }

  return 0;
}	

void SystemInit(void)
{
  ;
}


/* void USART2_IRQHandler(void)
{
  nvic_clear_uart2_irq();
  uart_clear_irq(USART2_BASE);
  led_toggle();
} */

