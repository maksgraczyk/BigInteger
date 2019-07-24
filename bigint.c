#include "bigint.h"
#include "utils.h"
#include <stdlib.h>
#include <math.h>

#if BITS_PER_PART_ACTUAL == 16
#define CONVERSION_MASK 0xFFFF
#elif BITS_PER_PART_ACTUAL == 32
#define CONVERSION_MASK 0xFFFFFFFF
#elif BITS_PER_PART_ACTUAL == 64
#define CONVERSION_MASK 0xFFFFFFFFFFFFFFFF
#else
#define CONVERSION_MASK 0xFF
#endif

#define MASK_1 0x1

BigInt *bigint_new(bool single_part) {
  BigInt *number = calloc(1, sizeof(BigInt));

  if (!number) {
    return NULL;
  }

  if (single_part) {
    number->parts = calloc(1, sizeof(uint_t));
    number->parts_count = 1;
  }
  
  return number;
}

void bigint_free(BigInt *number) {
  free(number->parts);
  free(number);
}

BigInt *bigint_convert(stdnum_t number) {
  BigInt *result = bigint_new(false);

  if (!result) {
    return NULL;
  }

  stdnum_t current_mask = CONVERSION_MASK;
  stdnum_t number_copy = number;
  int parts_count = 0;

  while (number_copy != 0) {
    parts_count++;
    number_copy >>= BITS_PER_PART_ACTUAL;
  }

  result->parts_count = parts_count;
  result->parts = calloc(parts_count, sizeof(uint_t));

  if (!result->parts) {
    bigint_free(result);
    return NULL;
  }

  for (int i = 0; i < parts_count; i++) {
    uint_t part = (number >> (BITS_PER_PART_ACTUAL * i)) & CONVERSION_MASK;
    result->parts[i] = part;
  }

  return result;
}

/*
Local function: _bit_add
Adds two numbers using bitwise operations and stores the result under result_ptr.
Returns true if overflow has occurred. Otherwise, false is returned.
*/
static bool _bit_add(uint_t first, uint_t second, uint_t *result_ptr) {
  uint_t result = first ^ second;
  uint_t carry = first & second;
  bool overflow = false;

  while (carry != 0) {
    if (carry >> (BITS_PER_PART_ACTUAL - 1)) {
      overflow = true;
    }
    
    uint_t new_result = result ^ (carry << 1);
    carry = result & (carry << 1);
    result = new_result;
  }

  *result_ptr = result;

  return overflow;
}

bool bigint_add(BigInt *number, BigInt *to_add) {
  int new_parts_count = max(number->parts_count, to_add->parts_count);
  int old_parts_count = number->parts_count;

  uint_t *realloc_result = realloc(number->parts,
				   new_parts_count * sizeof(uint_t));
  
  if (!realloc_result) {
    return false;
  }

  number->parts = realloc_result;
  number->parts_count = new_parts_count;

  for (int i = old_parts_count; i < new_parts_count; i++) {
    number->parts[i] = 0;
  }

  bool part_overflow = false;

  for (int i = 0; i < number->parts_count; i++) {
    uint_t part_from_to_add = i < to_add->parts_count ? to_add->parts[i] : 0;
    bool overflow_in_step_1 = _bit_add(number->parts[i], part_from_to_add,
				       &number->parts[i]);
    bool overflow_in_step_2 = _bit_add(number->parts[i], part_overflow,
				       &number->parts[i]);
    part_overflow = overflow_in_step_1 || overflow_in_step_2;
  }

  if (part_overflow) {
    new_parts_count++;

    realloc_result = realloc(number->parts, new_parts_count * sizeof(uint_t));

    if (!realloc_result) {
      return false;
    }

    number->parts = realloc_result;

    number->parts_count = new_parts_count;
    number->parts[new_parts_count - 1] = 1;
  }

  return true;
}

void bigint_lsl(BigInt *number, unsigned int bits) {
  for (int i = 0; i < bits; i++) {
    uint_t carry = 0;
    for (int j = 0; j < number->parts_count; j++) {
      uint_t new_part = (number->parts[j] << 1) | carry;
      carry = number->parts[j] >> (BITS_PER_PART_ACTUAL - 1);
      number->parts[j] = new_part;
    }
  }
}

/*
Local function: _bigint_sr
Performs a right shift on a BigInt instance by a specified number of bits,
using a specified initial carry.
*/
static void _bigint_sr(BigInt *number, unsigned int bits, uint_t initial_carry) {
  for (int i = 0; i < bits; i++) {
    uint_t carry = initial_carry;
    for (int j = number->parts_count - 1; j >= 0; j--) {
      uint_t new_part = (number->parts[j] >> 1)
	| (carry << (BITS_PER_PART_ACTUAL - 1));
      carry = number->parts[j] & MASK_1;
      number->parts[j] = new_part;
    }
  }
}

void bigint_lsr(BigInt *number, unsigned int bits) {
  _bigint_sr(number, bits, 0);
}

void bigint_asr(BigInt *number, unsigned int bits) {
  _bigint_sr(number, bits,
	     number->parts[number->parts_count - 1]
	     >> (BITS_PER_PART_ACTUAL - 1));
}

BigInt *bigint_copy(BigInt *number, unsigned int extra_zero_parts) {
  BigInt *copy = bigint_new(false);

  if (!copy) {
    return NULL;
  }

  copy->parts_count = number->parts_count + extra_zero_parts;
  copy->parts = calloc(copy->parts_count, sizeof(uint_t));

  if (!copy->parts) {
    bigint_free(copy);
    return NULL;
  }

  for (int i = 0; i < number->parts_count; i++) {
    copy->parts[i] = number->parts[i];
  }

  return copy;
}

BigInt *bigint_multiply(BigInt *number, stdnum_t multiply_by) {
  BigInt *result = bigint_new(true);

  if (!result) {
    return NULL;
  }

  if (multiply_by != 0) {
    BigInt *to_add = bigint_copy(number,
				 (log2l(multiply_by) + 1)
				 / BITS_PER_PART_ACTUAL + 1);

    if (!to_add) {
      bigint_free(result);
      return NULL;
    }

    do {
      if ((multiply_by & MASK_1) && !bigint_add(result, to_add)) {
	bigint_free(to_add);
	bigint_free(result);
	return NULL;
      }
      
      multiply_by >>= 1;
      bigint_lsl(to_add, 1);
    } while (multiply_by != 0);

    bigint_free(to_add);
  }

  return result;
}

bool bigint_print(BigInt *number, FILE *stream) {
  if (fprintf(stream, "0x") < 0) {
    return false;
  }

  bool print_zero_parts = false;
  
  for (int i = number->parts_count - 1; i >= 0; i--) {
    if (number->parts[i] != 0 || print_zero_parts) {
      print_zero_parts = true;
      
      if (fprintf(stream, "%02X", number->parts[i]) < 0) {
	return false;
      }
    }
  }

  return true;
}
