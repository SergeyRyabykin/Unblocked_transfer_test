#ifndef NVIC_H
#define NVIC_H

void nvic_enable_uart2_irq(void);
void nvic_clear_uart2_irq(void);

void nvic_enable_dma1_ch7_irq(void);
void nvic_clear_dma1_ch7_irq(void);

#endif /* NVIC_H */