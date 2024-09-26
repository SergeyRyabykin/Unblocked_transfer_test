#ifndef UART_H
#define UART_H


void uart_init(void);
void uart_putc(unsigned char s);
void uart_clear_irq(void);

#endif /* UART_H */