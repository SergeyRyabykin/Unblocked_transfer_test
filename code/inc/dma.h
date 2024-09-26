#ifndef DMA_H
#define DMA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

void dma_init(unsigned int chan_num, uintptr_t dest_addr);

#ifdef __cplusplus
}
#endif

#endif /* DMA_H */