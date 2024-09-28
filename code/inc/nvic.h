#ifndef NVIC_H
#define NVIC_H

#ifdef __cplusplus
extern "C" {
#endif

#define NVIC_UART2_IRQ_NUM 38
#define NVIC_DMA1_CH1_IRQ_NUM 11
#define NVIC_DMA1_CH7_IRQ_NUM 17

void nvic_enable_irq(unsigned int irq_num, unsigned char priority);
void nvic_clear_irq(unsigned int irq_num);


#ifdef __cplusplus
}
#endif

#endif /* NVIC_H */