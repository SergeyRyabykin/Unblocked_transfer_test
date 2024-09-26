#include "pin.h"
#include "common.h"

#define GPIO_CRL_OFS  0x0
#define GPIO_CRH_OFS  0x4
#define GPIO_IDR_OFS  0x8
#define GPIO_ODR_OFS  0xc
#define GPIO_BSRR_OFS 0x10

void pin_init(uintptr_t port_base, unsigned int pin_num, gpio_mode_t mode)
{
    *((uint32_t *)(port_base + GPIO_CRL_OFS)) = mode & 0xf;
}