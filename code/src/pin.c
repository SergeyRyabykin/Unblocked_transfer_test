#include "pin.h"
#include "common.h"

void pin_init(uintptr_t port_base, unsigned int pin_num, gpio_mode_t mode)
{
    uintptr_t reg_addr = 0;
    unsigned int offset = 0;
    uint32_t odr_val = 0;

    if(8 > pin_num) {
        reg_addr = port_base + GPIO_CRL_OFS;
        offset = pin_num * 4;
    }
    else {
        reg_addr = port_base + GPIO_CRH_OFS;
        offset = (pin_num - 8) * 4;
    }

    odr_val = (1 << pin_num);

    if(mode & ~0xf) {
        REG_VAL(port_base + GPIO_ODR_OFS) |= odr_val;
    }
    else {
        REG_VAL(port_base + GPIO_ODR_OFS) &= ~odr_val;
    }


    REG_VAL(reg_addr) &= ~(0xf << offset);
    REG_VAL(reg_addr) |= (mode & 0xf) << offset;
}