#include "CppUTest/TestHarness.h"
// #include "CppUTestExt/MockSupport.h"

TEST_GROUP(Just_Group)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Just_Group, Fail_Test)
{
   FAIL("Yooops, I failed!");
}

