#ifndef FORMATPP_CORE_H_
#define FORMATPP_CORE_H_

#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <cassert>
#include <climits>
#include <cmath>
#include <tuple>
#include <type_traits>

namespace formatpp {
namespace detail {

template <typename T>
T max(const T &a, const T &b)
{
    return a < b ? b : a;
}

template <typename T>
T min(const T &a, const T &b)
{
    return b < a ? b : a;
}

namespace {

inline double ilog2(uint8_t radix) {
    static constexpr double tab[] = { 0, 0,
        1.0,
        0.6309297535714575,
        0.5,
        0.43067655807339306,
        0.38685280723454163,
        0.3562071871080222,
        0.3333333333333333,
        0.31546487678572877,
        0.3010299956639812,
        0.2890648263178879,
        0.27894294565112987,
        0.27023815442731974,
        0.26264953503719357,
        0.2559580248098155,
        0.25,
        0.24465054211822604,
        0.23981246656813146,
        0.23540891336663824,
        0.23137821315975915,
        0.227670248696953,
        0.22424382421757544,
        0.22106472945750374,
        0.21810429198553155,
        0.21533827903669653,
        0.21274605355336318,
        0.2103099178571525,
        0.20801459767650948,
        0.20584683246043448,
        0.2037950470905062,
        0.20184908658209985,
        0.2,
        0.19823986317056053,
        0.1965616322328226,
        0.1949590218937863,
        0.19342640361727081,
        0.19195872000656014,
        0.1905514124267734,
        0.18920035951687003,
        0.18790182470910757,
        0.18665241123894338,
        0.1854490234153689,
        0.18428883314870617,
        0.18316925091363362,
        0.18208790046993825,
        0.1810425967800402,
        0.18003132665669264,
        0.17905223175104137,
        0.1781035935540111,
        0.17718382013555792,
        0.17629143438888212,
        0.17542506358195453,
        0.17458343004804494,
        0.17376534287144002,
        0.1729696904450771,
        0.17219543379409813,
        0.17144160057391347,
        0.17070727966372012,
        0.16999161628691403,
        0.16929380759878143,
        0.1686130986895011,
        0.16794877895704194,
        0.16730017881017412,
        0.16666666666666666,
        0.16604764621593782,
        0.16544255391905832,
        0.16485085672216038,
        0.16427204996205025,
        0.16370565544521565,
        0.1631512196835108,
        0.16260831227163416,
        0.16207652439312228,
        0.16155546744299637,
        0.16104477175644444,
        0.1605440854340214,
        0.16005307325482135,
        0.15957141566993818,
        0.15909880786929415,
        0.15863495891559604,
        0.15817959093978226,
        0.15773243839286438,
        0.1572932473495469,
        0.156861774859441,
        0.15643778834207153,
        0.15602106502222501,
        0.15561139140249397,
        0.15520856277015513,
        0.1548123827357682,
        0.15442266280111014,
        0.15403922195426356,
        0.15366188628986424,
        0.15329048865267808,
        0.15292486830283214,
        0.15256487060115928,
        0.15221034671324338,
        0.15186115333086322,
        0.1515171524096389,
        0.15117821092177644,
        0.15084420062289414,
        0.1505149978319906,
        0.15019048322368797,
        0.14987054163194743,
        0.14955506186451523,
        0.14924393652741208,
        0.14893706185882832,
        0.148634337571835,
        0.14833566670536175,
        0.14804095548293264,
        0.1477501131786861,
        0.14746305199023912,
        0.14717968691798522,
        0.1468999356504447,
        0.14662371845531105,
        0.146350958075862,
        0.14608157963242446,
        0.14581551052860536,
        0.14555268036201674,
        0.14529302083924284,
        0.14503646569481302,
        0.14478295061395813,
        0.14453241315894394,
        0.1442847926987864,
        0.1440400303421672,
        0.14379806887337757,
        0.14355885269113103,
        0.14332232775009315,
        0.14308844150498737,
        0.14285714285714285,
        0.14262838210336,
        0.14240211088697471,
        0.14217828215101067,
        0.14195685009331532,
        0.1417377701235801,
        0.14152099882215266,
        0.14130649390055278,
        0.14109421416360957,
        0.1408841194731412,
        0.14067617071310393,
        0.14047032975614,
        0.14026655943145866,
        0.14006482349398794,
        0.13986508659473787,
        0.1396673142523192,
        0.13947147282556493,
        0.13927752948720412,
        0.1390854521985406,
        0.13889520968509134,
        0.13870677141314172,
        0.13852010756717748,
        0.13833518902815395,
        0.1381519873525671,
        0.13797047475229052,
        0.13779062407514628,
        0.1376124087861776,
        0.13743580294959373,
        0.13726078121135893,
        0.13708731878239783,
        0.13691539142239212,
        0.1367449754241439,
        0.13657604759848213,
        0.13640858525969024,
        0.13624256621143366,
        0.13607796873316694,
        0.13591477156700138,
        0.13575295390501496,
        0.13559249537698637,
        0.13543337603853725,
        0.13527557635966633,
        0.13511907721365987,
        0.1349638598663645,
        0.13480990596580797,
        0.13465719753215485,
        0.13450571694798438,
        0.13435544694887797,
        0.13420637061430538,
        0.13405847135879795,
        0.13391173292339814,
        0.13376613936737558,
        0.13362167506019965,
        0.13347832467375914,
        0.13333607317482013,
        0.13319490581771357,
        0.13305480813724413,
        0.13291576594181265,
        0.13277776530674432,
        0.13264079256781566,
        0.13250483431497315,
        0.13236987738623676,
        0.1322359088617821,
        0.13210291605819502,
        0.1319708865228925,
        0.13183980802870454,
        0.13170966856861138,
        0.13158045635063056,
        0.13145215979284927,
        0.13132476751859679,
        0.13119826835175236,
        0.13107265131218435,
        0.13094790561131578,
        0.13082402064781276,
        0.13070098600339125,
        0.13057879143873863,
        0.1304574268895465,
        0.13033688246265052,
        0.13021714843227464,
        0.13009821523637602,
        0.12998007347308715,
        0.12986271389725298,
        0.12974612741705907,
        0.1296303050907487,
        0.1295152381234257,
        0.12940091786394073,
        0.12928733580185806,
        0.12917448356450073,
        0.12906235291407148,
        0.1289509357448472,
        0.1288402240804449,
        0.12873021007115668,
        0.12862088599135182,
        0.1285122442369443,
        0.1284042773229231,
        0.12829697788094419,
        0.1281903386569819,
        0.12808435250903813,
        0.12797901240490775,
        0.12787431141999844,
        0.12777024273520352,
        0.12766679963482608,
        0.1275639755045533,
        0.12746176382947913,
        0.12736015819217406,
        0.127259152270801,
        0.12715873983727546,
        0.12705891475546918,
        0.1269596709794558,
        0.12686100255179733,
        0.12676290360187092,
        0.1266653683442337,
        0.1265683910770258,
        0.12647196618040968,
        0.1263760881150453,
        0.12628075142059988,
        0.12618595071429148,
        0.12609168068946533,
        0.1259979361142023,
        0.1259047118299582,
        0.12581200275023383,
        0.12571980385927414,
        0.12562811021079626,
        0.1255369169267456,
        0.1254462191960791,
        0.12535601227357512,
        0.12526629147866908,
        0.12517705219431446,
    };
    return tab[radix];
}

constexpr int max_digits_31[256] = {
0, 0, 31, 19, 15, 13, 11, 11, 10, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

constexpr int max_digits_63[256] = {
0, 0, 63, 39, 31, 27, 24, 22, 21, 19, 18, 18, 17, 17, 16, 16, 15, 15, 15, 14, 14, 14, 14, 13, 13, 13, 13, 13, 13, 12, 12, 12,
12, 12, 12, 12, 12, 12, 12, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
};
} // {}

template <typename T>
int max_precision(uint8_t radix) noexcept;

template<>
inline int max_precision<double>(uint8_t radix) noexcept
{
    switch (radix) {
    case 2:
        return 53;
    case 4:
        return 26;
    case 8:
        return 17;
    case 10:
        return 16;
    case 16:
        return 13;
    default:
        return 53 * ilog2(radix);
    }
}
template<>
inline int max_precision<float>(uint8_t radix) noexcept
{
    switch (radix) {
    case 2:
        return 23;
    case 4:
        return 11;
    case 8:
        return 7;
    case 10:
        return 7;
    case 16:
        return 6;
    default:
        return 23 * ilog2(radix);
    }
}

} // detail


template <typename Char>
class char_buf
{
public:
    using char_t = Char;

    char_buf() = default;
    char_buf(char_t *buffer, size_t capacity) : buf(buffer), cap(capacity)
    {
    }

    void append(const char_t *str, size_t count)
    {
        if (len + count >= cap)
            throw std::out_of_range("char_buf capacity exceeded");
        for (size_t i = 0; i < count; i++)
            buf[len + i] = str[i];
        len += count;
        buf[len] = 0; // null-terminate
    }

    void append(const char_t *str)
    {
        while (char_t c = *str++)
        {
            if (len + 1 >= cap)
                throw std::out_of_range("char_buf capacity exceeded");
            buf[len++] = c;
        }
        buf[len] = 0;
    }

    void append(size_t count, char_t value)
    {
        if (len + count >= cap)
            throw std::out_of_range("char_buf capacity exceeded");
        for (size_t i = 0; i < count; i++)
            buf[len + i] = value;
        len += count;

        buf[len] = 0;
    }

    using iterator = char_t*;
    using const_iterator = const char_t*;
    iterator begin() { return data(); }
    const_iterator cbegin() const { return data(); }
    const_iterator begin() const { return data(); }
    iterator end() { return data() + length(); }
    const_iterator cend() const { return data() + length(); }
    const_iterator end() const { return data() + length(); }

    char_t *data() noexcept { return buf; }
    const char_t *data() const noexcept { return buf; }
    const char_t *c_str() const noexcept { return buf; }
    size_t length() const noexcept { return len; }
    size_t size() const noexcept { return len; }
    size_t capacity() const noexcept { return cap; }

private:
    char_t *buf = nullptr;
    size_t len = 0;
    size_t cap = 0;
};

using std::size_t;
using std::ptrdiff_t;

enum class fp_format_mode
{
    /// Use either positional or exponential best fit
    automatic,
    /// Always use positional notation, even for very large or small numbers
    positional,
    /// Always use scientific (exponential) notation
    scientific,
    /// Use binary representation
    binary_repr
};

/// @brief Tells if `c` is ASCII digit.
/// @remarks `isdigit` can return true for digits in other encodings,
///           which we don't support in format strings
inline bool is_ascii_digit(char c)
{
    return c >= '0' && c <= '9';
}

template <bool condition, typename T = void>
using enable_if_t = typename std::enable_if<condition, T>::type;

class IntegralType;
class FloatingPointType;
class PointerType;
class StringType;
class CharType;
class BooleanType;

template <typename T>
class formatter;

template <typename T>
enable_if_t<std::is_integral<T>::value, IntegralType> TypeCategory(const T &);

template <typename T>
enable_if_t<std::is_floating_point<T>::value, FloatingPointType> TypeCategory(const T &);

template <typename Char>
StringType TypeCategory(const std::basic_string<Char> &);

StringType TypeCategory(const char *);
StringType TypeCategory(char *);
CharType TypeCategory(char);
BooleanType TypeCategory(bool);

template <typename T>
PointerType TypeCategory(T *);

template <typename Char>
size_t string_length(const std::basic_string<Char> &s) { return s.length(); }

inline size_t string_length(const char *s) { return std::strlen(s); }

template <typename Char>
inline size_t string_length(const char_buf<Char> &s) { return s.length(); }

void TypeCategory(...);

template <typename T>
struct type_category
{
    using type = decltype(TypeCategory(std::declval<T>()));
};

template <typename T>
using category = typename type_category<T>::type;

template <typename T>
using is_string_type = std::is_same<category<T>, StringType>;

struct integer_format_options
{
    void parse(const char *options, size_t &i)
    {
        char c;
        switch (c = options[i])
        {
        case '+':
        case ' ':
            leading_sign = c;
            i++;
            break;
        }

        if (options[i] == '0')
        {
            leading_char = '0';
            i++;
        }

        while (is_ascii_digit(c = options[i]))
        {
            if (width < 0)
                width = c - '0';
            else
                width = 10*width + (c - '0');
            i++;
        }

        if (c == '.')
        {
            i++;
            while (is_ascii_digit(c = options[i]))
            {
                if (precision < 0)
                    precision = c - '0';
                else
                    precision = 10*precision + (c - '0');
                i++;
            }
        }

        switch (options[i])
        {
        case 'd':
        case 'D':
        case 'i':
        case 'I':
            radix = 10;
            is_signed = true;
            break;
        case 'u':
        case 'U':
            radix = 10;
            is_signed = false;
            break;
        case 'b':
        case 'B':
            is_signed = false;
            radix = 2;
            break;
        case 'o':
        case 'O':
            is_signed = false;
            radix = 8;
            break;
        case 'x':
            is_signed = false;
            radix = 16;
            digits = lowercase_digits();
            break;
        case 'X':
            is_signed = false;
            radix = 16;
            digits = uppercase_digits();
            break;
        case '\0':
            break;
        default:
            throw std::runtime_error(std::string("Invalid format specifier for an integer: ") + options);
        }
    }

    int width = -1;
    uint8_t radix = 10;
    int precision = -1;
    char leading_sign = 0;
    bool is_signed = true;
    char leading_char = ' ';
    static const char *lowercase_digits()
    {
        return "0123456789abcdefghijklmnopqrstuvwxyz";
    }
    static const char *uppercase_digits()
    {
        return "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    const char *digits = lowercase_digits();
};

struct fp_format_options
{
    void parse(const char *options, size_t &i)
    {
        char c;
        switch (c = options[i])
        {
        case '+':
        case ' ':
            leading_sign = c;
            i++;
            break;
        }

        if (options[i] == '0')
        {
            leading_char = '0';
            i++;
        }

        while (is_ascii_digit(c = options[i]))
        {
            if (width < 0)
                width = c - '0';
            else
                width = 10*width + (c - '0');
            i++;
        }

        if (options[i] == '.')
        {
            i++;
            precision = 0;
            while (is_ascii_digit(c = options[i]))
            {
                precision = 10*precision + (c - '0');
                i++;
            }
        }

        switch (options[i])
        {
        case 'd':
            radix = 10;
            i++;
            break;
        case 'b':
        case 'B':
            radix = 2;
            i++;
            break;
        case 'a':
            radix = 16;
            digits = lowercase_digits();
            i++;
            break;
        case 'A':
            radix = 16;
            digits = uppercase_digits();
            i++;
            break;
        case 'f':
        case 'F':
        case 'g':
        case 'G':
        case 'e':
        case 'E':
            break;
        case '\0':
            return;
        default:
            throw std::runtime_error(std::string("Invalid format specifier for a floating point number: ") + options);
        }

        switch (c = options[i])
        {
        case 'f':
        case 'F':
            fp_mode = fp_format_mode::positional;
            uppercase = std::isupper(c);
            break;
        case 'g':
        case 'G':
            fp_mode = fp_format_mode::automatic;
            uppercase = std::isupper(c);
            break;
        case 'e':
        case 'E':
            fp_mode = fp_format_mode::scientific;
            uppercase = std::isupper(c);
            break;
        case 'x':
        case 'X':
            uppercase = std::isupper(c);
            digits = uppercase ? uppercase_digits() : lowercase_digits();
            fp_mode = fp_format_mode::binary_repr;
            break;
        }
    }

    int width = -1;
    int precision = -1;
    uint8_t radix = 10;
    char leading_sign = 0;
    char leading_char = ' ';
    bool uppercase = false;
    bool binary_repr = false;
    fp_format_mode fp_mode = fp_format_mode::automatic;
    static const char *lowercase_digits()
    {
        return "0123456789abcdefghijklmnopqrstuvwxyz";
    }
    static const char *uppercase_digits()
    {
        return "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    const char *digits = lowercase_digits();
};

struct bool_format_options
{
    void parse(const char *options, size_t &i)
    {
        while (char c = is_ascii_digit(options[i]))
        {
            if (width < 0)
                width = c - '0';
            else
                width = 10*width + (c - '0');
            i++;
        }

        switch (options[i])
        {
        case 'b':
            values = lowercase();
            break;
        case 'B':
            values = uppercase();
            break;
        case 'i':
            values = numeric();
            break;
        }
    }
    static const char **lowercase()
    {
        static const char *values[] = { "false", "true" };
        return values;
    }
    static const char **uppercase()
    {
        static const char *values[] = { "False", "True" };
        return values;
    }
    static const char **numeric()
    {
        static const char *values[] = { "0", "1" };
        return values;
    }
    const char **values = lowercase();
    int width = -1;
};

struct default_options
{
    void parse(const char *options, size_t &i)
    {
        char c;
        while (is_ascii_digit(c = options[i]))
        {
            width = 10*width + (c - '0');
            i++;
        }
        if (c == '.')
        {
            i++;
            while (is_ascii_digit(c = options[i]))
            {
                if (precision < 0)
                    precision = c - '0';
                else
                    precision = 10*precision + (c - '0');
                i++;
            }
        }
    }
    int width = 0;
    int precision = -1;
};

template <typename T, typename Category = category<T>>
struct default_format_options : default_options
{};

template <typename T>
struct default_format_options<T, IntegralType> : integer_format_options
{};

template <typename T>
struct default_format_options<T, FloatingPointType> : fp_format_options
{};

template <typename T>
struct default_format_options<T, BooleanType> : bool_format_options
{};

template <typename T>
struct format_options : default_format_options<T>
{
    format_options() = default;
    format_options(const char *options, size_t &i) { this->parse(options, i); }
    format_options(const char *options) { size_t i = 0; this->parse(options, i); }
};


template <typename char_t>
inline const char_t *c_str(const std::basic_string<char_t> &s) { return s.c_str(); }
inline const char *c_str(const char *s) { return s; }

template <typename char_t>
inline const char_t *c_str(const char_buf<char_t> &buf)
{
    return buf.c_str();
}


template <typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value> put(std::ostream &s, const StringLike &value)
{
    s << value;
}

template <typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value> put(std::string &s, const StringLike &value)
{
    s.append(value);
}

template <typename char_t, typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value> put(char_buf<char_t> &s, const StringLike &value)
{
    s.append(c_str(value), string_length(value));
}

inline void put(std::ostream &s, char c)
{
    s.put(c);
}

template <typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value>
put(std::ostream &s, const StringLike &value, size_t max_len)
{
    s.write(c_str(value), detail::min(max_len, string_length(value)));
}

template <typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value>
put(std::string &s, const StringLike &value, size_t max_len)
{
    s.append(c_str(value), detail::min(max_len, string_length(value)));
}

template <typename char_t, typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value>
put(char_buf<char_t> &s, const StringLike &value, size_t max_len)
{
    s.append(c_str(value), detail::min(max_len, string_length(value)));
}

inline void put(std::ostream &s, size_t n, char value)
{
    const size_t max_blk = 256;
    char tmp[max_blk];
    for (size_t i = 0; i < n && i < 64; i++)
        tmp[i] = value;
    while (n > 0)
    {
        size_t blk = detail::min(n, max_blk);
        s.write(tmp, blk);
        n -= blk;
    }
}



inline void put(std::string &s, size_t n, char value)
{
    s.append(n, value);
}

template <typename char_t>
inline void put(char_buf<char_t> &buf, size_t n, char value)
{
    buf.append(n, value);
}

template <typename T>
struct bump_allocator
{
    bump_allocator() = default;
    bump_allocator(T *ptr, size_t count) : data(ptr), total(count) {}

    T *data = nullptr;
    size_t total = 0;
    size_t used = 0;

    T *allocate(size_t count)
    {
        if (used + count > total)
            return nullptr;
        T *ret = data + used;
        used += count;
        return ret;
    }

    bool free(T *ptr, size_t count)
    {
        if (ptr + count == data + used)
        {
            used -= count;
            return true;
        }
        else
            return false;
    }
};

struct tmp_buf_allocator
{
    static constexpr size_t static_buffer_size = 256;
    char static_buf[static_buffer_size];
    static constexpr size_t num_allocs = 24;
    bump_allocator<char> allocs[num_allocs];
    tmp_buf_allocator()
    {
        allocs[0] = { static_buf, sizeof(static_buf) };
    }
    ~tmp_buf_allocator()
    {
        for (size_t i = 1; i < num_allocs; i++)
            delete[] allocs[i].data;
    }

    struct buffer_lease
    {
        buffer_lease(tmp_buf_allocator *owner, char *data, size_t count)
        : owner(owner), data(data), count(count) {}
        buffer_lease(const buffer_lease &) = delete;
        buffer_lease(buffer_lease &&b)
        {
            owner = b.owner;
            data = b.data;
            count = b.count;
            b.owner = nullptr;
            b.data = nullptr;
            b.count = 0;
        }
        ~buffer_lease()
        {
            release();
        }

        void release()
        {
            if (data)
            {
                owner->free(data, count);
                data = nullptr;
                count = 0;
            }
        }

        buffer_lease &operator=(const buffer_lease &b) = delete;
        buffer_lease &operator=(buffer_lease &&b)
        {
            release();
            owner = b.owner;
            data = b.data;
            count = b.count;
            b.owner = nullptr;
            b.data = nullptr;
            b.count = 0;
            return *this;
        }

        tmp_buf_allocator *owner;
        char *data;
        size_t count;

        char *get() const noexcept { return data; }
        size_t size() const noexcept { return count; }
    };
    char *tail = static_buf;

    buffer_lease allocate(size_t count)
    {
        return { this, allocate_raw(count), count };
    }

    char *allocate_raw(size_t count)
    {
        size_t prev_size = 0;
        for (size_t i = 0; i < num_allocs; i++)
        {
            if (allocs[i].data)
            {
                if (char *mem = allocs[i].allocate(count))
                    return mem;
            }
            else
            {
                size_t capacity = prev_size << 1;
                while (count > capacity)
                    capacity <<= 1;
                allocs[i] = { new char[capacity], capacity };
                return allocs[i].allocate(count);
            }
            prev_size = allocs[i].total;
        }
        throw std::bad_alloc();
    }

    void free(char *mem, size_t count)
    {
        for (int i = num_allocs - 1; i >= 0; i--)
            if (allocs[i].free(mem, count))
                break;
    }
};

template <typename Output>
struct output_context
{
    output_context(Output output) : output(output) {}

    using buf_lease = tmp_buf_allocator::buffer_lease;
    tmp_buf_allocator alloc;

    buf_lease get_tmp_buffer(size_t count)
    {
        return alloc.allocate(count);
    }

    typename std::add_lvalue_reference<Output>::type out()
    {
        return output;
    }

    Output output;
};

using string_output_context = output_context<std::string &>;
using ostream_output_context = output_context<std::ostream &>;

template <typename T>
struct ios_formatter
{
    template <typename Context>
    void format(Context &ctx, const T &value, const format_options<T> &options)
    {
        std::ostringstream ss;
        if (options.width > 0)
            ss.width(options.width);
        ss << value;
        put(ctx.out(), ss.str());
    }
};

template <typename T, typename Category = category<T>>
struct default_formatter : ios_formatter<T> {};

template <typename T>
struct default_formatter<T, BooleanType>
{
    template <typename Context>
    static void format(Context &ctx, const T &value, const format_options<T> &options)
    {
        auto v = options.values[static_cast<bool>(value)];
        int l = strlen(v);
        if (options.width > l)
            put(ctx.out(), options.width - l, ' ');
        put(ctx.out(), v);
    }
};

template <typename T>
struct default_formatter<T, CharType>
{
    template <typename Context>
    static void format(Context &ctx, const T &value, const format_options<T> &options)
    {
        if (options.width > 1)
            put(ctx.out(), options.width - 1, ' ');
        put(ctx.out(), value);
    }
};

template <typename T>
struct default_formatter<T, IntegralType>
{
    template <typename Context>
    static void format(Context &ctx, const T &value, const format_options<T> &options)
    {
        format_fixed(ctx, value, options);
    }

    template <typename Context>
    static void format_fixed(Context &ctx, const T &value, const format_options<T> &options, int fixed_point = 0, bool trim_trailing_zeros = false)
    {
        int buf_size = sizeof(T)*8 + 3;
        if (options.width + 2 > buf_size)
            buf_size = options.width + 2;
        if (options.precision + 2 > buf_size)
            buf_size = options.precision + 2;
        auto buf_lease = ctx.get_tmp_buffer(buf_size);
        char *buf = buf_lease.get();
        char *rbuf = buf + buf_size - 1;
        *rbuf = 0;
        int n = 0;
        bool is_negative = false;
        typename std::make_unsigned<T>::type x;
        if (std::is_signed<T>::value && options.is_signed && value < 0)
        {
            is_negative = true;
            x = -value;
        }
        else
        {
            x = value;
        }

        switch (options.radix)
        {
        case 2:
            write_fixed<2>(rbuf, x, n, options.radix, options.digits, fixed_point, trim_trailing_zeros);
            break;
        case 8:
            write_fixed<8>(rbuf, x, n, options.radix, options.digits, fixed_point, trim_trailing_zeros);
            break;
        case 10:  // use compiler's optimization for division by 10
            write_fixed<10>(rbuf, x, n, options.radix, options.digits, fixed_point, trim_trailing_zeros);
            break;
        case 16:
            write_fixed<16>(rbuf, x, n, options.radix, options.digits, fixed_point, trim_trailing_zeros);
            break;
        default:
            write_fixed<0>(rbuf, x, n, options.radix, options.digits, fixed_point, trim_trailing_zeros);
            break;
        }

        while (n < options.precision)
            rbuf[-++n] = options.digits[0];

        if (options.precision != 0 && (rbuf[-n] == '.' || rbuf[-n] == 0))
            rbuf[-++n] = options.digits[0];
        int space_for_sign = is_negative || options.leading_sign;
        if (options.leading_char != ' ')
            while (n < options.width - space_for_sign)
                rbuf[-++n] = options.leading_char;
        if (is_negative)
            rbuf[-++n] = '-';
        else if (options.leading_sign)
            rbuf[-++n] = options.leading_sign;
        while (n < options.width)
            rbuf[-++n] = ' ';
        put(ctx.out(), rbuf-n);
    }

private:
    template <uint8_t static_radix>
    static void write_fixed(char *rbuf, typename std::make_unsigned<T>::type x, int &_n,
                            uint8_t dynamic_radix, const char *digits,
                            int fixed_point, bool trim_trailing_zeros)
    {
        int n = _n;
        const unsigned radix = static_radix ? static_radix : dynamic_radix;
        int trimmed = 0;
        while (n + trimmed < fixed_point)
        {
            unsigned digit = x % radix;
            if (digit || !trim_trailing_zeros)
            {
                rbuf[-++n] = digits[digit];
                trim_trailing_zeros = false;
            }
            else
                trimmed++;
            x /= radix;
        }
        if (fixed_point > 0 && !trim_trailing_zeros)
            rbuf[-++n] = '.';
        while (x)
        {
            unsigned digit = x % radix;
            x /= radix;
            rbuf[-++n] = digits[digit];
        }
        _n = n;
    }
};

template <typename T>
struct default_formatter<T, FloatingPointType>
{
    static T lo(T value, T eps) {
        return std::floor(value / eps) * eps;
    }
    static T hi(T value, T eps) {
        return std::ceil(value / eps) * eps;
    }

    template <typename Context>
    static void format(Context &ctx, const T &value, const format_options<T> &options)
    {
        switch (options.fp_mode)
        {
        case fp_format_mode::automatic:
            automatic(ctx, value, options);
            break;
        case fp_format_mode::positional:
            positional(ctx, value, options);
            break;
        case fp_format_mode::scientific:
            scientific(ctx, value, options);
            break;
        case fp_format_mode::binary_repr:
            binary_repr(ctx, value, options);
            break;
        }
    }

    template <typename Context>
    static bool format_special(Context &ctx, const T &value, const format_options<T> &options)
    {
        if (std::isinf(value))
        {
            int l = value < 0 || options.leading_sign ? 4 : 3;
            const char *symbol = value < 0 ? "-inf" : options.leading_sign ? "+inf" : "inf";
            if (options.width > l)
                put(ctx.out(), ' ', options.width - l);
            put(ctx.out(), symbol, l);
            return true;
        }
        else if (std::isnan(value))
        {
            if (options.width > 3)
                put(ctx.out(), ' ', options.width - 3);
            put(ctx.out(), "nan");
            return true;
        }

        return false;
    }

    static std::pair<T, int> round_value(T value, const format_options<T> &options, bool fractional_digits)
    {
        if (value < 0)
        {
            auto tmp = round_value(-value, options, fractional_digits);
            return { -tmp.first, tmp.second };
        }

        int e = exponent(value, options.radix);
        int precision = options.precision >= 0 ? options.precision : 6;
        T m = powi(options.radix, fractional_digits ? -precision : e+1-precision);
        T rem = std::fmod(value, m);
        if (rem > m*T(0.5))
        {
            value = std::nextafter(value + m -rem, value + m);
        }
        if (value >= powi(options.radix, e+1))
            e++;

        return { value, e };
    }

    template <typename Context>
    static void automatic(Context &ctx, T value, const format_options<T> &options)
    {
        if (format_special(ctx, value, options))
            return;

        int e;
        std::tie(value, e) = round_value(value, options, false);

        int width = options.width > 0 ? options.width : options.precision > 0 ? options.precision : 6;
        if (abs(e) > width)
            scientific(ctx, value, e, options, true);
        else
            positional(ctx, value, e, options, true);
    }
    template <typename Context>
    static void positional(Context &ctx, T value, const format_options<T> &options)
    {
        if (format_special(ctx, value, options))
            return;
        int e;
        std::tie(value, e) = round_value(value, options, true);
        positional(ctx, value, e, options, false);
    }
    template <typename Context>
    static void scientific(Context &ctx, T value, const format_options<T> &options)
    {
        if (format_special(ctx, value, options))
            return;
        int e;
        std::tie(value, e) = round_value(value, options, false);
        scientific(ctx, value, e, options, false);
    }

    template <typename U = T>
    static U powi(uint8_t base, int e)
    {
        if (e < 0)
            return 1.0 / powi<U>(base, -e);

        U result = 1;
        U tmp = base;
        while (e > 0)
        {
            if (e&1)
                result *= tmp;
            tmp *= tmp;
            e >>= 1;
        }
        return result;
    }

    template <typename IntegralRepr, typename Context>
    static void positional_impl(Context &ctx, T value, int exponent, const format_options<T> &options, int digits, bool is_auto)
    {
        int precision = options.precision >= 0 ? options.precision : 6;
        int shift = is_auto ? digits - exponent - 1 : precision;
        T vm = value * powi(options.radix, shift);
        IntegralRepr ival = std::trunc(vm);

        format_options<IntegralRepr> iopt;
        iopt.width = options.width;
        iopt.radix = options.radix;
        iopt.leading_sign = options.leading_sign;
        formatter<IntegralRepr>::format_fixed(ctx, ival, iopt, shift, is_auto);
    }

    template <typename Context>
    static void positional_impl_l(Context &ctx, long double value, int exponent, const format_options<T> &options, int digits, bool is_auto)
    {
        auto buf_lease = ctx.get_tmp_buffer(digits + 2);
        char *buf = buf_lease.get();
        int precision = options.precision ? options.precision : 6;
        long double epsilon = is_auto ? powi<long double>(options.radix, exponent-digits)
                                      : powi<long double>(options.radix, -precision);
        long double v = std::fabs(value) * powi<long double>(options.radix, -exponent);
        if (v < 1) v = 1;
        else if (v >= options.radix) v = std::nextafter((long double)options.radix, 0);

        int i = 0;
        int n = exponent > 0 ? exponent : 1;
        if (value < 0)
            buf[i++] = '-';
        else if (options.leading_sign)
            buf[i++] = '+';
        int e;
        for (e = exponent; e >= 0 && value; e--)
        {
            auto p = powi<long double>(options.radix, e);
            int d = (int)(value / p);
            value -= d * p;
            if (value < 0)
                value = 0;
            else if (value >= p)
                value = std::nextafter(p, 0);
            buf[i++] = options.digits[d];
        }
        for (; e >= 0; e--)
            buf[i++] = options.digits[0];
        if (!i || buf[i-1] == '-' || buf[i-1] == '+')
            buf[i++] = options.digits[0];

        if (is_auto)
        {
            if (digits > n)
            {
                epsilon = powi<long double>(options.radix, i-digits);
                bool first = true;
                while (value > epsilon && n < digits)
                {
                    if (first)
                    {
                        buf[i++] = '.';
                        first = false;
                    }
                    value *= options.radix;
                    int d = (int)value;
                    value -= d;
                    if (value < 0)
                    {
                        value = 0;
                        d++;
                    }
                    buf[i++] = options.digits[d];
                    epsilon *= options.radix;
                    n++;
                }
            }
        }
        else
        {
            if (precision > 0)
            {
                buf[i++] = '.';
                for (int j = 0; j < precision; j++)
                {
                    value *= options.radix;
                    int d = (int)value;
                    value -= d;
                    if (value < 0)
                    {
                        value = 0;
                        d++;
                    }
                    buf[i++] = options.digits[d];
                }
            }
        }
        buf[i++] = 0;
        if (i < options.width)
        {
            put(ctx.out(), options.width - i, ' ');
        }
        put(ctx.out(), buf, i);
    }
    template <typename Context>
    static void positional(Context &ctx, T value, int exponent, const format_options<T> &options, bool is_auto)
    {
        int precision = options.precision >= 0 ? options.precision : 6;

        int digits = is_auto ? precision : std::max(exponent, 0) + precision + 1;

        const int max_digits_31 = detail::max_digits_31[options.radix];
        const int max_digits_63 = detail::max_digits_63[options.radix];
        if (digits <= max_digits_31)
            positional_impl<int32_t>(ctx, value, exponent, options, digits, is_auto);
        else if (digits <= max_digits_63)
            positional_impl<int64_t>(ctx, value, exponent, options, digits, is_auto);
        else
            positional_impl_l(ctx, (long double)value, exponent, options, digits, is_auto);
    }

    template <typename Context, typename Exp = int>
    static void scientific(Context &ctx, T value, int exponent, const format_options<T> &options, bool is_auto)
    {
        if (options.width > 0)
        {
            auto tmp_opt = options;
            tmp_opt.width = 0;
            int precision = options.precision >= 0 ? options.precision : 6;
            const size_t tmp_n = precision + 8;
            auto buf_lease = ctx.get_tmp_buffer(tmp_n);
            char_buf<char> tmp(buf_lease.get(), tmp_n);
            output_context<char_buf<char>&> tmp_ctx(tmp);
            scientific(tmp_ctx, value, exponent, tmp_opt, is_auto);
            print(ctx, c_str(tmp), string_length(tmp), options.width);
        }
        else
        {
            if (value == 0) {
                positional(ctx, 0, 0, options, is_auto);
                put(ctx.out(), options.uppercase ? "E+0" : "e+0");
            } else {
                auto tmp = value*powi(options.radix,-exponent);
                if (std::abs(tmp) < 1)
                    tmp = std::copysign(1, value);
                positional(ctx, tmp, 0, options, is_auto);
                put(ctx.out(), options.uppercase ? "E" : "e");
                format_options<Exp> opt;
                opt.leading_sign = '+';
                formatter<Exp>().format(ctx, exponent, opt);
            }
        }
    }
    template <typename Context>
    static void binary_repr(Context &ctx, const T &value, const format_options<T> &options)
    {
    }

    static int exponent(T value, uint8_t radix)
    {
        if (!value)
            return INT_MIN;
        int bin_exp;
        switch (radix)
        {
        case 2:
            std::frexp(value, &bin_exp);
            return bin_exp;
        case 8:
            std::frexp(value, &bin_exp);
            return bin_exp < 0 ? (bin_exp - 2)/3 : bin_exp/3;
        case 16:
            std::frexp(value, &bin_exp);
            return bin_exp >> 2;
        default:
            break;
        }

        std::frexp(value, &bin_exp);
        //bin_exp = fpexp(value);
        int e = bin_exp * detail::ilog2(radix);
        //int e = std::floor(std::log2(std::abs(value)) * detail::ilog2(radix));
        T m = powi(radix, e);
        if (value < m)
        {
            do
            {
                e--;
                m /= radix;
            } while (value < m);
        }
        else if (value >= m * radix)
        {
            m *= radix;
            do
            {
                e++;
                m *= radix;
            }
            while (value > m);
        }
        return e;
    }

    template <typename Context>
    static void print(Context &ctx, const char *value, int len, int width)
    {
        if (len < width)
            put(ctx.out(), ' ', width - len);
        put(ctx.out(), value, len);
    }

};

template <typename StringLike>
class default_formatter<StringLike, StringType>
{
public:
    template <typename Context>
    static void format(Context &ctx, const StringLike &value, const format_options<StringLike> &options)
    {
        int len = string_length(value);
        if (options.precision >= 0 && options.precision < len)
            len = options.precision;
        if (len < options.width)
            put(ctx.out(), options.width - len, ' ');
        put(ctx.out(), value, len);
    }
};

template <typename T>
class formatter : public default_formatter<T> {};

template <typename Context, typename T>
struct format_param;

template <typename Context>
struct format_param_base
{
    virtual ~format_param_base() = default;

    virtual void format(Context &context, const char *format_str, size_t &format_index) const = 0;
    virtual void format(Context &context) const = 0;

    template <typename T>
    T &value()
    {
        return dynamic_cast<format_param<Context, T>*>(this)->value;
    }
};

template <typename Context, typename T>
struct format_param : format_param_base<Context>
{
    format_param(T v) : value(std::forward<T>(v)) {}
    T value;
    using formatted_type = typename std::remove_reference<T>::type;

    void format(Context &context, const char *format_str, size_t &format_index) const override
    {
        format_options<formatted_type> opt(format_str, format_index);
        formatter<formatted_type>::format(context, value, opt);
    }

    void format(Context &context) const override
    {
        formatter<formatted_type>::format(context, value, {});
    }
};

template <size_t index, typename T, typename...Args>
void get_tuple_addresses(T *array[], std::tuple<Args...> &tuple,
                        std::integral_constant<size_t, index>, std::integral_constant<size_t, 1>)
{
    array[index] = &std::get<index>(tuple);
}

template <size_t begin, size_t count, typename T, typename...Args>
void get_tuple_addresses(T *array[], std::tuple<Args...> &tuple,
                        std::integral_constant<size_t, begin>, std::integral_constant<size_t, count>)
{
    constexpr size_t n1 = count/2;
    constexpr size_t n2 = count - n1;
    get_tuple_addresses(array, tuple, std::integral_constant<size_t, begin>(), std::integral_constant<size_t, n1>());
    get_tuple_addresses(array, tuple, std::integral_constant<size_t, begin + n1>(), std::integral_constant<size_t, n2>());
}

template <typename T, typename...Args>
void get_tuple_addresses(T *array[], std::tuple<Args...> &tuple)
{
    get_tuple_addresses(array, tuple, std::integral_constant<size_t, 0>(), std::integral_constant<size_t, sizeof...(Args)>());
}


template <typename Context, typename... Args>
class format_params
{
public:
    format_params() = default;
    static constexpr size_t N = sizeof...(Args);

    format_params(Args&&... args)
    : storage(format_param<Context, Args>(std::forward<Args>(args))...)
    {
        get_tuple_addresses(params, storage);
    }

    static constexpr size_t size() { return N; }

    format_param_base<Context> &operator[](size_t index) const noexcept
    {
        return *params[index];
    }

private:
    format_param_base<Context> *params[N];
    std::tuple<format_param<Context, Args>...> storage;
};

template <typename Context>
class format_params<Context>
{
public:
    static constexpr size_t size() { return 0; }

    format_param_base<Context> &operator[](size_t index) const
    {
        throw std::logic_error("Trying to get a value from an empty argument list");
    }
};

template <typename Context, typename... Args>
format_params<Context, Args...> make_format_params(Args&&... args)
{
    return { std::forward<Args>(args)... };
}

inline int parse_index(const char *s, size_t &i)
{
    int index = -1;
    for (;; i++)
    {
        char c = s[i];
        if (is_ascii_digit(c))
        {
            if (index < 0)
                index = c - '0';
            else
                index = index*10 + (c - '0');
        }
        else if (c == ':' || c == '}')
            break;
        else
            throw std::logic_error("Invalid format string");
    }
    return index;
}

template <typename Context, typename FormatString, typename... Args>
void vformat(Context &ctx, const FormatString &format, const format_params<Context, Args...> &params)
{
    size_t len = string_length(format);
    const char *s = c_str(format);

    int last_idx = -1;
    size_t start = 0;

    for (size_t i = 0; i < len; i++)
    {
        switch (s[i])
        {
        case '{':
            if (s[++i] == '{')  // it's just a brace
            {
                put(ctx.out(), s + start, i - start);
                start = i + 1;
            }
            else
            {
                put(ctx.out(), s + start, i-1 - start);
                int index = ++last_idx;
                int N = params.size();
                int explicit_idx = parse_index(s, i);
                if (explicit_idx >= 0)
                    index = last_idx = explicit_idx;

                if (index < 0 || index >= N)
                    throw std::out_of_range("Argument index out of range: " + std::to_string(index));

                if (s[i] == ':')
                {
                    i++;
                    params[index].format(ctx, s, i);
                }
                else
                    params[index].format(ctx);


                while (s[i] != '}')
                {
                    if (!s[i])
                        throw std::logic_error("Missing closing brace in a format specfier");
                    i++;
                }
                start = i+1;
            }
            break;
        case '}':
            if (s[++i] == '}')  // use double closing braces, for symmetry
            {
                put(ctx.out(), s + start, i - start);
                start = i + 1;
            }
            break;
        }
    }
    put(ctx.out(), s + start, len - start);
}

template <typename Output, typename FormatString, typename... Args>
void format_to(output_context<Output> &context, const FormatString &format_string, Args&&... args)
{
    return vformat(context, format_string,
                   make_format_params<output_context<Output>>(std::forward<Args>(args)...));
}

template <typename Output, typename FormatString, typename... Args>
void format_to(Output &out, const FormatString &format_string, Args&&... args)
{
    output_context<Output &> ctx(out);
    format_to(ctx, format_string, std::forward<Args>(args)...);
}

template <typename FormatString, typename... Args>
std::string format_str(const FormatString &format_string, Args&&... args)
{
    std::string str;
    format_to(str, format_string, std::forward<Args>(args)...);
    return str;
}

template <typename FormatString, typename... Args>
void print(const FormatString &format_string, Args&&... args)
{
    format_to(std::cout, format_string, std::forward<Args>(args)...);
}

template <typename FormatString, typename... Args>
void vprint(const FormatString &format_string, const format_params<ostream_output_context, Args...> &params)
{
    ostream_output_context ctx(std::cout);
    vformat(ctx, format_string, params);
}

} // formatpp

#endif
