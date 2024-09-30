#include "CppUTest/TestHarness.h"
#include "dma.h"

#define RCC_AHBENR_OFFS 0x14
#define DMA1EN (1 << 0)

uintptr_t DMA1_BASE = 0;
uintptr_t RCC_BASE = 0;

TEST_GROUP(DMA_Group)
{
    // extern void DMA1_Channel7_IRQHandler(void);
    
    uint32_t dma[400/sizeof(uint32_t)];
    uint32_t rcc[400/sizeof(uint32_t)];
    
    dma_channel_ctx_t ctx;
    dma_channel_ctx_t * const chan_ctx = &ctx;

    void setup()
    {
        memset(dma, 0, sizeof(dma));
        memset(rcc, 0, sizeof(rcc));
        DMA1_BASE = (uintptr_t)dma;
        RCC_BASE = (uintptr_t)rcc;

        ctx.channel = 7;
        ctx.status = (dma_status_t)0xff;
    }

    void teardown()
    {
    }
};

TEST(DMA_Group, DMAInit_Test)
{
    dma_m2p_init(chan_ctx);

    // Check the clocking is enabled
    uintptr_t reg_addr = (uintptr_t)rcc + RCC_AHBENR_OFFS;

    CHECK_EQUAL(0, *((uint32_t *)reg_addr - 1));
    CHECK_EQUAL(0, *((uint32_t *)reg_addr + 1));
    CHECK_EQUAL(DMA1EN, *((uint32_t *)reg_addr));

    //Check the DMA channel settings
    reg_addr = (uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0, *((uint32_t *)reg_addr - 1));
    CHECK_EQUAL(0, *((uint32_t *)reg_addr + 1));
    CHECK_EQUAL((2 << 12) | (1 << 7) | (1 << 4) | (1 << 3) | (1 << 1), *((uint32_t *)reg_addr));

    CHECK_EQUAL(DMA_READY, ctx.status);
}

TEST(DMA_Group, DMAInit_NoRegsCorruption_Test)
{
    uintptr_t reg_addr = (uintptr_t)rcc + RCC_AHBENR_OFFS;

    memset(dma, 0xFF, sizeof(dma));
    memset(rcc, 0xFF, sizeof(rcc));

    *((uint32_t *)reg_addr) = ~(DMA1EN);

    dma_m2p_init(chan_ctx);

    reg_addr = (uintptr_t)rcc + RCC_AHBENR_OFFS;

    CHECK_EQUAL(0xFFFFFFFF, *((uint32_t *)reg_addr - 1));
    CHECK_EQUAL(0xFFFFFFFF, *((uint32_t *)reg_addr + 1));
    CHECK_EQUAL(0xFFFFFFFF, *((uint32_t *)reg_addr));

     reg_addr = (uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0xFFFFFFFF, *((uint32_t *)reg_addr - 1));
    CHECK_EQUAL(0xFFFFFFFF, *((uint32_t *)reg_addr + 1));
    CHECK_EQUAL((2 << 12) | (1 << 7) | (1 << 4) | (1 << 3) | (1 << 1), *((uint32_t *)reg_addr));
}

TEST(DMA_Group, DMAStart_Test)
{
    const uint32_t source = 0xdeadbeaf;
    const uint32_t periph = 0x01020304;
    const uint16_t size = 0x8974;

    memset(dma, 0xaaaaaaaa, sizeof(dma));
    dma_start(chan_ctx, (char *)source, periph, size);

    // Check the peripheral is written to the correct register
    uintptr_t reg_addr = (uintptr_t)dma + 0x10 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(periph, *((uint32_t *)reg_addr));

    // Check the memory address is written to the correct register
    reg_addr = (uintptr_t)dma + 0x14 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(source, *((uint32_t *)reg_addr));

    // Check the data size is written to the correct register
    reg_addr = (uintptr_t)dma + 0xc + (20 * (ctx.channel - 1));

    CHECK_EQUAL(size, *((uint32_t *)reg_addr));

    //Check the DMA channel is enabled
    reg_addr = (uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0xaaaaaaaa | 1, *((uint32_t *)reg_addr));
    CHECK_EQUAL(DMA_BUSY, ctx.status);
}

TEST(DMA_Group, DMAStart_Busy_Test)
{
    const uint32_t source = 0xdeadbeaf;
    const uint32_t periph = 0x01020304;
    const uint16_t size = 0x8974;

    ctx.status = DMA_BUSY;

    dma_start(chan_ctx, (char *)source, periph, size);

    // Check the peripheral is written to the correct register
    uintptr_t reg_addr = (uintptr_t)dma + 0x10 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0, *((uint32_t *)reg_addr));

    // Check the memory address is written to the correct register
    reg_addr = (uintptr_t)dma + 0x14 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0, *((uint32_t *)reg_addr));

    // Check the data size is written to the correct register
    reg_addr = (uintptr_t)dma + 0xc + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0, *((uint32_t *)reg_addr));

    //Check the DMA channel is enabled
    reg_addr = (uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1));

    CHECK_EQUAL(0, *((uint32_t *)reg_addr));

    CHECK_EQUAL(DMA_BUSY, ctx.status);
}

TEST(DMA_Group, DMAClearIrqFlags_Test)
{
    dma_clean_irq(chan_ctx);

    uintptr_t reg_addr = (uintptr_t)dma + 0x4;

    CHECK_EQUAL((uint32_t)(1 << 24), *((uint32_t *)reg_addr));

    // Another channal
    ctx.channel = 3;
    dma_clean_irq(chan_ctx);

    reg_addr = (uintptr_t)dma + 0x4;

    CHECK_EQUAL((uint32_t)(1 << 8), *((uint32_t *)reg_addr));
}

TEST(DMA_Group, DMAGetStatus_Test)
{
    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 27);
    dma_status_t status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_ERROR, status);

    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 25);
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_COMPL, status);

    *((uint32_t *)((uintptr_t)dma + 0x0)) = 0;
    *((uint32_t *)((uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1)))) = 1;
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_BUSY, status);

    *((uint32_t *)((uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1)))) = 0;
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_READY, status);

    // Another channel
    ctx.channel = 5;

    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 19);
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_ERROR, ctx.status);
    CHECK_EQUAL(DMA_ERROR, status);

    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 17);
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_COMPL, ctx.status);
    CHECK_EQUAL(DMA_COMPL, status);

    *((uint32_t *)((uintptr_t)dma + 0x0)) = 0;
    *((uint32_t *)((uintptr_t)dma + 0x8 + (20 * (5 - 1)))) = 1;
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_BUSY, ctx.status);
    CHECK_EQUAL(DMA_BUSY, status);

    *((uint32_t *)((uintptr_t)dma + 0x8 + (20 * (5 - 1)))) = 0;
    status = dma_get_status(chan_ctx);
    CHECK_EQUAL(DMA_READY, ctx.status);
    CHECK_EQUAL(DMA_READY, status);
}
    
TEST(DMA_Group, DMAIrqHandler_Error_Test)
{
    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 27);

    dma_handle_irq(chan_ctx);

    CHECK_EQUAL(DMA_ERROR, ctx.status);

    uintptr_t reg_addr = (uintptr_t)dma + 0x4;
    CHECK_EQUAL((uint32_t)(1 << 24), *((uint32_t *)reg_addr));
}

TEST(DMA_Group, DMAIrqHandler_Complete_Test)
{
    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 25);

    uintptr_t reg_addr = (uintptr_t)dma + 0x8 + (20 * (ctx.channel - 1));
    *((uint32_t *)reg_addr) = 0xaaaaaaaa | 1;

    dma_handle_irq(chan_ctx);

    CHECK_EQUAL(DMA_READY, ctx.status);
    CHECK_EQUAL(0xaaaaaaaa, *((uint32_t *)reg_addr));

    reg_addr = (uintptr_t)dma + 0x4;
    CHECK_EQUAL((uint32_t)(1 << 24), *((uint32_t *)reg_addr));
}

TEST(DMA_Group, DMAIrqHandler_Half_Test)
{
    *((uint32_t *)((uintptr_t)dma + 0x0)) = (1 << 26);

    uintptr_t reg_addr = (uintptr_t)dma + 0x8 + (0x20 * (ctx.channel - 1));
    *((uint32_t *)reg_addr) = 0xaaaaaaaa | 1;

    dma_handle_irq(chan_ctx);

    CHECK_EQUAL(DMA_BUSY, ctx.status);
    CHECK_EQUAL(0xaaaaaaaa | 1, *((uint32_t *)reg_addr));

    reg_addr = (uintptr_t)dma + 0x4;
    CHECK_EQUAL((uint32_t)(1 << 24), *((uint32_t *)reg_addr));
}