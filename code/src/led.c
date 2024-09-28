#include "led.h"
#include "common.h"

/* #define GPIO_CRL  (PORTE_BASE + GPIO_CRL_OFS)
#define GPIO_CRH  (PORTE_BASE + GPIO_CRH_OFS)
#define GPIO_IDR  (PORTE_BASE + GPIO_IDR_OFS)
#define GPIO_ODR  (PORTE_BASE + GPIO_ODR_OFS)
#define GPIO_BSRR (PORTE_BASE + GPIO_BSRR_OFS) */

#define IOPEEN (1 << 6)

void led_init(void)
{
    /* Enable clock */
    RCC(RCC_APB2ENR) |= IOPEEN;

    /* Enable pad */
    PORTE(GPIO_BSRR) = 1;

    PORTE(GPIO_CRL) &= ~0xf;
    PORTE(GPIO_CRL) |= 0x7;
}

void led_on(void)
{
    PORTE(GPIO_BSRR) = (1 << 16);

}

void led_off(void)
{
    PORTE(GPIO_BSRR) = 1;
}

void led_toggle(void)
{
    if(PORTE(GPIO_ODR) & 1) {
        PORTE(GPIO_BSRR) = (1 << 16);
    }
    else {
        PORTE(GPIO_BSRR) = 1;
    }
}
