#include "uart.h"
#include "common.h"

#define USART1_BASE 0x40013800
#define USART2_BASE 0x40004400

#define USART1EN (1 << 14)
#define USART2EN (1 << 17)
#define IOPAEN   (1 << 2)
#define AFIOEN   (1 << 0)

#define USART_SR_OFS  0x0
#define USART_DR_OFS  0x4
#define USART_BRR_OFS 0x8
#define USART_CR1_OFS 0xC
#define USART_CR2_OFS 0x10

#define CR1_UE (1 << 13)
#define CR1_TE (1 << 3)
#define CR1_TCIE (1 << 6)

void uart_init(void)
{
    /* Enable clock */
    RCC(RCC_APB2ENR) |= IOPAEN | AFIOEN;
    RCC(RCC_APB1ENR) |= USART2EN;

    /* TX - PA2 Alternate function push-pull */
    PORTA(GPIO_CRL) &= ~(0xf << 8);
    PORTA(GPIO_CRL) |= (0xB << 8);
    /* RX - PA3 Input floating/Input pull-up */
    PORTA(GPIO_CRL) &= ~(0xf << 12);
    PORTA(GPIO_CRL) |= (0x4 << 12);

    /* Enable USART2 */
    REG_VAL(USART2_BASE + USART_CR1_OFS) |= CR1_UE;

    /* The word length and stop bits are set by default values as 8 bit and 1 bit correspondingly*/

    /* TODO: Add DMA initialization */

    /* APB frequency must be 8MHz as the clock settings were not used 
    so with these coefficients the baudrate must be 115200 */
    REG_VAL(USART2_BASE + USART_BRR_OFS) = (4 << 4) | 5;

    /* Enable transmitter */
    REG_VAL(USART2_BASE + USART_CR1_OFS) |= CR1_TE | CR1_TCIE;
}

void uart_putc(unsigned char s)
{
    REG_VAL(USART2_BASE + USART_DR_OFS) = s;
}

void uart_clear_irq(void)
{
    REG_VAL(USART2_BASE + USART_SR_OFS) &= ~(CR1_TCIE);
}


