#include "print_service.h"
#include "dma.h"
#include "uart.h"
#include "nvic.h"
#include "miscellaneous.h"
#include "common.h"

#define DMA_MAX_DATA_LENGTH UINT16_MAX
#define MAX_STR_LENGTH 1000000

static dma_channel_ctx_t ctx_g = {
    .channel = 7,
    .status = DMA_UNINIT
};

static volatile int32_t data_length_g;
static volatile char *str_position_g;

void print_service_init(void)
{
    /* Enable USART2 clocking */
    RCC(RCC_APB1ENR) |= USART2EN;
    uart_init(USART2_BASE, USART_MODE_DMA);

    dma_m2p_init(&ctx_g);
    nvic_enable_irq(NVIC_DMA1_CH7_IRQ_NUM, 1);
}

ret_code_t put_str(char * str)
{
    uint16_t current_length = 0;

    if(DMA_READY != ctx_g.status) {
        return ERR;
    }

    data_length_g = xstrlen(str);

    if(MAX_STR_LENGTH < data_length_g) {
        data_length_g = MAX_STR_LENGTH;
    }

    if(0 < data_length_g) {
        current_length = (DMA_MAX_DATA_LENGTH < data_length_g) ? DMA_MAX_DATA_LENGTH : data_length_g;
        str_position_g = str;
        dma_start(&ctx_g, str, USART2_BASE + USART_DR_OFS, current_length);
    }
    
    return OK;
}

void DMA1_Channel7_IRQHandler(void)
{
    uint16_t current_length = 0;

    nvic_clear_irq(NVIC_DMA1_CH7_IRQ_NUM);
    dma_handle_irq(&ctx_g);

    data_length_g -= DMA_MAX_DATA_LENGTH;

    if(0 < data_length_g) {
        current_length = (DMA_MAX_DATA_LENGTH < data_length_g) ? DMA_MAX_DATA_LENGTH : data_length_g;
        str_position_g += DMA_MAX_DATA_LENGTH;

        dma_start(&ctx_g, (char *)str_position_g, USART2_BASE + USART_DR_OFS, current_length);
    }
}




/* ONLY FOR TEST PURPOSE TO HAVE ACCESS TO STATIC VARIABLES FROM TESTS */
#ifdef UNIT_TEST

void test_set_dma_channel_context_status(dma_status_t status) 
{
    ctx_g.status = status;
}

dma_channel_ctx_t *test_get_dma_channel_context_ptr(void)
{
    return &ctx_g;
}

volatile int *test_get_data_length_g_ptr(void)
{
    return &data_length_g;
}

#endif
