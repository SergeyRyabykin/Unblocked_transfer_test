#include "nvic.h"
#include "common.h"

#define NVIC_ISER_BASE 0xE000E100
#define NVIC_IPR_BASE  0xE000E400
#define NVIC_ICPR_BASE 0xE000E280

void nvic_enable_irq(unsigned int irq_num, unsigned char priority)
{
    /* Set irq priority */
    *((uint8_t *)(NVIC_IPR_BASE + irq_num)) = (uint8_t)priority;

    /* Enable irq */
    *((uint32_t *)(NVIC_ISER_BASE + (4 * (irq_num/32)))) |= (1 << (irq_num%32));
}

void nvic_clear_irq(unsigned int irq_num)
{
    *((uint32_t *)(NVIC_ICPR_BASE + (4 * (irq_num/32)))) |= (1 << (irq_num%32));
}
