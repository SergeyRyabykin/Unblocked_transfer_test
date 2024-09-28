#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum {
    USART_MODE_IT,
    USART_MODE_DMA
} uart_mode_t;

void uart_init(uintptr_t uart_base, uart_mode_t mode);
void uart_putc(uintptr_t uart_base, unsigned char s);
void uart_clear_irq(uintptr_t uart_base);

#ifdef __cplusplus
}
#endif

#endif /* UART_H */