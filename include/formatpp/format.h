#ifndef FORMATPP_CORE_H_
#define FORMATPP_CORE_H_

#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <memory>
#include <iterator>
#include <vector>
#include <iostream>
#include <type_traits>

namespace formatpp {

using std::size_t;
using std::ptrdiff_t;

enum class fp_format_mode
{
    /// Use either positional or exponential best fit
    automatic,
    /// Always use positional notation, even for very large or small numbers
    positional,
    /// Always use scientific (exponential) notation
    scientific
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

#if __cplusplus >= 201402L
using std::make_unique;
#else
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif

class IntegralType;
class FloatingPointType;
class PointerType;
class StringType;
class CharType;
class BooleanType;

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
    int radix = 10;
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
        }
    }

    int width = -1;
    int precision = -1;
    int radix = 10;
    char leading_sign = 0;
    char leading_char = ' ';
    bool uppercase = false;
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
    void parse(const char *options, size_t i)
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

inline void put(std::ostream &s, char c)
{
    s.put(c);
}

inline void append(std::string &s, char c)
{
    s += c;
}

inline const char *c_str(const std::string &s) { return s.c_str(); }
inline const char *c_str(const char *s) { return s; }

template <typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value>
put(std::ostream &s, const StringLike &value, size_t max_len)
{
    s.write(c_str(value), std::min(max_len, string_length(value)));
}

template <typename StringLike>
inline enable_if_t<is_string_type<StringLike>::value>
put(std::string &s, const StringLike &value, size_t max_len)
{
    s.append(c_str(value), std::min(max_len, string_length(value)));
}

inline void put(std::ostream &s, size_t n, char value)
{
    std::fill_n(std::ostream_iterator<char>(s), n, value);
}

inline void put(std::string &s, size_t n, char value)
{
    s.append(n, value);
}

template <typename Output>
struct output_context
{
    output_context(Output output) : output(output) {}

    using buf_lease = std::unique_ptr<char[]>;

    buf_lease get_temp_buffer(size_t count)
    {
        return buf_lease(new char[count]);
    }

    typename std::add_lvalue_reference<Output>::type out()
    {
        return output;
    }

    Output output;
};

using string_output_context = output_context<std::string &>;

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
        auto buf_size = std::max<size_t>(sizeof(T)*8 + 2, options.width+2);
        auto buf_lease = ctx.get_temp_buffer(buf_size);
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
            while (x)
            {
                rbuf[-++n] = '0' + (x&1);
                x >>= 1;
            }
            break;
        case 8:
            while (x)
            {
                rbuf[-++n] = '0' + (x&7);
                x >>= 3;
            }
            break;
        case 10:  // use compiler's optimization for division by 10
            while (x)
            {
                auto d = x % 10;
                x /= 10;
                rbuf[-++n] = options.digits[d];
            }
        case 16:
            while (x)
            {
                rbuf[-++n] = options.digits[x&15];
                x >>= 4;
            }
            break;
        default:
            while (x)
            {
                auto d = x % options.radix;
                x /= options.radix;
                rbuf[-++n] = options.digits[d];
            }
            break;
        }

        while (n < options.precision)
            rbuf[-++n] = options.digits[0];

        if (!n && options.precision != 0)
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
};

template <typename T>
struct default_formatter<T, FloatingPointType>
{
    template <typename Context>
    static void format(Context &ctx, const T &value, const format_options<T> &options)
    {
        auto buf_size = std::max<size_t>(sizeof(T)*8 + 10, options.width+2);
        auto buf_lease = ctx.get_temp_buffer(buf_size);
        char *buf = buf_lease.get();
        int i = 0;
        char format_str[16] = {0};
        format_str[i++] = '%';

        if (options.leading_sign)
            format_str[i++] = options.leading_sign;
        if (options.leading_char == '0')
            format_str[i++] = '0';
        if (options.width >= 0)
            i+=snprintf(format_str + i, sizeof(format_str) - i, "%i", options.width);
        if (options.precision >= 0)
            i+=snprintf(format_str + i, sizeof(format_str) - i, ".%i", options.precision);

        switch (options.fp_mode)
        {
        case fp_format_mode::automatic:
            format_str[i++] = options.uppercase ? 'G' : 'g';
            break;
        case fp_format_mode::positional:
            format_str[i++] = 'f';
            break;
        case fp_format_mode::scientific:
            format_str[i++] = options.uppercase ? 'E' : 'e';
            break;
        }
        format_str[i++] = 0;
        snprintf(buf, buf_size, format_str, value);
        put(ctx.out(), buf);
    }
};

template <typename StringLike>
class default_formatter<StringLike, StringType>
{
public:
    template <typename Context>
    static void format(Context &ctx, const StringLike &value, const format_options<StringLike> &options)
    {
        auto len = string_length(value);
        if (options.precision >= 0 && options.precision < len)
            len = options.precision;
        if (len < options.width)
            put(ctx.out(), options.width - len, ' ');
        put(ctx.out(), value, len);
    }
};

template <typename T>
class formatter : public default_formatter<T> {};

/*template <typename T>
struct format_options_wrapper;

struct format_options_wrapper_base
{
    virtual ~format_options_wrapper_base() = default;
    template <typename T>
    format_options<T> *get()
    {
        return dynamic_cast<format_options_wrapper<T>*>(this);
    }
};

template <typename T>
struct format_options_wrapper : format_options_wrapper_base, format_options<T>
{
    using format_options<T>::format_options;
};*/

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

template <typename Context, size_t N>
class format_params
{
public:
    format_params() = default;

    template <typename... Args>
    format_params(Args&&... args)
    : params{ make_unique<format_param<Context, Args>>(std::forward<Args>(args))... }
    {
        static_assert(sizeof...(Args) == N, "Invalid number of arguments");
    }

    static constexpr size_t size() { return N; }

    format_param_base<Context> &operator[](size_t index) const {
        return *params[index];
    }
private:
    std::unique_ptr<format_param_base<Context>> params[N];
};

template <typename Context>
class format_params<Context, (size_t)-1>
{
public:
    format_params() = default;

    template <typename... Args>
    format_params(Args&&... args)
    {
        size_t N = sizeof...(Args);
        std::unique_ptr<format_param_base<Context>> params[] = {
            make_unique<format_param<Context, Args>>(std::forward<Args>(args))...
        };
        this->params.reserve(N);
        for (auto &p : params)
            this->params.push_back(std::move(p));
    }

    size_t size() const { return params.size(); }

    format_param_base<Context> &operator[](size_t index) const {
        return *params[index];
    }
private:

    std::vector<std::unique_ptr<format_param_base<Context>>> params;
};

template <typename Context, typename... Args>
format_params<Context, sizeof...(Args)> make_format_params(Args&&... args)
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

template <typename Context, typename FormatString, size_t N>
void vformat(Context &ctx, const FormatString &format, const format_params<Context, N> &params)
{
    size_t len = string_length(format);
    const char *s = c_str(format);

    struct text_chunk
    {
        const char *start;
        size_t length;
    };

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
                int explicit_idx = parse_index(s, i);
                if (explicit_idx >= 0)
                    index = last_idx = explicit_idx;

                if (index < 0 || index >= params.size())
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

} // formatpp

#endif
