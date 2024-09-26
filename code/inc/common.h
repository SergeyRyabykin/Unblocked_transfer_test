#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#define REG_VAL(reg) (*(uint32_t *)(reg))

#define RCC_BASE         (uint32_t)0x40021000
#define RCC_APB1ENR_OFS  0x1C
#define RCC_APB1ENR      REG_VAL(RCC_BASE + RCC_APB1ENR_OFS)
#define RCC_APB2ENR_OFS  0x18
#define RCC_APB2ENR      REG_VAL(RCC_BASE + RCC_APB2ENR_OFS)
#define RCC_APB2RSTR_OFS 0xC
#define RCC_APB2RSTR     REG_VAL(RCC_BASE + RCC_APB2RSTR_OFS)

#define GPIO_CRL_OFS  0x0
#define GPIO_CRH_OFS  0x4
#define GPIO_IDR_OFS  0x8
#define GPIO_ODR_OFS  0xc
#define GPIO_BSRR_OFS 0x10

#define PORTA_BASE (uint32_t)0x40010800
#define PORTE_BASE (uint32_t)0x40011800


#define PORTA(reg) REG_VAL(PORTA_BASE + reg##_OFS)
#define PORTE(reg) REG_VAL(PORTE_BASE + reg##_OFS)


#endif /* COMMON_H */