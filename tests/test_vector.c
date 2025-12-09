#include "tests.h"
#include "../vector.h"
#include <stdlib.h>

static void test_vec_new(test_suite_t* suite)
{
    vector_t* vec = vec_new(10);
    TEST_ASSERT_NOT_NULL(vec, "vec_new should return non-NULL");
    TEST_ASSERT_EQ(vec_size(vec), 0, "new vector should have size 0");
    vec_free(vec);
}

static void test_vec_append(test_suite_t* suite)
{
    vector_t* vec = vec_new(2);
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    vec_append(vec, &val1);
    TEST_ASSERT_EQ(vec_size(vec), 1, "size should be 1 after first append");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 0), &val1, "first element should be val1");
    
    vec_append(vec, &val2);
    TEST_ASSERT_EQ(vec_size(vec), 2, "size should be 2 after second append");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 1), &val2, "second element should be val2");
    
    vec_append(vec, &val3);
    TEST_ASSERT_EQ(vec_size(vec), 3, "size should be 3 after third append");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 2), &val3, "third element should be val3");
    
    vec_free(vec);
}

static void test_vec_get_set(test_suite_t* suite)
{
    vector_t* vec = vec_new(5);
    int val1 = 100;
    int val2 = 200;
    int val3 = 300;
    
    vec_append(vec, &val1);
    vec_append(vec, &val2);
    
    TEST_ASSERT_PTR_EQ(vec_get(vec, 0), &val1, "get(0) should return val1");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 1), &val2, "get(1) should return val2");
    
    vec_set(vec, 0, &val3);
    TEST_ASSERT_PTR_EQ(vec_get(vec, 0), &val3, "get(0) should return val3 after set");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 1), &val2, "get(1) should still return val2");
    
    vec_free(vec);
}

static void test_vec_first_last(test_suite_t* suite)
{
    vector_t* vec = vec_new(5);
    int val1 = 1;
    int val2 = 2;
    int val3 = 3;
    
    vec_append(vec, &val1);
    vec_append(vec, &val2);
    vec_append(vec, &val3);
    
    TEST_ASSERT_PTR_EQ(vec_first(vec), &val1, "first should return first element");
    TEST_ASSERT_PTR_EQ(vec_last(vec), &val3, "last should return last element");
    
    vec_free(vec);
}

static void test_vec_resize(test_suite_t* suite)
{
    vector_t* vec = vec_new(2);
    int val1 = 1;
    int val2 = 2;
    
    vec_append(vec, &val1);
    vec_append(vec, &val2);
    
    TEST_ASSERT_EQ(vec_size(vec), 2, "size should be 2");
    
    vec_resize(vec, 5);
    TEST_ASSERT_EQ(vec_size(vec), 2, "size should still be 2 after resize");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 0), &val1, "first element should still be val1");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 1), &val2, "second element should still be val2");
    
    vec_resize(vec, 1);
    TEST_ASSERT_EQ(vec_size(vec), 1, "size should be 1 after shrinking");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 0), &val1, "first element should still be val1");
    
    vec_free(vec);
}

static void test_vec_shrink(test_suite_t* suite)
{
    vector_t* vec = vec_new(10);
    int val1 = 1;
    int val2 = 2;
    
    vec_append(vec, &val1);
    vec_append(vec, &val2);
    
    vec_shrink(vec);
    TEST_ASSERT_EQ(vec_size(vec), 2, "size should still be 2 after shrink");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 0), &val1, "first element should still be val1");
    TEST_ASSERT_PTR_EQ(vec_get(vec, 1), &val2, "second element should still be val2");
    
    vec_free(vec);
}

static void test_vec_clone(test_suite_t* suite)
{
    vector_t* vec = vec_new(5);
    int val1 = 1;
    int val2 = 2;
    int val3 = 3;
    
    vec_append(vec, &val1);
    vec_append(vec, &val2);
    vec_append(vec, &val3);
    
    vector_t* cloned = vec_clone(vec);
    TEST_ASSERT_NOT_NULL(cloned, "clone should return non-NULL");
    TEST_ASSERT_EQ(vec_size(cloned), 3, "cloned vector should have size 3");
    TEST_ASSERT_PTR_EQ(vec_get(cloned, 0), &val1, "cloned first element should match");
    TEST_ASSERT_PTR_EQ(vec_get(cloned, 1), &val2, "cloned second element should match");
    TEST_ASSERT_PTR_EQ(vec_get(cloned, 2), &val3, "cloned third element should match");
    
    vec_free(cloned);
    vec_free(vec);
}

static void test_vec_size_null(test_suite_t* suite)
{
    size_t size = vec_size(NULL);
    TEST_ASSERT_EQ(size, 0, "vec_size(NULL) should return 0");
}

int main(void)
{
    RUN_SUITE("vector",
        {"vec_new", test_vec_new},
        {"vec_append", test_vec_append},
        {"vec_get_set", test_vec_get_set},
        {"vec_first_last", test_vec_first_last},
        {"vec_resize", test_vec_resize},
        {"vec_shrink", test_vec_shrink},
        {"vec_clone", test_vec_clone},
        {"vec_size_null", test_vec_size_null}
    );
    
    printf("All vector tests passed!\n");
    return 0;
}

