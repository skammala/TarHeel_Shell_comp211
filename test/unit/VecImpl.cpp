#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Vec.h"
}

/*
 * Unit tests established during the implementation of Vec.c
 * These tests are permitted to reach through the abstraction
 * and access members of a Vec struct directly.
 */

TEST(VecImpl, value) {
    Vec v = Vec_value(2, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(2, v.capacity);
    ASSERT_STREQ((char*)v.buffer, "");
    Vec_drop(&v);
}

TEST(VecImpl, drop) {
    Vec v = Vec_value(2, sizeof(int16_t));
    Vec_drop(&v);
    ASSERT_EQ(0, v.length);
    ASSERT_EQ(0, v.capacity);
    ASSERT_EQ(nullptr, v.buffer);
}

TEST(VecImpl, ref) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 1;
    buffer[1] = 2;
    v.length = 2;
    ASSERT_EQ(&buffer[0], Vec_ref(&v, 0));
    ASSERT_EQ(&buffer[1], Vec_ref(&v, 1));
    Vec_drop(&v);
}

TEST(VecImpl, ref_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    ASSERT_DEATH({
            Vec_ref(&v, 3);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, get) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 200;
    buffer[1] = 300;
    v.length = 2;
    int16_t int_val = 0;
    Vec_get(&v, 1, &int_val);
    ASSERT_EQ(300, int_val); 
    Vec_drop(&v);
}

TEST(VecImpl, get_out_of_bounds) { 
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    ASSERT_DEATH({
            Vec_ref(&v, 2);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, set) {
    Vec v = Vec_value(3, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    int16_t val = 400;
    Vec_set(&v, 2, &val);
    ASSERT_EQ(val, buffer[2]);
    ASSERT_EQ(3, v.length); 
    Vec_drop(&v);
}

TEST(VecImpl, set_exceeds_capacity) {
    Vec v = Vec_value(1, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    v.length = 1;
    int16_t val = 100;
    Vec_set(&v, 1, &val);
    ASSERT_EQ(val, buffer[1]);
    ASSERT_EQ(v.length, 2);
    ASSERT_EQ(v.capacity, 4);
    Vec_drop(&v);
}

TEST(VecImpl, set_out_of_bounds) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    int16_t val = 400;
    ASSERT_DEATH({
            Vec_set(&v, 3, &val);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, equals) {
    Vec v = Vec_value(4, sizeof(int16_t));
    Vec v2 = Vec_value(2, sizeof(int16_t));
    Vec v3 = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    int16_t *buffer2 = (int16_t*) v2.buffer;
    int16_t *buffer3 = (int16_t*) v3.buffer;
    buffer[0] = 50;
    buffer2[0] = 50;
    buffer3[0] = 1000;
    buffer3[1] = 1000;
    v.length = 1;
    v2.length = 1;
    v3.length = 2;
    ASSERT_EQ(true, Vec_equals(&v, &v2));
    ASSERT_EQ(false, Vec_equals(&v, &v3));
    Vec_drop(&v);
    Vec_drop(&v2);
    Vec_drop(&v3);
}

TEST(VecImpl, splice_delete) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    v.length = 3;
    int16_t items[] = {0, 1, 2};
    Vec_splice(&v, 1, 1, items, 0);
    buffer = (int16_t*) v.buffer;
    ASSERT_EQ(2, v.length);
    ASSERT_EQ(100, buffer[0]);
    ASSERT_EQ(300, buffer[1]);
    Vec_drop(&v);
}
/*
TEST(VecImpl, splice_delete_single_item) {
   Vec v = Vec_value(1, sizeof(int16_t));
  int16_t *buffer = (int16_t*) v.buffer; 
  buffer[0] = 100;
  v.length = 1;
  int16_t items[] = {0, 1, 2};
  Vec_splice(&v, 0, 1, items, 0);
  ASSERT_EQ(0, v.length);
  Vec_drop(&v);
}
*/
TEST(VecImpl, splice_delete_empty_vec) {
    Vec v = Vec_value(0, sizeof(int16_t));
    ASSERT_EQ(0, v.length);
    int16_t items[] = {1, 2, 3};
    ASSERT_DEATH({
            Vec_splice(&v, 0, 1, items, 0);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, splice_delete_exceeds_bounds) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 200;
    v.length = 2;
    int16_t items[] = {1, 2, 3}; 
    ASSERT_DEATH({
            Vec_splice(&v, 0, 3, items, 1);
            }, ".* - Out of Bounds");
    Vec_drop(&v);
}

TEST(VecImpl, splice_prepend) {
    Vec v = Vec_value(2, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 100;
    buffer[1] = 400;
    int16_t items[] = {200, 300, 400, 500};
    v.length = 2;
    Vec_splice(&v, 0, 0, items, 2);
    ASSERT_EQ(4, v.length);
    ASSERT_EQ(200, buffer[0]);
    ASSERT_EQ(300, buffer[1]);
    ASSERT_EQ(100, buffer[2]);
    ASSERT_EQ(400, buffer[3]);
    Vec_drop(&v);
}
/*
TEST(VecImpl, splice_insert_empty_vec) {
    Vec v = Vec_value(0, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    ASSERT_EQ(0, v.length);
    int16_t items[] = {100, 200};
    Vec_splice(&v, 0, 0, items, 2);
    buffer = (int16_t*) v.buffer;
    ASSERT_EQ(100, buffer[0]);
    ASSERT_EQ(200, buffer[1]);
    Vec_drop(&v);
}
*/
TEST(VecImpl, splice_replace_entire_vec) {
    Vec v = Vec_value(4, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    int16_t items[] = {10, 20, 30, 40};
    buffer[0] = 100;
    buffer[1] = 200;
    buffer[2] = 300;
    buffer[3] = 400;
    v.length = 4;
    Vec_splice(&v, 0, 4, items, 4);
    ASSERT_EQ(4, v.length);
    ASSERT_EQ(10, buffer[0]);
    ASSERT_EQ(20, buffer[1]);
    ASSERT_EQ(30, buffer[2]);
    ASSERT_EQ(40, buffer[3]);
    Vec_drop(&v);
}

TEST(VecImpl, splice_replace_several_inserts) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 5;
    buffer[1] = 10;
    buffer[2] = 15;
    v.length = 3;
    int16_t items[] = {7, 8, 9, 10, 11, 12, 13, 14};
    ASSERT_EQ(3, v.length);
    Vec_splice(&v, 1, 1, items, 8);
    buffer = (int16_t*) v.buffer;
    ASSERT_EQ(10, v.length);
    ASSERT_EQ(5, buffer[0]);
    ASSERT_EQ(7, buffer[1]);
    ASSERT_EQ(8, buffer[2]);
    ASSERT_EQ(9, buffer[3]);
    ASSERT_EQ(10, buffer[4]);
    ASSERT_EQ(11, buffer[5]);
    ASSERT_EQ(15, buffer[9]);
    Vec_drop(&v);
}

TEST(VecImpl, splice_append) {
    Vec v = Vec_value(3, sizeof(int16_t));
    int16_t *buffer = (int16_t*) v.buffer;
    buffer[0] = 10;
    buffer[1] = 20;
    buffer[2] = 30;
    v.length = 3;
    int16_t items[] = {40, 50, 60, 70};
    Vec_splice(&v, 3, 0, items, 4);
    buffer = (int16_t*) v.buffer;
    ASSERT_EQ(7, v.length);
    ASSERT_EQ(10, buffer[0]);
    ASSERT_EQ(20, buffer[1]);
    ASSERT_EQ(30, buffer[2]);
    ASSERT_EQ(40, buffer[3]);
    ASSERT_EQ(50, buffer[4]);
    ASSERT_EQ(60, buffer[5]);
    ASSERT_EQ(70, buffer[6]);
    Vec_drop(&v);
}
