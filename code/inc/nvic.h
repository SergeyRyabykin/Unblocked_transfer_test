#ifndef NVIC_H
#define NVIC_H

#ifdef __cplusplus
extern "C" {
#endif

void nvic_enable_uart2_irq(void);
void nvic_clear_uart2_irq(void);

void nvic_enable_dma1_ch7_irq(void);
void nvic_clear_dma1_ch7_irq(void);

#ifdef __cplusplus
}
#endif

#endif /* NVIC_H */