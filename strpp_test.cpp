#include "strpp.h"

#include <gtest/gtest-param-test.h>
#include <gtest/gtest.h>
#include <tuple>

using TD = std::tuple<int, std::string, std::string>;

struct StrppTest : public testing::TestWithParam<TD>
{};

TEST_P(StrppTest, Increment)
{
    auto const& [lineno, input, expected] = GetParam();
    (void)lineno; // is there for the diagnostics

    std::string const result = ++input;
    EXPECT_EQ(expected, result);
}

/*
  Auto-increment and Auto-decrement
    "++" and "--" work as in C. That is, if placed before a variable, they
    increment or decrement the variable by one before returning the value,
    and if placed after, increment or decrement after returning the value.

        $i = 0;  $j = 0;
        print $i++;  # prints 0
        print ++$j;  # prints 1

    Note that just as in C, Perl doesn't define when the variable is
    incremented or decremented. You just know it will be done sometime
    before or after the value is returned. This also means that modifying a
    variable twice in the same statement will lead to undefined behavior.
    Avoid statements like:

        $i = $i ++;
        print ++ $i + $i ++;

    Perl will not guarantee what the result of the above statements is.

    The auto-increment operator has a little extra builtin magic to it. If
    you increment a variable that is numeric, or that has ever been used in
    a numeric context, you get a normal increment. If, however, the variable
    has been used in only string contexts since it was set, and has a value
    that is not the empty string and matches the pattern
    "/^[a-zA-Z]*[0-9]*\z/", the increment is done as a string, preserving
    each character within its range, with carry:

        print ++($foo = "99");      # prints "100"
        print ++($foo = "a0");      # prints "a1"
        print ++($foo = "Az");      # prints "Ba"
        print ++($foo = "zz");      # prints "aaa"

    "undef" is always treated as numeric, and in particular is changed to 0
    before incrementing (so that a post-increment of an undef value will
    return 0 rather than "undef").

    The auto-decrement operator is not magical.
*/

TD const test_data[] = {
    { __LINE__, "0", "1" },
    { __LINE__, "1", "2" },
    { __LINE__, "9", "10" },
    { __LINE__, "99", "100" },
    { __LINE__, "42", "43" },
    { __LINE__, "a", "b" },
    { __LINE__, "a0", "a1" },
    { __LINE__, "a9", "b0" },
    { __LINE__, "z9", "aa0" },
    { __LINE__, "Az", "Ba" },
    { __LINE__, "zz", "aaa" },
};

INSTANTIATE_TEST_CASE_P(Pfx, StrppTest, testing::ValuesIn(test_data));

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
