#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/**
 * The purpose of these tests is to prove correctness of the
 * abstraction barrier from the user's point-of-view. You should
 * not access any member of the Vec directly.
 */

TEST(VecSpec, values_init_empty) {
    Vec v = Vec_value(10, sizeof(double));
    ASSERT_EQ(0, Vec_length(&v));
    Vec_drop(&v);
}

TEST(VecSpec, values_add_item) {
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t value = 100;
    Vec_set(&v, 0, &value);
    ASSERT_EQ(1, Vec_length(&v));
    value = 0;
    Vec_get(&v, 0, &value);
    ASSERT_EQ(100, value);
    Vec_drop(&v);
}

TEST(VecSpec, check_pointer) {
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t value = 100;
    Vec_set(&v, 0, &value);
    ASSERT_EQ(value, *(int16_t*) Vec_ref(&v, 0));
    Vec_drop(&v);
}

TEST(VecSpec, equals_test) {
    Vec vec1 = Vec_value(10, sizeof(int16_t));
    Vec vec2 = Vec_value(10, sizeof(int16_t));
    Vec vec3 = Vec_value(10, sizeof(int16_t));
    int16_t value = 100;
    Vec_set(&vec1, 0, &value);
    Vec_set(&vec2, 0, &value);
    ASSERT_TRUE(Vec_equals(&vec1, &vec2));
    ASSERT_FALSE(Vec_equals(&vec1, &vec3));
    Vec_drop(&vec1);
    Vec_drop(&vec2);
    Vec_drop(&vec3);
}

TEST(VecSpec, splice_test) {
    Vec v = Vec_value(10, sizeof(int16_t));
    int16_t value = 100;
    int16_t value2 = 200;
    Vec_set(&v, 0, &value);
    Vec_set(&v, 1, &value2);
    ASSERT_EQ(2, Vec_length(&v));
    int16_t items[] = {1, 2, 3, 4};
    Vec_splice(&v, 1, 1, items, 2);
    int16_t get_value;
    Vec_get(&v, 0, &get_value);
    ASSERT_EQ(100, get_value);
    Vec_get(&v, 1, &get_value);
    ASSERT_EQ(1, get_value);
    Vec_get(&v, 2, &get_value);
    ASSERT_EQ(2, get_value);
    Vec_drop(&v);
}
