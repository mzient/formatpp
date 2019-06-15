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
        return 53 / std::log2(radix);
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
        return 23 / std::log2(radix);
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
    static void write_fixed(char *rbuf, typename std::make_unsigned<T>::type x, int &n,
                            uint8_t dynamic_radix, const char *digits,
                            int fixed_point, bool &trim_trailing_zeros)
    {
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
        T m = powi(options.radix, fractional_digits ? precision : precision-e-1);
        T sig, rem;
        rem = std::modf(value*m, &sig);
        if (rem > 0.5)
            sig++;
        value = sig / m;
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
    static U powi(unsigned base, int e) {
        if (e < 0)
            return 1.0 / powi<U>(base, -e);

        U result = 1;
        U tmp = base;
        while (e > 0) {
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
    static void positional(Context &ctx, T value, int exponent, const format_options<T> &options, bool is_auto)
    {
        int precision = options.precision >= 0 ? options.precision : 6;

        int digits = is_auto ? precision : std::max(exponent, 0) + precision + 1;

        const int max_digits_31 = detail::max_digits_31[options.radix];
        const int max_digits_63 = detail::max_digits_63[options.radix];
        if (digits <= max_digits_31)
            positional_impl<int>(ctx, value, exponent, options, digits, is_auto);
        else if (digits <= max_digits_63)
            positional_impl<int64_t>(ctx, value, exponent, options, digits, is_auto);
        else
        {
            auto opt = options;
            int point = exponent < max_digits_63 || (!is_auto && options.precision > 0);
            int zeros = is_auto ? (detail::max(0, exponent - max_digits_63)) : digits - max_digits_63;

            if (opt.width > 0)
                opt.width = detail::max(1, opt.width - (point + zeros));

            if (exponent >= max_digits_63)
            {
                double tmp = value*powi<double>(opt.radix, max_digits_63-exponent);
                positional_impl<int64_t>(ctx, tmp, max_digits_63, opt, max_digits_63, true);
            }
            else
                positional_impl<int64_t>(ctx, value, exponent, opt, max_digits_63, is_auto);

            if (exponent > max_digits_63)
                put(ctx.out(), exponent - max_digits_63, options.digits[0]);
            if (!is_auto && options.precision > 0)
            {
                put(ctx.out(), 1, '.');
                put(ctx.out(), options.precision, options.digits[0]);
            }
        }
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
    template <typename Context>
    static void binary_repr(Context &ctx, const T &value, const format_options<T> &options)
    {
    }

    static int exponent(T value, uint8_t radix)
    {
        int bin_exp;
        switch (radix)
        {
        case 2:
            std::frexp(value, &bin_exp);
            return bin_exp;
        case 8:
            std::frexp(value, &bin_exp);
            return bin_exp < 0 ? (bin_exp - 2)/3 : bin_exp/3;
        case 10:
            return std::floor(std::log10(std::abs(value)));
        case 16:
            std::frexp(value, &bin_exp);
            return bin_exp >> 2;
        default:
            return std::floor(log(std::abs(value)) / log(radix));
        }
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
