#include <formatpp/format.h>
#include <gtest/gtest.h>
#include <chrono>
#include <cmath>

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

TEST(Formatter, IntLimits)
{
    std::string str;
    string_output_context ctx(str);
    formatter<int8_t>().format(ctx, -128, "d");
    EXPECT_EQ(str, "-128");
    str = "";
    formatter<int32_t>().format(ctx, 0x7fffffff, "d");
    EXPECT_EQ(str, "2147483647");
    str = "";
    formatter<int32_t>().format(ctx, -0x7fffffff-1, "d");
    EXPECT_EQ(str, "-2147483648");
    str = "";
    formatter<uint32_t>().format(ctx, 0xffffffff, "d");
    EXPECT_EQ(str, "4294967295");
    str = "";
    formatter<int64_t>().format(ctx, std::numeric_limits<int64_t>::max(), "d");
    EXPECT_EQ(str, "9223372036854775807");
    str = "";
    formatter<int64_t>().format(ctx, std::numeric_limits<int64_t>::min(), "d");
    EXPECT_EQ(str, "-9223372036854775808");
}

TEST(FormatFloat, Exponent)
{
    std::string str;
    string_output_context ctx(str);
    formatter<float> fmt;
    EXPECT_EQ(fmt.exponent(0.09, 10), -2);
    EXPECT_EQ(fmt.exponent(0.1, 10), -1);
    EXPECT_EQ(fmt.exponent(1, 10), 0);
    EXPECT_EQ(fmt.exponent(10, 10), 1);
    EXPECT_EQ(fmt.exponent(10 - 1e-6f, 10), 0);
    EXPECT_EQ(fmt.exponent(99, 10), 1);
    EXPECT_EQ(fmt.exponent(100, 10), 2);
    EXPECT_EQ(fmt.exponent(999, 10), 2);
    EXPECT_EQ(fmt.exponent(1.1e+38, 10), 38);
    EXPECT_EQ(fmt.exponent(0.99999e+14, 10), 13);
    EXPECT_EQ(fmt.exponent(std::nextafter<float>(1e+14f, 0.0f), 10), 13);
    EXPECT_EQ(fmt.exponent(std::nextafter<float>(1e+14f, 1e+15f), 10), 14);
}

TEST(Formatter, Float)
{
    std::string str;
    string_output_context ctx(str);
    formatter<float>().format(ctx, -2.5, "");
    EXPECT_EQ(str, "-2.5");
    str = "";
    formatter<float>().format(ctx, 2.5f, "+");
    EXPECT_EQ(str, "+2.5");
    str = "";
    formatter<float>().format(ctx, 1.25f, "f");
    EXPECT_EQ(str, "1.250000");
    str = "";
    formatter<float>().format(ctx, 2.51234f, "+.4");
    EXPECT_EQ(str, "+2.512");
    str = "";
    formatter<float>().format(ctx, 2.51254f, "+.4");
    EXPECT_EQ(str, "+2.513");
    str = "";
    formatter<float>().format(ctx, 12.3456f, "+.2f");
    EXPECT_EQ(str, "+12.35");
    str = "";
    formatter<float>().format(ctx, 0.1f, {});
    EXPECT_EQ(str, "0.1");
    str = "";
    formatter<float>().format(ctx, 0.099999f, ".4g");
    EXPECT_EQ(str, "0.1");
    str = "";
    formatter<float>().format(ctx, 99.999f, ".4g");
    EXPECT_EQ(str, "100");
    str = "";
    formatter<float>().format(ctx, 0.9999e+10f, ".4g");
    EXPECT_EQ(str, "9.999e+9");
    str = "";
    formatter<float>().format(ctx, 0.99999e+10f, ".4g");
    EXPECT_EQ(str, "1e+10");
    str = "";
    formatter<float>().format(ctx, 0.99999e+10f, ".3E");
    EXPECT_EQ(str, "1.000E+10");
    str = "";
    formatter<float>().format(ctx, 0.999e+10f, ".3E");
    EXPECT_EQ(str, "9.990E+9");
    str = "";
    formatter<float>().format(ctx, -0.9999f, ".3E");
    EXPECT_EQ(str, "-1.000E+0");
    str = "";
    formatter<float>().format(ctx, 1.9999f, ".3E");
    EXPECT_EQ(str, "2.000E+0");
    str = "";
    formatter<float>().format(ctx, 1.0f/999, "");
    EXPECT_EQ(str, "0.001001");
    str = "";
    formatter<float>().format(ctx, 1.0f/999, "f");
    EXPECT_EQ(str, "0.001001");
    str = "";
    formatter<float>().format(ctx, 0, ".5f");
    EXPECT_EQ(str, "0.00000");
    str = "";
    formatter<float>().format(ctx, 0, ".5e");
    EXPECT_EQ(str, "0.00000e+0");
    str = "";
    formatter<double>().format(ctx, 1e+15-1, ".25f");
    EXPECT_EQ(str, "999999999999999.0000000000000000000000000");
    str = "";
    formatter<double>().format(ctx, 1-1e-15, ".25f");
    EXPECT_EQ(str.substr(0, 16), "0.99999999999999");
    str = "";
    formatter<double>().format(ctx, 1-1e-15, ".25g");
    EXPECT_EQ(str.substr(0, 16), "0.99999999999999");
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
    auto params = make_format_params<string_output_context>(1, 1.5, i, f, std::string("xyz"), "abc");
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

TEST(TempBuffer, Alloc)
{
    tmp_buf_allocator alloc;
    {
        // static
        auto m1 = alloc.allocate(alloc.static_buffer_size/2);
        // dynamic
        auto m2 = alloc.allocate(alloc.static_buffer_size*3/2);
        // static
        auto m3 = alloc.allocate(alloc.static_buffer_size/2);
        EXPECT_EQ(m3.data, m1.data + alloc.static_buffer_size/2);

        // destruction in stack order should guarantee proper deallocation
    }
    for (auto &a : alloc.allocs)
        EXPECT_EQ(a.used, 0);
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
}

using perf_clock = std::chrono::high_resolution_clock;

template <typename rep, typename period>
double ns(std::chrono::duration<rep, period> t)
{
    return std::chrono::duration_cast<std::chrono::duration<double, std::nano>>(t).count();
}

const char *strdata = "asdf";

TEST(Format, Perf)
{
    const int outer_N = 100;
    const int N = 1000;
    const int total_N = outer_N * N;
    perf_clock::time_point start, end;

    for (int i = 0; i < 100; i++)
    {
        char buf[64];
        sprintf(buf, "%i %g %s", i, 1.0f/i, "asdf");
        (void)std::string(buf);
    }
    for (int i = 0; i < 100; i++)
    {
        (void)format_str("{} {} {}", i, 1.0f/i, "asdf");
    }

    perf_clock::duration time_format(0), time_sprintf(0), time_stream(0);

    for (int i = 0; i < outer_N; i++)
    {
        start = perf_clock::now();
        for (int i = 0; i < N; i++)
        {
            (void)format_str("{} {}", i, strdata);
        }
        end = perf_clock::now();
        time_format += end - start;
        start = perf_clock::now();
        for (int i = 0; i < N; i++)
        {
            char buf[64];
            int n = snprintf(buf, 64, "%i %s", i, strdata);
            (void)std::string(buf, n);
        }
        end = perf_clock::now();
        time_sprintf += end - start;

        start = perf_clock::now();
        for (int i = 0; i < N; i++)
        {
            std::stringstream ss;
            ss << i << " " << 1.0/i << " " << strdata;
            (void)ss.str();
        }
        end = perf_clock::now();
        time_stream += end - start;
    }
    std::cout << "format_str (dynamic buffer): " << std::round(ns(time_format) / total_N) << "ns" << std::endl;
    std::cout << "snprintf (fixed buffer + string(buf)): " << std::round(ns(time_sprintf) / total_N) << "ns" << std::endl;
    std::cout << "stringstream: " << std::round(ns(time_stream) / total_N) << "ns" << std::endl;
}
