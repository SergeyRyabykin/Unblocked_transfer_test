#include "CppUTest/TestHarness.h"
#include "pin.h"
// #include "CppUTestExt/MockSupport.h"

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
    pin_init((uintptr_t)porte, 0, IN_PD);

    CHECK_EQUAL(0x8, *(static_cast<uint32_t *>(porte + CRL_OFFS)));
    CHECK_EQUAL(0x0, *(static_cast<uint32_t *>(porte + ODR_OFFS)));
}

