#include "nvic.h"
#include "common.h"

#define NVIC_UART2_IRQ_NUM 38
#define NVIC_DMA1_CH7_IRQ_NUM 17

#define NVIC_ISER_BASE 0xE000E100
#define NVIC_IPR_BASE  0xE000E400
#define NVIC_ICPR_BASE 0xE000E280

#define NVIC_ENABLE_IRQ(num) do { \
    *((uint32_t *)(NVIC_ISER_BASE + (4 * (num/32)))) |= (1 << (num%32)); \
    } while(0)

#define NVIC_SET_PRIORITY(num, prior) do { \
    *((uint8_t *)(NVIC_IPR_BASE + num)) = (uint8_t)prior; \
} while(0)

#define NVIC_CLEAR_IRQ(num) do { \
    *((uint32_t *)(NVIC_ICPR_BASE + (4 * (num/32)))) |= (1 << (num%32)); \
} while(0)

void nvic_enable_uart2_irq(void)
{
    NVIC_SET_PRIORITY(NVIC_UART2_IRQ_NUM, 1);
    NVIC_ENABLE_IRQ(NVIC_UART2_IRQ_NUM);
}

void nvic_clear_uart2_irq(void)
{
    NVIC_CLEAR_IRQ(NVIC_UART2_IRQ_NUM);
}

void nvic_enable_dma1_ch7_irq(void)
{
    NVIC_SET_PRIORITY(NVIC_DMA1_CH7_IRQ_NUM, 1);
    NVIC_ENABLE_IRQ(NVIC_DMA1_CH7_IRQ_NUM);
}

void nvic_clear_dma1_ch7_irq(void)
{
    NVIC_CLEAR_IRQ(NVIC_DMA1_CH7_IRQ_NUM);
}