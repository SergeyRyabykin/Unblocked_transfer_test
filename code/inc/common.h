#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "memorymap.h"

#define REG_VAL(reg) (*(uint32_t *)(reg))

/* RCC */
#define RCC_APB1ENR_OFS  0x1C
#define RCC_APB2ENR_OFS  0x18
#define RCC_APB2RSTR_OFS 0xC
#define RCC_AHBENR_OFS   0x14

/* GPIO */
#define GPIO_CRL_OFS  0x0
#define GPIO_CRH_OFS  0x4
#define GPIO_IDR_OFS  0x8
#define GPIO_ODR_OFS  0xc
#define GPIO_BSRR_OFS 0x10

/* DMA */
#define DMA_ISR_OFS   0x0
#define DMA_IFCR_OFS  0x4
#define DMA_CCR_OFS   0x8
#define DMA_CNDTR_OFS 0xC
#define DMA_CPAR_OFS  0x10
#define DMA_CMAR_OFS  0x14

/* USART */
#define USART_SR_OFS  0x0
#define USART_DR_OFS  0x4
#define USART_BRR_OFS 0x8
#define USART_CR1_OFS 0xC
#define USART_CR2_OFS 0x10
#define USART_CR3_OFS 0x14

/* Register access */
#define RCC(reg)      REG_VAL(RCC_BASE + reg##_OFS)
#define PORTA(reg)    REG_VAL(PORTA_BASE + reg##_OFS)
#define PORTE(reg)    REG_VAL(PORTE_BASE + reg##_OFS)
#define DMA1(reg, ch) REG_VAL(DMA1_BASE + reg##_OFS + 20 * (ch - 1))
#define DMA1_IFCR     REG_VAL(DMA1_BASE + DMA_IFCR_OFS)
#define DMA1_ISR      REG_VAL(DMA1_BASE + DMA_ISR_OFS)



#endif /* COMMON_H */