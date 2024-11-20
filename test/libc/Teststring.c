#include "unity.h"
#include "string.h"

void setUp(){}
void tearDown(){}

void test_strlen()
{
    size_t output = strlen("123");
    TEST_ASSERT_EQUAL(3, output);
}

void test_strlen_zero()
{
    size_t output = strlen("");
    TEST_ASSERT_EQUAL(0, output);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_strlen);
    RUN_TEST(test_strlen_zero);

    UNITY_END();

    return 0;
}