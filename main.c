#include "led.h"
#include "uart.h"
#include "nvic.h"
#include "dma.h"
#include "miscellaneous.h"

#define COUNT_VAL 0xfffff

static const char * string = "Hello from the DMA1\n";
static dma_channel_ctx_t dma_print = {
  .channel = 7,
  .status = DMA_UNINIT
};


int main(void) 
{
  led_init();
  uart_init_dma();
  dma_m2p_init(&dma_print);
  /* nvic_enable_uart2_irq(); */
  nvic_enable_dma1_ch7_irq();


  while(1) {
    volatile unsigned long i = 0;

    for(i = 0; i < COUNT_VAL; ) {
      i++;
    }
    /* uart_putc(0x30); */
    led_toggle();
    if(DMA_READY == dma_print.status) {
      dma_start(&dma_print, (uint32_t)string, 0x40004404, xstrlen(string));
    }
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
  uart_clear_irq();
  led_toggle();
} */

void DMA1_Channel7_IRQHandler(void)
{
  nvic_clear_dma1_ch7_irq();
  dma_handle_irq(&dma_print);
}