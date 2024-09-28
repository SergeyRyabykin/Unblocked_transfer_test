#include "CppUTest/TestHarness.h"
#include "uart.h"

uintptr_t USART2_BASE = 0;

constexpr unsigned int SR_OFS = 0x0;
constexpr unsigned int DR_OFS = 0x4;
constexpr unsigned int CR1_OFS = 0xC;
constexpr unsigned int CR3_OFS = 0x14;
constexpr unsigned int BRR_OFS = 0x8;


constexpr uint32_t UE = (1 << 13);
constexpr uint32_t TC = (1 << 6);
constexpr uint32_t TE = (1 << 3);
constexpr uint32_t TCIE = (1 << 6);
constexpr uint32_t DMAT = (1 << 7);

TEST_GROUP(Uart_Group)
{
    uint32_t uart[400/sizeof(uint32_t)];
    uintptr_t base_addr = 0;

    void setup()
    {
        memset(uart, 0, sizeof(uart));
        USART2_BASE = (uintptr_t)uart;
    }

    void teardown()
    {
    }
};

TEST(Uart_Group, UartInit_DMAMode_Test)
{
    *(reinterpret_cast<uint32_t *>(USART2_BASE + CR1_OFS)) = (1 << 16);
    *(reinterpret_cast<uint32_t *>(USART2_BASE + CR3_OFS)) = (1 << 16);
    uart_init(USART2_BASE, USART_MODE_DMA);

    CHECK_EQUAL(TE | UE | (1 << 16), *(reinterpret_cast<uint32_t *>(USART2_BASE + CR1_OFS)));
    CHECK_EQUAL(DMAT | (1 << 16), *(reinterpret_cast<uint32_t *>(USART2_BASE + CR3_OFS)));
    CHECK_EQUAL(static_cast<uint32_t>((4 << 4) | 5), *(reinterpret_cast<uint32_t *>(USART2_BASE + BRR_OFS)));
}

TEST(Uart_Group, UartInit_ITMode_Test)
{
    *(reinterpret_cast<uint32_t *>(USART2_BASE + CR1_OFS)) = (1 << 16);
    *(reinterpret_cast<uint32_t *>(USART2_BASE + CR3_OFS)) = (1 << 16);
    uart_init(USART2_BASE, USART_MODE_IT);

    CHECK_EQUAL(TE | TCIE | UE | (1 << 16), *(reinterpret_cast<uint32_t *>(USART2_BASE + CR1_OFS)));
    CHECK_EQUAL((1 << 16), *(reinterpret_cast<uint32_t *>(USART2_BASE + CR3_OFS)));
    CHECK_EQUAL(static_cast<uint32_t>((4 << 4) | 5), *(reinterpret_cast<uint32_t *>(USART2_BASE + BRR_OFS)));
}

TEST(Uart_Group, UartPutc_Test)
{
    unsigned char sym = 0x15;
    uart_putc(USART2_BASE, sym);

    CHECK_EQUAL(sym, *(reinterpret_cast<uint32_t *>(USART2_BASE + DR_OFS)));
}

TEST(Uart_Group, UartClearIrq_Test)
{
    *(reinterpret_cast<uint32_t *>(USART2_BASE + SR_OFS)) = 0xffffffff;

    uart_clear_irq(USART2_BASE);

    CHECK_EQUAL(~TC, *(reinterpret_cast<uint32_t *>(USART2_BASE + SR_OFS)));
}

