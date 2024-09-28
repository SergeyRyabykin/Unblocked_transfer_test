#include "CppUTestExt/MockSupport.h"
#include "dma.h"
#include "uart.h"
#include "nvic.h"
#include "miscellaneous.h"

//------------------------------------------- UART -----------------------------------
void uart_init(uintptr_t uart_base, uart_mode_t mode)
{
    mock().actualCall("uart_init")
        .withParameter("uart_base", uart_base)
        .withParameter("mode", mode);
}

//------------------------------------------- DMA -----------------------------------
void dma_m2p_init(dma_channel_ctx_t * const ctx)
{
    mock().actualCall("dma_m2p_init")
        .withParameter("ctx->channel", ctx->channel)
        .withParameter("ctx->status", ctx->status)
        .withParameter("ctx", ctx);

    ctx->status = DMA_READY;
}

void dma_start(dma_channel_ctx_t * const ctx, char const * const source, uint32_t dest, uint16_t size)
{
    mock().actualCall("dma_start")
        .withParameter("ctx", ctx)
        .withParameter("ctx->channel", ctx->channel)
        .withParameter("ctx->status", ctx->status)
        .withParameter("source", source)
        .withParameter("dest", dest)
        .withParameter("size", size);
}

void dma_clean_irq(dma_channel_ctx_t const * const ctx)
{
    mock().actualCall("dma_clean_irq")
        .withParameter("ctx->channel", ctx->channel)
        .withParameter("ctx->status", ctx->status)
        .withParameter("ctx", ctx);
}

void dma_handle_irq(dma_channel_ctx_t * const ctx)
{
    mock().actualCall("dma_handle_irq")
        .withParameter("ctx->channel", ctx->channel)
        .withParameter("ctx->status", ctx->status)
        .withParameter("ctx", ctx);
}


//------------------------------------------- NVIC -----------------------------------
void nvic_enable_irq(unsigned int irq_num, unsigned char priority)
{
    mock().actualCall("nvic_enable_irq")
        .withParameter("irq_num", irq_num)
        .withParameter("priority", priority);
}

void nvic_clear_irq(unsigned int irq_num)
{
    mock().actualCall("nvic_clear_irq")
        .withParameter("irq_num", irq_num);
}

//------------------------------------------- MISCELLANEOUS -----------------------------------
size_t xstrlen(char const * const str)
{
    return mock().actualCall("xstrlen")
        .withParameter("str", str)
        .returnUnsignedLongLongIntValueOrDefault(0);
}


