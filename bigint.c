#include "bigint.h"
#include "utils.h"
#include <stdlib.h>

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

BigInt *bigint_convert(long long number) {
  BigInt *result = bigint_new(false);

  if (!result) {
    return NULL;
  }
  
  long long current_mask = CONVERSION_MASK;

  int parts_count = 0;

  while ((number & current_mask) != 0) {
    parts_count++;
    current_mask <<= BITS_PER_PART_ACTUAL;
  }

  result->parts_count = parts_count;
  result->parts = calloc(parts_count, sizeof(uint_t));

  if (!result->parts) {
    bigint_free(result);
    return NULL;
  }

  for (int i = 0; i < parts_count; i++) {
    uint_t part = (number & (CONVERSION_MASK << (BITS_PER_PART_ACTUAL * i)))
      >> (BITS_PER_PART_ACTUAL * i);
    result->parts[i] = part;
  }

  return result;
}

bool bigint_add(BigInt *number, BigInt *to_add) {
  int new_parts_count = max(number->parts_count, to_add->parts_count);
  int old_parts_count = number->parts_count;

  if (!realloc(number->parts, new_parts_count * sizeof(uint_t))) {
    return false;
  }

  number->parts_count = new_parts_count;

  for (int i = old_parts_count; i < new_parts_count; i++) {
    number->parts[i] = 0;
  }

  bool part_overflow = false;

  for (int i = 0; i < to_add->parts_count; i++) {
    uint_t number_part = number->parts[i];
    number->parts[i] = number_part + to_add->parts[i] + part_overflow;
    part_overflow = number->parts[i] < min(number_part, to_add->parts[i]);
  }

  if (part_overflow) {
    new_parts_count++;

    if (!realloc(number->parts, new_parts_count * sizeof(uint_t))) {
      return false;
    }

    number->parts_count = new_parts_count;
    number->parts[new_parts_count - 1] = 1;
  }

  return true;
}

void bigint_lsl(BigInt *number, int bits) {
  for (int i = 0; i < bits; i++) {
    uint_t carry = 0;
    for (int j = 0; j < number->parts_count; j++) {
      uint_t new_part = (number->parts[j] << 1) | carry;
      carry = number->parts[j] >> (BITS_PER_PART_ACTUAL - 1);
      number->parts[j] = new_part;
    }
  }
}

static void _bigint_sr(BigInt *number, int bits, uint_t initial_carry) {
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

void bigint_lsr(BigInt *number, int bits) {
  _bigint_sr(number, bits, 0);
}

void bigint_asr(BigInt *number, int bits) {
  _bigint_sr(number, bits,
	     number->parts[number->parts_count - 1]
	     >> (BITS_PER_PART_ACTUAL - 1));
}

BigInt *bigint_multiply(BigInt *number, long long multiply_by) {
  
}

bool bigint_print(BigInt *number, FILE *stream) {
  //TODO
}
