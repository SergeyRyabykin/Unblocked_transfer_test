#include "led.h"
#include "uart.h"
#include "nvic.h"

#define COUNT_VAL 0xfffff

int main(void) 
{
  led_init();
  uart_init_ti();
  nvic_enable_uart2_irq();


  while(1) {
    volatile unsigned long i = 0;

    for(i = 0; i < COUNT_VAL; ) {
      i++;
    }
    uart_putc(0x30);
  }

  return 0;
}	

void SystemInit(void)
{
  ;
}

void USART2_IRQHandler(void)
{
  nvic_clear_uart2_irq();
  uart_clear_irq();
  led_toggle();
}