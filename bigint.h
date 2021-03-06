#ifndef BIGINTEGER_BIGINT_H
#define BIGINTEGER_BIGINT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define stdnum_t unsigned long long

/*
You can specify the number of bits per part via the BITS_PER_PART preprocessor
macro below.

The following values are accepted: 8, 16, 32, 64 (on 64-bit architectures only,
e.g. x86_64). If a non-supported number is supplied, the library falls back to
8 bits per part.
*/
#define BITS_PER_PART 8

#if BITS_PER_PART == 16

#define BITS_PER_PART_ACTUAL 16
#define uint_t uint16_t
#define int_t int16_t

#elif BITS_PER_PART == 32

#define BITS_PER_PART_ACTUAL 32
#define uint_t uint32_t
#define int_t int32_t

#elif BITS_PER_PART == 64 && _____LP64_____

#define BITS_PER_PART_ACTUAL 64
#define uint_t uint64_t
#define int_t int64_t

#else

#define BITS_PER_PART_ACTUAL 8
#define uint_t uint8_t
#define int_t int8_t

#endif

typedef struct {
  uint_t *parts;
  int parts_count;
} BigInt;

/*
Initialises a new BigInt instance.
If single_part is true, the instance has a single part equal to 0
(parts_count = 1, parts = [0]). Otherwise, the instance has no parts
(parts_count = 0, parts = NULL).
Returns NULL if memory allocation fails.
*/
BigInt *bigint_new(bool single_part);

/*
Frees memory allocated for a BigInt instance.
*/
void bigint_free(BigInt *number);

/*
Converts a standard number to an equivalent BigInt instance.
Returns NULL if the operation fails (e.g. due to an memory allocation error).
*/
BigInt *bigint_convert(stdnum_t number);

/*
Adds two BigInt instances to one another in the following way:
number = number + to_add

Returns true if the operation succeeds. Otherwise, false is returned.

Memory allocated for number will be extended if required. If not enough
memory can be allocated for the result, the operation fails.
*/
bool bigint_add(BigInt *number, BigInt *to_add);

/*
Performs a logical shift left on a BigInt instance by a specified number
of bits.

Please note that an arithmetic shift left is the same as a logical shift left.
*/
void bigint_lsl(BigInt *number, unsigned int bits);

/*
Performs a logical shift right on a BigInt instance by a specified number
of bits.
*/
void bigint_lsr(BigInt *number, unsigned int bits);

/*
Performs an arithmetic shift right on a BigInt instance by a specified number
of bits.
*/
void bigint_asr(BigInt *number, unsigned int bits);

/*
Makes a deep copy of a specified BigInt instance, creating additional parts
equal to 0 at the end of the parts array as well if extra_zero_parts is greater
than 0. The exact number of those parts is determined by extra_zero_parts.

Returns NULL if memory allocation fails.
*/
BigInt *bigint_copy(BigInt *number, unsigned int extra_zero_parts);

/*
Multiplies a BigInt instance by a standard number in the following way:
result = number * multiply_by

Returns the result if the operation succeeds. Otherwise, NULL is returned.

Memory allocated for number will be extended if required. if not enough
memory can be allocated for the result, the operation fails.
*/
BigInt *bigint_multiply(BigInt *number, stdnum_t multiply_by);

/*
Prints the number represented by a BigInt instance to a specified stream.
Base 16 is used.

Returns true if the operation succeeds. Otherwise, false is returned.
*/
bool bigint_print(BigInt *number, FILE *stream);



#endif
