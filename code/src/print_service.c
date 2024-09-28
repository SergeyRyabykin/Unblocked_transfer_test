#include "print_service.h"
#include "dma.h"
#include "uart.h"
#include "nvic.h"
#include "miscellaneous.h"
#include "common.h"

#define DMA_MAX_DATA_LENGTH UINT16_MAX

static dma_channel_ctx_t ctx_g = {
    .channel = 7,
    .status = DMA_UNINIT
};

static volatile int data_length_g;
static volatile char *str_position_g;



void print_service_init(void)
{
    dma_m2p_init(&ctx_g);
    uart_init(USART2_BASE, USART_MODE_DMA);
    nvic_enable_irq(NVIC_DMA1_CH7_IRQ_NUM, 1);
}

ret_code_t put_str(char * str)
{
    ret_code_t ret = OK;
    uint16_t current_length = 0;

    if(DMA_READY == ctx_g.status) {
        data_length_g = xstrlen(str);
        if(0 < data_length_g) {
            current_length = (UINT16_MAX < data_length_g) ? UINT16_MAX : data_length_g;
            str_position_g = str;
            dma_start(&ctx_g, str, USART2_BASE + USART_DR_OFS, current_length);
        }
    }
    else {
        ret = ERR;
    }

    return ret;
}

void DMA1_Channel7_IRQHandler(void)
{
    nvic_clear_irq(NVIC_DMA1_CH7_IRQ_NUM);
    dma_handle_irq(&ctx_g);
    
    data_length_g -= DMA_MAX_DATA_LENGTH;

    if(0 < data_length_g) {
        dma_start(&ctx_g, (char *)str_position_g, USART2_BASE + USART_DR_OFS, data_length_g);
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
