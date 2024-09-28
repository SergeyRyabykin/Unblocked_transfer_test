#include "CppUTest/TestHarness.h"
#include "miscellaneous.h"
#include "string.h"



TEST_GROUP(Miscellaneous_Group)
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST(Miscellaneous_Group, Xltrlen_Test)
{
    const char *str = "Hello from DMA\n";

    size_t str1_len_lib = strlen(str);
    size_t str1_len_x = xstrlen(str);

    CHECK_EQUAL(str1_len_lib, str1_len_x);
    

    char * long_str = new char[2000001];

    if(long_str) {
        memset(long_str, 0x30, 2000000);
        long_str[2000000] = '\0';

        str1_len_lib = strlen(long_str);
        str1_len_x = xstrlen(long_str);

        CHECK_EQUAL(2000000, str1_len_lib);
        CHECK_EQUAL(1000000, str1_len_x);

        delete[] long_str;
    }
    else {
        FAIL("Memory allocation failed");
    }
}

