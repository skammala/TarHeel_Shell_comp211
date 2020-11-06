#include "gtest/gtest.h"

extern "C" {
#include "stdint.h"
#include "Str.h"
}

/**
 * The purpose of these tests is to prove correctness of the Str
 * abstraction barrier from the user's point-of-view.
 */

TEST(StrSpec, values_init_empty) {
    Str s = Str_value(10);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);
}

TEST(StrSpec, values_init_cstr) {
    Str s = Str_value(10);
    ASSERT_STREQ("", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, append_chars) {
    Str s = Str_value(10);
    const char *string = "abc";
    Str_append(&s, string);
    ASSERT_EQ('a', Str_get(&s, 0));
    ASSERT_EQ('b', Str_get(&s, 1));
    ASSERT_EQ('c', Str_get(&s, 2));
    Str_drop(&s);
}

TEST(StrSpec, pointer_to_str) {
    Str s = Str_value(10);
    const char *string = "abcdef";
    Str_append(&s, string);
    ASSERT_STREQ(string, Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, str_insert_chars) {
    Str s = Str_value(5);
    Str_set(&s, 0, 'a');
    Str_set(&s, 1, 'b');
    Str_set(&s, 2, 'd');
    Str_set(&s, 2, 'c');
    ASSERT_EQ('a', Str_get(&s, 0));
    ASSERT_EQ('b', Str_get(&s, 1));
    ASSERT_EQ('c', Str_get(&s, 2));
    Str_drop(&s);
}

TEST(StrSpec, str_length) {
    Str s = Str_value(5);
    Str_set(&s, 0, 'a');
    Str_set(&s, 1, 'b');
    ASSERT_EQ(2, Str_length(&s));
    Str_drop(&s);
}

TEST(StrSpec, new_str_from_cstr) {
    const char *string = "hello";
    Str s = Str_from(string);
    ASSERT_STREQ(string, Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrSpec, splice_string) {
    Str s = Str_from("hello");
    Str_splice(&s, 0, 5, "abcd", 3);
    ASSERT_EQ('a', Str_get(&s, 0));
    ASSERT_EQ('b', Str_get(&s, 1));
    ASSERT_EQ('c', Str_get(&s, 2));
    Str_drop(&s);
}
