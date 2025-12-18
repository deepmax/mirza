#include "tests.h"
#include "../buffer.h"
#include "../types.h"
#include <stdlib.h>
#include <string.h>

static void test_buffer_init(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 10);
    TEST_ASSERT_NOT_NULL(buf.data, "buffer data should be non-NULL");
    TEST_ASSERT_EQ(buf.allc, 10, "buffer allocation should be 10");
    TEST_ASSERT_EQ(buf.used, 0, "buffer used should be 0");
    TEST_ASSERT_EQ(buffer_size(&buf), 0, "buffer_size should return 0");
    buffer_free(&buf);
}

static void test_buffer_add(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 2);
    
    uint8_t val1 = 10;
    uint8_t val2 = 20;
    uint8_t val3 = 30;
    
    size_t size1 = buffer_add(&buf, val1);
    TEST_ASSERT_EQ(size1, 1, "buffer_add should return 1");
    TEST_ASSERT_EQ(buffer_size(&buf), 1, "buffer_size should be 1");
    TEST_ASSERT_EQ(buffer_get(&buf, 0), val1, "get(0) should return val1");
    
    size_t size2 = buffer_add(&buf, val2);
    TEST_ASSERT_EQ(size2, 2, "buffer_add should return 2");
    TEST_ASSERT_EQ(buffer_size(&buf), 2, "buffer_size should be 2");
    TEST_ASSERT_EQ(buffer_get(&buf, 1), val2, "get(1) should return val2");
    
    size_t size3 = buffer_add(&buf, val3);
    TEST_ASSERT_EQ(size3, 3, "buffer_add should return 3");
    TEST_ASSERT_EQ(buffer_size(&buf), 3, "buffer_size should be 3");
    TEST_ASSERT_EQ(buffer_get(&buf, 2), val3, "get(2) should return val3");
    
    buffer_free(&buf);
}

static void test_buffer_set_get(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 5);
    
    uint8_t val1 = 100;
    uint8_t val2 = 200;
    uint8_t val3 = 44;  // 300 % 256
    
    buffer_set(&buf, 0, val1);
    buffer_set(&buf, 1, val2);
    buffer_set(&buf, 2, val3);
    
    TEST_ASSERT_EQ(buffer_get(&buf, 0), val1, "get(0) should return val1");
    TEST_ASSERT_EQ(buffer_get(&buf, 1), val2, "get(1) should return val2");
    TEST_ASSERT_EQ(buffer_get(&buf, 2), val3, "get(2) should return val3");
    
    buffer_set(&buf, 1, val3);
    TEST_ASSERT_EQ(buffer_get(&buf, 1), val3, "get(1) should return val3 after set");
    
    buffer_free(&buf);
}

static void test_buffer_adds(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 5);
    
    uint8_t items[] = {10, 20, 30, 40, 50};
    size_t len = sizeof(items) / sizeof(items[0]);
    
    size_t size = buffer_adds(&buf, items, len);
    TEST_ASSERT_EQ(size, len, "buffer_adds should return len");
    TEST_ASSERT_EQ(buffer_size(&buf), len, "buffer_size should be len");
    
    for (size_t i = 0; i < len; i++) {
        TEST_ASSERT_EQ(buffer_get(&buf, i), items[i], "get(i) should match items[i]");
    }
    
    buffer_free(&buf);
}

static void test_buffer_sets(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 10);
    
    uint8_t items[] = {100, 200, 44};  // 300 % 256 = 44
    size_t len = sizeof(items) / sizeof(items[0]);
    
    buffer_sets(&buf, 0, items, len);
    
    for (size_t i = 0; i < len; i++) {
        TEST_ASSERT_EQ(buffer_get(&buf, i), items[i], "get(i) should match items[i]");
    }
    
    uint8_t items2[] = {50, 60};
    buffer_sets(&buf, 1, items2, 2);
    TEST_ASSERT_EQ(buffer_get(&buf, 0), 100, "get(0) should still be 100");
    TEST_ASSERT_EQ(buffer_get(&buf, 1), 50, "get(1) should be 50");
    TEST_ASSERT_EQ(buffer_get(&buf, 2), 60, "get(2) should be 60");
    
    buffer_free(&buf);
}

static void test_buffer_shrink(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 10);
    
    uint8_t val1 = 10;
    uint8_t val2 = 20;
    
    buffer_add(&buf, val1);
    buffer_add(&buf, val2);
    
    TEST_ASSERT_EQ(buf.allc, 10, "allocation should be 10");
    TEST_ASSERT_EQ(buf.used, 2, "used should be 2");
    
    buffer_shrink(&buf);
    TEST_ASSERT_EQ(buf.allc, 2, "allocation should be 2 after shrink");
    TEST_ASSERT_EQ(buf.used, 2, "used should still be 2");
    TEST_ASSERT_EQ(buffer_get(&buf, 0), val1, "get(0) should still be val1");
    TEST_ASSERT_EQ(buffer_get(&buf, 1), val2, "get(1) should still be val2");
    
    buffer_free(&buf);
}

static void test_buffer_clear(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 10);
    
    uint8_t items[] = {10, 20, 30, 40, 50};
    buffer_adds(&buf, items, sizeof(items) / sizeof(items[0]));
    
    TEST_ASSERT_EQ(buffer_size(&buf), 5, "size should be 5");
    
    buffer_clear(&buf);
    TEST_ASSERT_EQ(buffer_size(&buf), 0, "size should be 0 after clear");
    TEST_ASSERT_EQ(buf.used, 0, "used should be 0 after clear");
    TEST_ASSERT_EQ(buf.allc, 32, "allocation should be 32 after clear");
    
    buffer_free(&buf);
}

static void test_buffer_auto_resize(test_suite_t* suite)
{
    buffer_t buf;
    buffer_init(&buf, 2);
    
    uint8_t items[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t len = sizeof(items) / sizeof(items[0]);
    
    for (size_t i = 0; i < len; i++) {
        buffer_add(&buf, items[i]);
    }
    
    TEST_ASSERT_EQ(buffer_size(&buf), len, "size should be len");
    TEST_ASSERT_NE(buf.allc, 2, "allocation should have grown");
    
    for (size_t i = 0; i < len; i++) {
        TEST_ASSERT_EQ(buffer_get(&buf, i), items[i], "get(i) should match items[i]");
    }
    
    buffer_free(&buf);
}

int main(void)
{
    RUN_SUITE("buffer",
        {"buffer_init", test_buffer_init},
        {"buffer_add", test_buffer_add},
        {"buffer_set_get", test_buffer_set_get},
        {"buffer_adds", test_buffer_adds},
        {"buffer_sets", test_buffer_sets},
        {"buffer_shrink", test_buffer_shrink},
        {"buffer_clear", test_buffer_clear},
        {"buffer_auto_resize", test_buffer_auto_resize}
    );
    
    printf("All buffer tests passed!\n");
    return 0;
}

