#include "led.h"
#include "uart.h"
#include "nvic.h"
#include "dma.h"
#include "pin.h"
#include "common.h"
#include "miscellaneous.h"

#define COUNT_VAL 0xfffff

#define USART2EN (1 << 17)
#define IOPAEN   (1 << 2)

static const char * string = "Hello from the DMA1\n";

static dma_channel_ctx_t dma_print = {
  .channel = 7,
  .status = DMA_UNINIT
};


int main(void) 
{
  /* Enable PORTA clocking */
  RCC(RCC_APB2ENR) |= IOPAEN/*  | AFIOEN */;
  /* Enable USART2 clocking */
  RCC(RCC_APB1ENR) |= USART2EN;
  /* TX - PA2 Alternate function push-pull */
  pin_init(PORTA_BASE, 2, ALT_PP);
  /* RX - PA3 Input floating/Input pull-up */
  pin_init(PORTA_BASE, 3, IN_FLT);

  led_init();
  uart_init(USART2_BASE, USART_MODE_DMA);
  /* uart_init(USART2_BASE, USART_MODE_IT); */
  dma_m2p_init(&dma_print);
  /* nvic_enable_uart2_irq(); */
  nvic_enable_dma1_ch7_irq();

  while(1) {
    volatile unsigned long i = 0;

    for(i = 0; i < COUNT_VAL; ) {
      i++;
    }
    /* uart_putc(USART2_BASE, 0x31); */

    if(DMA_READY == dma_print.status) {
      dma_start(&dma_print, string, 0x40004404, xstrlen(string));
    }
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

void DMA1_Channel7_IRQHandler(void)
{
  nvic_clear_dma1_ch7_irq();
  dma_handle_irq(&dma_print);
}