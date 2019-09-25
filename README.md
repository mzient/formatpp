# format++

A C++ library replacing `printf`.

## Main features

* **Small** - Core library is < 2000 LoC

* **Header-only** - No link-time dependencies, symbol import/export, etc.

* **Type-safe** - When format string doesn't match the type, exception is thrown

* **Automatic default type** - `print("{:05}", 123)` => `int` inferred; prints `00123`

* **Positional arguments** - `print("{1} {0}", "latter", "former")` prints `former latter`

* **Support for custom types** - custom output formatting, custom format specifiers

* **Familiar printf-like format specifiers**
```
print("{0:08x}", 255)    =>  000000ff
print("{0:1.5f}", 1.5)   =>  1.50000
print("{0:1.5g}", 1.5)   =>  1.5`
print("{0:.5e}", 1500.f) =>  1.50000e+3
```

* **Can print floating point numbers in hex and binary**
```
print("{af}", 16.25)      => 10.40000 // hex float, fixed (f)
print("{bg}", 1.125)      => 1.001    // binary float, automatic (g)
print("{x}, 1.0f)         => 3f800000 // IEEE754 representation
```
