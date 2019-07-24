# BigInteger
BigInteger is a C library adding support for very large integers, making it possible e.g. to calculate factorials of big numbers. It is a simple project and I think the way of handling large integers here is not the fastest possible.

## Features
**Please note that BigInteger currently supports unsigned numbers only.**

* Creating a new instance of the big integer (BigInt), either unset or set to represent 0
* Converting a standard number to the equivalent BigInt instance
* Adding two BigInt instances
* Performing the following shift operations on a BigInt instance:
  * Logical shift left
  * Logical shift right
  * Arithmetic shift right
* Making a deep copy of a BigInt instance
* Multiplying a BigInt instance by a standard number
* Printing the number represented by a BigInt instance to a stream (**only in hex at the moment**)

## How to compile
### Unix-like systems (including Linux)
Clone this repository and type `make` in the console (you may want to change some settings beforehand: see `bigint.h`). A shared library named `libbigint.so` will be created then.

If you want to compile a simple factorial test program as well, type `make factorial_test`.

### Other systems (including Windows)
The library should work fine on systems other than Unix-like ones, e.g. Windows. However, the current version of Makefile does not support them.

## How to use
### Library
See `bigint.h` for the documentation.

### Factorial test program
Run the program (it is named `factorial_test` right after the successful compilation) and follow the prompts.

An execution example:

```
Enter n: 20
20! = 0x21C3677C82B40000
```

## Possible improvements
* Adding division operations
* Supporting signed numbers
* Printing BigInt instances in decimal
* Finding a faster way of handling large numbers

## License
(C) 2019 Maksymilian Graczyk. This project is licensed under GNU LGPL v3.0. See LICENSE for details.
