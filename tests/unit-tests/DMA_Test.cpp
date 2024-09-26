#include "CppUTest/TestHarness.h"
#include "dma.h"



TEST_GROUP(DMA_Group)
{
    uint32_t dma[400/sizeof(uint32_t)];
    uint32_t periph = 0;
    unsigned int channel = 1;

    void setup()
    {
        memset(dma, 0, sizeof(dma));
        periph = 0;
        channel = 1;
    }

    void teardown()
    {
    }
};

TEST(DMA_Group, DMAInit_Test)
{
    dma_init(channel, (uintptr_t)&periph);

    CHECK_EQUAL()
}

