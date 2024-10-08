#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "common.h"
#include "uart.h"
#include "dma.h"
#include "print_service.h"
#include "ret_code.h"
#include "nvic.h"

uintptr_t USART2_BASE = 0x40004400;

extern "C" dma_channel_ctx_t *test_get_dma_channel_context_ptr(void);
extern "C" void test_set_dma_channel_context_status(dma_status_t status);
extern "C" int *test_get_data_length_g_ptr(void);
extern "C" void DMA1_Channel7_IRQHandler(void);

uintptr_t RCC_BASE = 0;

TEST_GROUP(PrintService_Group)
{
    unsigned char dma_channel = 7;
    const char * str = "Test string to test dma_start function\n";
    dma_channel_ctx_t *ctx = NULL;
    int *data_length = NULL;
    uint32_t dest = USART2_BASE + USART_DR_OFS;
    char * long_str = NULL;
    uint32_t rcc[400/sizeof(uint32_t)];


    void setup()
    {
        data_length = test_get_data_length_g_ptr();
        ctx = test_get_dma_channel_context_ptr();
        long_str = new char[2000001];
        RCC_BASE = (uintptr_t)rcc;
    }

    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
        delete[] long_str;
    }
};

TEST(PrintService_Group, PrintServiceInit_Test)
{
    mock().expectOneCall("dma_m2p_init")
        .withParameter("ctx->channel", dma_channel)
        .ignoreOtherParameters();

    mock().expectOneCall("uart_init")
        .withParameter("uart_base", USART2_BASE)
        .withParameter("mode", USART_MODE_DMA);

    mock().expectOneCall("nvic_enable_irq")
        .withParameter("irq_num", 10 + dma_channel)
        .withParameter("priority", 1);

    print_service_init();

    // Check the clocking is enabled
    uintptr_t reg_addr = (uintptr_t)rcc + RCC_APB1ENR_OFS;

    CHECK_EQUAL(USART2EN, *((uint32_t *)reg_addr));
}

TEST(PrintService_Group, PutStr_Test)
{
    mock().strictOrder();

    mock().expectOneCall("xstrlen")
        .withParameter("str", str)
        .andReturnValue(strlen(str));

    mock().expectOneCall("dma_start")
        .withParameter("ctx->channel", 7)
        .withParameter("ctx->status", DMA_READY)
        .withParameter("source", (void *)str)
        .withParameter("dest", dest)
        .withParameter("size", strlen(str))
        .ignoreOtherParameters();


    test_set_dma_channel_context_status(DMA_READY);
    ret_code_t ret = put_str((char *)str);

    CHECK_EQUAL(OK, ret);
}

TEST(PrintService_Group, PutStr_DataLengthIsZero_Test)
{
    const char* str1 = "";

    mock().strictOrder();

    mock().expectOneCall("xstrlen")
        .withParameter("str", str1)
        .andReturnValue(strlen(str1));

    mock().expectNoCall("dma_start");

    test_set_dma_channel_context_status(DMA_READY);

    ret_code_t ret = put_str((char *)str1);

    CHECK_EQUAL(OK, ret);
}

TEST(PrintService_Group, PutStr_DataLengthIs1MB_Test)
{
    mock().strictOrder();

    mock().expectOneCall("xstrlen")
        .withParameter("str", long_str)
        .andReturnValue(1000000);

    mock().expectOneCall("dma_start")
        .withParameter("ctx->channel", 7)
        .withParameter("ctx->status", DMA_READY)
        .withParameter("source", (void *)long_str)
        .withParameter("dest", dest)
        .withParameter("size", UINT16_MAX)
        .ignoreOtherParameters();

    test_set_dma_channel_context_status(DMA_READY);

    ret_code_t ret = put_str(long_str);

    CHECK_EQUAL(OK, ret);
}


TEST(PrintService_Group, PutStr_Error_Test)
{
    mock().expectNoCall("xstrlen");

    mock().expectNoCall("dma_start");

    test_set_dma_channel_context_status(DMA_BUSY);

    ret_code_t ret = put_str((char *)str);

    CHECK_EQUAL(ERR, ret);
}


TEST(PrintService_Group, IRQHandler_Test)
{
    mock().strictOrder();

    mock().expectOneCall("nvic_clear_irq")
        .withParameter("irq_num", NVIC_DMA1_CH7_IRQ_NUM);

    mock().expectOneCall("dma_handle_irq")
        .withParameter("ctx", ctx)
        .ignoreOtherParameters();

    mock().expectNoCall("dma_start");

    *data_length = 0x424F;

    DMA1_Channel7_IRQHandler();
}

TEST(PrintService_Group, IRQHandler_LongString_Test)
{
    int num_irq_handler_calls = 1000000 / UINT16_MAX + 1;
    test_set_dma_channel_context_status(DMA_READY);

    mock().ignoreOtherCalls();

    mock().expectNCalls(num_irq_handler_calls, "nvic_clear_irq")
        .withParameter("irq_num", NVIC_DMA1_CH7_IRQ_NUM);

    mock().expectNCalls(num_irq_handler_calls, "dma_handle_irq")
        .withParameter("ctx", ctx)
        .ignoreOtherParameters();

    for(int i = 0; i < num_irq_handler_calls - 1; i++) {
        mock().expectOneCall("dma_start")
            .withParameter("ctx->channel", 7)
            .withParameter("ctx->status", DMA_READY)
            .withParameter("source", (void *)(long_str + UINT16_MAX * i))
            .withParameter("dest", dest)
            .withParameter("size", UINT16_MAX)
            .ignoreOtherParameters();
    }

    mock().expectOneCall("dma_start")
            .withParameter("ctx->channel", 7)
            .withParameter("ctx->status", DMA_READY)
            .withParameter("source", (void *)(long_str + UINT16_MAX * (num_irq_handler_calls - 1)))
            .withParameter("dest", dest)
            .withParameter("size", 0x424F)
            .ignoreOtherParameters();

    mock().expectOneCall("xstrlen")
        .withParameter("str", long_str)
        .andReturnValue(1000000);

    put_str(long_str);

    do {
        DMA1_Channel7_IRQHandler();
    } while(0 < *data_length);
}

