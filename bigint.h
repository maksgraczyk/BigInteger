#ifndef BIGINTEGER_BIGINT_H
#define BIGINTEGER_BIGINT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  uint8_t *parts;
  int parts_count;
} BigInt;

/*
Initialises a new BigInt instance with no parts (parts_count = 0, parts = NULL).
Returns NULL if memory allocation fails.
*/
BigInt *bigint_new(void);

/*
Frees memory allocated for a BigInt instance.
*/
void bigint_free(BigInt *number);

/*
Converts a long long number to an equivalent BigInt instance.
Returns NULL if the operation fails (e.g. due to an memory allocation error).
*/
BigInt *bigint_convert(long long number);

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
void bigint_lsl(BigInt *number, int bits);

/*
Performs a logical shift right on a BigInt instance by a specified number
of bits.
*/
void bigint_lsr(BigInt *number, int bits);

/*
Performs an arithmetic shift right on a BigInt instance by a specified number
of bits.
*/
void bigint_asr(BigInt *number, int bits);

/*
Multiplies a BigInt instance by a long long number in the following way:
number = number * multiply_by

Returns true if the operation succeeds. Otherwise, false is returned.

Memory allocated for number will be extended if required. if not enough
memory can be allocated for the result, the operation fails.
*/
bool bigint_multiply(BigInt *number, long long multiply_by);

/*
Prints the number represented by a BigInt instance to a specified stream.
Returns true if the operation succeeds. Otherwise, false is returned.
*/
bool bigint_print(BigInt *number, FILE *stream);



#endif
