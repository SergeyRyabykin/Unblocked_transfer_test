#ifndef DMA_H
#define DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

typedef enum {
    DMA_UNINIT,
    DMA_READY,
    DMA_ERROR,
    DMA_COMPL,
    DMA_BUSY,
} dma_status_t;

typedef struct {
    unsigned char channel;
    volatile dma_status_t status;
} dma_channel_ctx_t;

void dma_m2p_init(dma_channel_ctx_t * const ctx);
void dma_start(dma_channel_ctx_t * const ctx, char const * const source, uint32_t dest, uint16_t size);
void dma_clean_irq(dma_channel_ctx_t const * const ctx);

/**
 * @brief Function to check actual DMA channel status
 * @warning The value may be diffrent from the latest state if used in IT mode
 * 
 * @param ctx DMA channel context
 * @return dma_status_t Current status of DMA channel
 */
dma_status_t dma_get_status(dma_channel_ctx_t * const ctx);
void dma_handle_irq(dma_channel_ctx_t * const ctx);


#ifdef __cplusplus
}
#endif

#endif /* DMA_H */