#include "CppUTest/TestHarness.h"
#include "pin.h"

constexpr uintptr_t CRL_OFFS = 0x0;
constexpr uintptr_t CRH_OFFS = 0x4;
constexpr uintptr_t ODR_OFFS = 0xC;

TEST_GROUP(Pin_Group)
{
    uint32_t porte[400/sizeof(uint32_t)];

    void setup()
    {
        memset(porte, 0, sizeof(porte));
    }

    void teardown()
    {
    }
};

TEST(Pin_Group, Init_Test)
{
    uintptr_t reg_addr = (uintptr_t)porte + CRL_OFFS;
    uintptr_t odr_addr = (uintptr_t)porte + ODR_OFFS;
    *(reinterpret_cast<uint32_t *>(reg_addr)) = 1 << 31;
    *(reinterpret_cast<uint32_t *>(odr_addr)) = 1;
    pin_init((uintptr_t)porte, 0, IN_PD);

    CHECK_EQUAL((uint32_t)(0x8 | (1 << 31)), *(reinterpret_cast<uint32_t *>(reg_addr)));
    CHECK_EQUAL(0x0, *(reinterpret_cast<uint32_t *>(odr_addr)));

    pin_init((uintptr_t)porte, 7, OUT_PP);

    CHECK_EQUAL((uint32_t)(0x3 << 28) | 0x8, *(static_cast<uint32_t *>(porte + CRL_OFFS)));
    CHECK_EQUAL(0x0, *(reinterpret_cast<uint32_t *>(odr_addr)));

    reg_addr = (uintptr_t)porte + CRH_OFFS;
    *(reinterpret_cast<uint32_t *>(reg_addr)) = 1 | (0xf << 8);
    *(reinterpret_cast<uint32_t *>(odr_addr)) = 1;
    pin_init((uintptr_t)porte, 10, ALT_PP);

    CHECK_EQUAL((uint32_t)(0xb << 8) | 1, *(reinterpret_cast<uint32_t *>(reg_addr)));
    CHECK_EQUAL(0x1, *(reinterpret_cast<uint32_t *>(odr_addr)));

    pin_init((uintptr_t)porte, 15, IN_PP);

    CHECK_EQUAL((uint32_t)(0xb << 8) | 1 | (0x8 << 28), *(reinterpret_cast<uint32_t *>(reg_addr)));
    CHECK_EQUAL((uint32_t)(1 << 15) | 1, *(reinterpret_cast<uint32_t *>(odr_addr)));
}

