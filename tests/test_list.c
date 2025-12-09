#include "tests.h"
#include "../list.h"
#include <stdlib.h>

static void test_list_new(test_suite_t* suite)
{
    list_t* list = list_new();
    TEST_ASSERT_NOT_NULL(list, "list_new should return non-NULL");
    TEST_ASSERT_EQ(list_size(list), 0, "new list should have size 0");
    TEST_ASSERT_NULL(list->head, "new list head should be NULL");
    TEST_ASSERT_NULL(list->tail, "new list tail should be NULL");
    list_free(list);
}

static void test_list_append(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    list_append(list, &val1);
    TEST_ASSERT_EQ(list_size(list), 1, "size should be 1 after first append");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val1, "first element should be val1");
    TEST_ASSERT_PTR_EQ(list_first(list), &val1, "first() should return val1");
    TEST_ASSERT_PTR_EQ(list_last(list), &val1, "last() should return val1");
    
    list_append(list, &val2);
    TEST_ASSERT_EQ(list_size(list), 2, "size should be 2 after second append");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val2, "second element should be val2");
    TEST_ASSERT_PTR_EQ(list_first(list), &val1, "first() should still return val1");
    TEST_ASSERT_PTR_EQ(list_last(list), &val2, "last() should return val2");
    
    list_append(list, &val3);
    TEST_ASSERT_EQ(list_size(list), 3, "size should be 3 after third append");
    TEST_ASSERT_PTR_EQ(list_get(list, 2), &val3, "third element should be val3");
    
    list_free(list);
}

static void test_list_prepend(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    list_prepend(list, &val1);
    TEST_ASSERT_EQ(list_size(list), 1, "size should be 1 after first prepend");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val1, "first element should be val1");
    
    list_prepend(list, &val2);
    TEST_ASSERT_EQ(list_size(list), 2, "size should be 2 after second prepend");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val2, "first element should be val2");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val1, "second element should be val1");
    
    list_prepend(list, &val3);
    TEST_ASSERT_EQ(list_size(list), 3, "size should be 3 after third prepend");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val3, "first element should be val3");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val2, "second element should be val2");
    TEST_ASSERT_PTR_EQ(list_get(list, 2), &val1, "third element should be val1");
    
    list_free(list);
}

static void test_list_insert(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    int val4 = 40;
    
    list_append(list, &val1);
    list_append(list, &val2);
    
    list_insert(list, 1, &val3);
    TEST_ASSERT_EQ(list_size(list), 3, "size should be 3 after insert");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val1, "first element should be val1");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val3, "second element should be val3");
    TEST_ASSERT_PTR_EQ(list_get(list, 2), &val2, "third element should be val2");
    
    list_insert(list, 0, &val4);
    TEST_ASSERT_EQ(list_size(list), 4, "size should be 4 after prepend insert");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val4, "first element should be val4");
    
    list_insert(list, 100, &val1);
    TEST_ASSERT_EQ(list_size(list), 5, "size should be 5 after out-of-bounds insert");
    TEST_ASSERT_PTR_EQ(list_last(list), &val1, "last element should be val1");
    
    list_free(list);
}

static void test_list_remove(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    list_append(list, &val1);
    list_append(list, &val2);
    list_append(list, &val3);
    
    list_remove(list, 1);
    TEST_ASSERT_EQ(list_size(list), 2, "size should be 2 after remove");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val1, "first element should be val1");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val3, "second element should be val3");
    
    list_remove(list, 0);
    TEST_ASSERT_EQ(list_size(list), 1, "size should be 1 after remove");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val3, "first element should be val3");
    
    list_remove(list, 0);
    TEST_ASSERT_EQ(list_size(list), 0, "size should be 0 after remove");
    
    list_remove(list, 0);
    TEST_ASSERT_EQ(list_size(list), 0, "size should still be 0 after invalid remove");
    
    list_free(list);
}

static void test_list_set_get(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    list_append(list, &val1);
    list_append(list, &val2);
    
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val1, "get(0) should return val1");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val2, "get(1) should return val2");
    TEST_ASSERT_NULL(list_get(list, 100), "get(100) should return NULL");
    
    list_set(list, 0, &val3);
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val3, "get(0) should return val3 after set");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val2, "get(1) should still return val2");
    
    list_set(list, 100, &val3);
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val2, "set(100) should not affect existing elements");
    
    list_free(list);
}

static void test_list_reverse(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    list_append(list, &val1);
    list_append(list, &val2);
    list_append(list, &val3);
    
    list_reverse(list);
    TEST_ASSERT_EQ(list_size(list), 3, "size should still be 3 after reverse");
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val3, "first element should be val3");
    TEST_ASSERT_PTR_EQ(list_get(list, 1), &val2, "second element should be val2");
    TEST_ASSERT_PTR_EQ(list_get(list, 2), &val1, "third element should be val1");
    
    list_reverse(list);
    TEST_ASSERT_PTR_EQ(list_get(list, 0), &val1, "first element should be val1 after second reverse");
    TEST_ASSERT_PTR_EQ(list_get(list, 2), &val3, "third element should be val3 after second reverse");
    
    list_free(list);
}

static void test_list_clear(test_suite_t* suite)
{
    list_t* list = list_new();
    int val1 = 10;
    int val2 = 20;
    int val3 = 30;
    
    list_append(list, &val1);
    list_append(list, &val2);
    list_append(list, &val3);
    
    TEST_ASSERT_EQ(list_size(list), 3, "size should be 3");
    
    list_clear(list);
    TEST_ASSERT_EQ(list_size(list), 0, "size should be 0 after clear");
    TEST_ASSERT_NULL(list->head, "head should be NULL after clear");
    TEST_ASSERT_NULL(list->tail, "tail should be NULL after clear");
    
    list_free(list);
}

int main(void)
{
    RUN_SUITE("list",
        {"list_new", test_list_new},
        {"list_append", test_list_append},
        {"list_prepend", test_list_prepend},
        {"list_insert", test_list_insert},
        {"list_remove", test_list_remove},
        {"list_set_get", test_list_set_get},
        {"list_reverse", test_list_reverse},
        {"list_clear", test_list_clear}
    );
    
    printf("All list tests passed!\n");
    return 0;
}

