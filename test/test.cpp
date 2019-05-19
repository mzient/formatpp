#include <gtest/gtest.h>
#include <formatpp/format.h>

using namespace formatpp;

static_assert(std::is_same<category<bool>, BooleanType>::value, "Invalid category");
static_assert(std::is_same<category<int>, IntegralType>::value, "Invalid category");
static_assert(std::is_same<category<double>, FloatingPointType>::value, "Invalid category");
static_assert(std::is_same<category<std::string>, StringType>::value, "Invalid category");
static_assert(std::is_same<category<char*>, StringType>::value, "Invalid category");
static_assert(std::is_same<category<const char*>, StringType>::value, "Invalid category");

TEST(FormatOptions, FP)
{
    {
        format_options<float> fp_opt("");
        EXPECT_EQ(fp_opt.width, -1);
        EXPECT_EQ(fp_opt.precision, -1);
    }
    {
        format_options<float> fp_opt("5");
        EXPECT_EQ(fp_opt.width, 5);
        EXPECT_EQ(fp_opt.precision, -1);
    }
    {
        format_options<double> fp_opt(".5");
        EXPECT_EQ(fp_opt.width, -1);
        EXPECT_EQ(fp_opt.precision, 5);
    }
    {
        format_options<float> fp_opt("+.5");
        EXPECT_EQ(fp_opt.width, -1);
        EXPECT_EQ(fp_opt.precision, 5);
        EXPECT_TRUE(fp_opt.leading_sign);
    }
    {
        format_options<float> fp_opt("+04.2");
        EXPECT_EQ(fp_opt.width, 4);
        EXPECT_EQ(fp_opt.precision, 2);
        EXPECT_EQ(fp_opt.leading_char, '0');
        EXPECT_TRUE(fp_opt.leading_sign);
    }
    {
        format_options<float> fp_opt("+04.2a");
        EXPECT_EQ(fp_opt.width, 4);
        EXPECT_EQ(fp_opt.precision, 2);
        EXPECT_EQ(fp_opt.leading_char, '0');
        EXPECT_EQ(fp_opt.radix, 16);
        EXPECT_TRUE(fp_opt.leading_sign);
    }
    EXPECT_THROW(format_options<float>("1nogood"), std::runtime_error);
}

TEST(FormatOptions, Integer)
{
    {
        format_options<int> opt("");
        EXPECT_EQ(opt.width, -1);
        EXPECT_EQ(opt.radix, 10);
        EXPECT_FALSE(opt.leading_sign);
        EXPECT_EQ(opt.leading_char, ' ');
    }
    {
        format_options<int> opt("5");
        EXPECT_EQ(opt.width, 5);
        EXPECT_FALSE(opt.leading_sign);
    }
    {
        format_options<int> opt("+5");
        EXPECT_EQ(opt.width, 5);
        EXPECT_TRUE(opt.leading_sign);
    }
    {
        format_options<int> opt("+05");
        EXPECT_EQ(opt.width, 5);
        EXPECT_TRUE(opt.leading_sign);
        EXPECT_EQ(opt.leading_char, '0');
    }
    {
        format_options<int> opt("+05x");
        EXPECT_EQ(opt.width, 5);
        EXPECT_TRUE(opt.leading_sign);
        EXPECT_EQ(opt.leading_char, '0');
        EXPECT_EQ(opt.radix, 16);
    }
    {
        format_options<int> opt("8b");
        EXPECT_EQ(opt.width, 8);
        EXPECT_EQ(opt.radix, 2);
    }
    {
        format_options<int> opt("o");
        EXPECT_EQ(opt.radix, 8);
    }
    EXPECT_THROW(format_options<int>("2.1e"), std::runtime_error);
    EXPECT_THROW(format_options<int>("2f"), std::runtime_error);
    EXPECT_THROW(format_options<int>("2e"), std::runtime_error);
    EXPECT_THROW(format_options<int>("1garbage"), std::runtime_error);
}

TEST(Formatter, Int)
{
    std::string str;
    string_output_context ctx(str);
    formatter<int8_t>().format(ctx, -5, "08b");
    EXPECT_EQ(str, "11111011");
    str = "";
    formatter<int>().format(ctx, -5, "04d");
    EXPECT_EQ(str, "-005");
    str = "";
    formatter<int>().format(ctx, 123, "+05d");
    EXPECT_EQ(str, "+0123");
    str = "";
    formatter<int>().format(ctx, 0x0badf00d, "x");
    EXPECT_EQ(str, "badf00d");
    str = "";
    formatter<int>().format(ctx, 0x0BADF00D, "X");
    EXPECT_EQ(str, "BADF00D");
    str = "";
    formatter<int>().format(ctx, 0x0BADF00D, "08X");
    EXPECT_EQ(str, "0BADF00D");
    str = "";
    formatter<int>().format(ctx, 1, "+3.3i");
    EXPECT_EQ(str, "+001");
    str = "";
    formatter<int>().format(ctx, 0, "+.0i");
    EXPECT_EQ(str, "+");
}

TEST(Formatter, Float)
{
    std::string str;
    string_output_context ctx(str);
    formatter<float>().format(ctx, -2.5, "");
    EXPECT_EQ(str, "-2.5");
    str = "";
    formatter<float>().format(ctx, 2.5, "+");
    EXPECT_EQ(str, "+2.5");
    str = "";
    formatter<float>().format(ctx, 2.51234, "+.4");
    EXPECT_EQ(str, "+2.512");
    str = "";
    formatter<float>().format(ctx, 2.51234, "+.4f");
    EXPECT_EQ(str, "+2.5123");
    str = "";
}

TEST(Formatter, Bool)
{
    std::string str;
    string_output_context ctx(str);
    formatter<bool>().format(ctx, true, "b");
    EXPECT_EQ(str, "true");
    str = "";
    formatter<bool>().format(ctx, false, "B");
    EXPECT_EQ(str, "False");
    str = "";
    formatter<bool>().format(ctx, false, "i");
    EXPECT_EQ(str, "0");
    str = "";
    formatter<bool>().format(ctx, true, "i");
    EXPECT_EQ(str, "1");
    str = "";
}

TEST(Formatter, String)
{
    std::string str;
    string_output_context ctx(str);
    formatter<std::string>().format(ctx, "ala ma kota", "20");
    EXPECT_EQ(str, "         ala ma kota");
    str = "";
    formatter<std::string>().format(ctx, "ala ma kota", "6");
    EXPECT_EQ(str, "ala ma kota");
    str = "";
    formatter<std::string>().format(ctx, "ala ma kota", ".6");
    EXPECT_EQ(str, "ala ma");
    str = "";
    formatter<std::string>().format(ctx, "ala ma kota", "10.6");
    EXPECT_EQ(str, "    ala ma");
    str = "";
}

TEST(FormatParams, VariableLength)
{
    int i = 2;
    float f = 5.5f;
    format_params<string_output_context, (size_t)-1> params = { 1, 1.5, i, f, std::string("xyz"), "abc" };
    EXPECT_EQ(1, params[0].value<int>());
    EXPECT_EQ(1.5, params[1].value<double>());
    EXPECT_EQ(2, params[2].value<int&>());
    EXPECT_EQ(5.5f, params[3].value<float&>());
    EXPECT_EQ("xyz", params[4].value<std::string>());
    EXPECT_STREQ("abc", params[5].value<const char(&)[4]>());
}

TEST(FormatParams, StaticLength)
{
    int i = 2;
    float f = 5.5f;
    auto params = make_format_params<string_output_context>(1, 1.5, i, f, std::string("xyz"), "abc");
    EXPECT_EQ(1, params[0].value<int>());
    EXPECT_EQ(1.5, params[1].value<double>());
    EXPECT_EQ(2, params[2].value<int&>());
    EXPECT_EQ(5.5f, params[3].value<float&>());
    EXPECT_EQ("xyz", params[4].value<std::string>());
    EXPECT_STREQ("abc", params[5].value<const char(&)[4]>());
}

TEST(Format, NonIndexed)
{
    EXPECT_EQ(format_str("ABC{}def{}GHI", 1.5f, "test"),
              "ABC1.5deftestGHI");

    std::stringstream ss;
    format_to(ss, "{:x} or not {}{} == 0x{:X}", 0x2B, 2, 'b', 255);
    EXPECT_EQ(ss.str(), "2b or not 2b == 0xFF");
}

TEST(Format, Indexed)
{
    EXPECT_EQ(format_str("ABC{2}def{1}GHI", 0xbad, "test", 1.5f),
              "ABC1.5deftestGHI");

    std::stringstream ss;
    format_to(ss, "{1}{2} or not {0:x} == {3:X}", 0x2B, 2, 'b', 255);
    EXPECT_EQ(ss.str(), "2b or not 2b == FF");
}

TEST(Format, Braces)
{
    EXPECT_EQ(format_str("a{{b}}c"), "a{b}c");;
    EXPECT_EQ(format_str("{{}}"), "{}");;
    EXPECT_EQ(format_str("{{{1}{0}}}", 21, 123), "{12321}");;
}

struct CustomType
{
    int a, b;
};

namespace formatpp {

template <>
struct formatter<CustomType>
{
    template <typename Context>
    static void format(Context &ctx, CustomType value, const format_options<CustomType> &opt)
    {
        format_to(ctx, "{0}, {1}", value.a, value.b);
    }
};
} // formatpp

TEST(Format, Custom)
{
    EXPECT_EQ(format_str("--{0}--", CustomType{4, 5}),
              "--4, 5--");

    print("{0:.5e}", 1500.f);
}
