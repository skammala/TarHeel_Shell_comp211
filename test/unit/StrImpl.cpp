#include "gtest/gtest.h"

extern "C" {
#include "Str.h"
#include "string.h"
}

TEST(StrImpl, value) {
    Str s = Str_value(2);
    ASSERT_EQ(1, s.length); // Vec's length includes null char
    ASSERT_EQ(3, s.capacity); // Requested space + 1 for null char
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('\0', buffer[0]);
    Str_drop(&s);
}

TEST(StrImpl, drop) {
    Str s = Str_value(2);
    Str_drop(&s);
    ASSERT_EQ(0, s.capacity);
    ASSERT_EQ(0, s.length);
    ASSERT_EQ(nullptr, s.buffer);
}

/**
 * Helper fixture to setup a Str whose buffer
 * is prefilled with "abcd". 
 */
Str fixture_abcd()
{
    Str s = Str_value(4);
    char *buffer = (char*) s.buffer;
    buffer[0] = 'a';
    buffer[1] = 'b';
    buffer[2] = 'c';
    buffer[3] = 'd';
    buffer[4] = '\0';
    s.length = 5; // Includes null char
    return s;
}

TEST(StrImpl, length) {
    Str s = Str_value(2);
    ASSERT_EQ(0, Str_length(&s));
    Str_drop(&s);

    s = fixture_abcd();
    ASSERT_EQ(4, Str_length(&s));
    Str_drop(&s);
}

TEST(StrImpl, cstr) {
    Str s = fixture_abcd();
    ASSERT_STREQ("abcd", Str_cstr(&s));
    Str_drop(&s);
}

TEST(StrImpl, ref) {
    Str s = fixture_abcd();
    char *buffer = (char*) s.buffer;
    for (size_t i = 0; i < s.length; ++i) {
        ASSERT_EQ(&buffer[i], Str_ref(&s, i));
    }
    Str_drop(&s);
}

TEST(StrImpl, from) {
    Str from = Str_from("abc");
    ASSERT_EQ(3, from.length - 1);
    char *buffer = (char*) from.buffer;
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('b', buffer[1]);
    ASSERT_EQ('c', buffer[2]);
    ASSERT_EQ('\0', buffer[3]);
    Str_drop(&from);
}

TEST(StrImpl, splice_replace_partial) {
    Str s = fixture_abcd();
    char cstr[] = {'y', 'z'};
    Vec_splice(&s, 1, 3, cstr, 2);
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('y', buffer[1]);
    ASSERT_EQ('z', buffer[2]);
    Str_drop(&s);
}

TEST(StrImpl, splice_retains_null_char) {
    Str s = fixture_abcd();
    char cstr[] = {'y', 'z'};
    Vec_splice(&s, 1, 3, cstr, 2);
    char *buffer = (char*) s.buffer;
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('y', buffer[1]);
    ASSERT_EQ('z', buffer[2]);
    ASSERT_EQ('\0', buffer[3]);
    Str_drop(&s);
}

TEST(StrImpl, splice_append_string) {
    Str s = fixture_abcd();
    char cstr[] = {'e', 'f'};
    Vec_splice(&s, 4, 0, cstr, 2);
    char *buffer = (char*) s.buffer;
    ASSERT_EQ(6, s.length - 1);
    ASSERT_EQ('a', buffer[0]);
    ASSERT_EQ('b', buffer[1]);
    ASSERT_EQ('c', buffer[2]);
    ASSERT_EQ('d', buffer[3]);
    ASSERT_EQ('e', buffer[4]);
    ASSERT_EQ('f', buffer[5]);
    Str_drop(&s);
}
