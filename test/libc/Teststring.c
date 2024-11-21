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

void test_intToString()
{
    char outString[11];
    memset(outString, '\0', 11);

    int count = intToString(123, outString);

    TEST_ASSERT_EQUAL(3, count);
    TEST_ASSERT_EQUAL_STRING("123", outString);
}

void test_intToString_negative()
{
    char outString[11];
    memset(outString, '\0', 11);

    int count = intToString(-123, outString);

    TEST_ASSERT_EQUAL(4, count);
    TEST_ASSERT_EQUAL_STRING("-123", outString);
}

void test_intToString_zero()
{
    char outString[11];
    memset(outString, '\0', 11);

    int count = intToString(0, outString);

    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL_STRING("0", outString);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_strlen);
    RUN_TEST(test_strlen_zero);

    RUN_TEST(test_intToString);
    RUN_TEST(test_intToString_negative);
    RUN_TEST(test_intToString_zero);

    UNITY_END();

    return 0;
}