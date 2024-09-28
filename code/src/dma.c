#include "common.h"
#include "dma.h"

#define DMA1EN (1 << 0)

#define CCR_PL    (2 << 12)
#define CCR_MINC  (1 << 7)
#define CCR_DIR   (1 << 4)
#define CCR_TEIE  (1 << 3)
#define CCR_TCIE  (1 << 1)
#define CCR_EN    (1 << 0)

#define CGIF (1 << 0)

#define TEIF (1 << 3)
#define HTIF (1 << 2)
#define TCIF (1 << 1)

void dma_m2p_init(dma_channel_ctx_t * const ctx)
{
    /* Enable DMA1 clocking */
    RCC(RCC_AHBENR) |= DMA1EN;

    /* Set DMA mode */
    DMA1(DMA_CCR, ctx->channel) = CCR_PL | CCR_MINC | CCR_DIR | CCR_TEIE | CCR_TCIE;

    ctx->status = DMA_READY;
}

void dma_start(dma_channel_ctx_t * const ctx, char const * const source, uint32_t dest, uint16_t size)
{
    /* Set source address from the memory */
    DMA1(DMA_CMAR, ctx->channel) = (uint32_t)source;

    /* Set destination address of the peripheral */
    DMA1(DMA_CPAR, ctx->channel) = dest;

    /* Set destination address of the peripheral */
    DMA1(DMA_CNDTR, ctx->channel) = size;

     /* Enable DMA ctx->channel */
    DMA1(DMA_CCR, ctx->channel) |= CCR_EN;
    ctx->status = DMA_BUSY;
}

void dma_clean_irq(dma_channel_ctx_t const * const ctx)
{
    DMA1_IFCR = (CGIF << (4 * (ctx->channel - 1)));
}

dma_status_t dma_get_status(dma_channel_ctx_t * const ctx)
{
    uint8_t isr = (DMA1_ISR >> ((ctx->channel - 1) * 4));

    if(isr & TEIF) {
        ctx->status = DMA_ERROR;
        return DMA_ERROR;
    }

    if(isr & TCIF) {
        ctx->status = DMA_COMPL;
        return DMA_COMPL;
    }

    if((DMA1(DMA_CCR, ctx->channel) & CCR_EN)) {
        ctx->status = DMA_BUSY;
        return DMA_BUSY;
    }

    ctx->status = DMA_READY;
    return DMA_READY;
}

void dma_handle_irq(dma_channel_ctx_t * const ctx)
{
    uint8_t isr = (DMA1_ISR >> ((ctx->channel - 1) * 4));

    if(isr & TEIF) {
        ctx->status = DMA_ERROR;
    }
    else if(isr & TCIF) {
        DMA1(DMA_CCR, ctx->channel) &= ~CCR_EN;
        ctx->status = DMA_READY;
    }
    else {
        ctx->status = DMA_BUSY;
    }

    dma_clean_irq(ctx);
}