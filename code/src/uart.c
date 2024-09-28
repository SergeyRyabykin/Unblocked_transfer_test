#include "uart.h"
#include "common.h"

#define CR1_UE (1 << 13)
#define CR1_TE (1 << 3)
#define CR1_TCIE (1 << 6)

#define CR3_DMAT (1 << 7)

void uart_init(uintptr_t uart_base, uart_mode_t mode)
{
    /* Enable USART */
    REG_VAL(uart_base + USART_CR1_OFS) |= CR1_UE;

    if(USART_MODE_DMA == mode) {
        /* DMA transmiter initialization */
        REG_VAL(USART2_BASE + USART_CR3_OFS) |= CR3_DMAT;
    }

    /* APB frequency must be 8MHz as the clock settings were not used 
    so with these coefficients the baudrate must be 115200 */
    REG_VAL(USART2_BASE + USART_BRR_OFS) = (4 << 4) | 5;

    /* Enable complete interrupt if IT mode */
    if(USART_MODE_IT == mode) {
        REG_VAL(USART2_BASE + USART_CR1_OFS) |= CR1_TCIE;
    }

    /* Enable transmitter */
    REG_VAL(USART2_BASE + USART_CR1_OFS) |= CR1_TE;
}

void uart_putc(uintptr_t uart_base, unsigned char s)
{
    REG_VAL(uart_base + USART_DR_OFS) = s;
}

void uart_clear_irq(uintptr_t uart_base)
{
    REG_VAL(uart_base + USART_SR_OFS) &= ~(CR1_TCIE);
}


