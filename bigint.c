#include "bigint.h"
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

BigInt *bigint_new(void) {
  BigInt *number = calloc(1, sizeof(BigInt));
  return number;
}

void bigint_free(BigInt *number) {
  free(number->parts);
  free(number);
}

BigInt *bigint_convert(long long number) {
  BigInt *result = bigint_new();

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
  //TODO
}

void bigint_lsl(BigInt *number, int bits) {
  for (int i = 0; i < bits; i++) {
    uint8_t carry = 0;
    for (int j = 0; j < number->parts_count; j++) {
      uint8_t new_part = (number->parts[j] << 1) | carry;
      carry = number->parts[j] >> (BITS_PER_PART_ACTUAL - 1);
      number->parts[j] = new_part;
    }
  }
}

static void _bigint_sr(BigInt *number, int bits, uint8_t initial_carry) {
  for (int i = 0; i < bits; i++) {
    uint8_t carry = initial_carry;
    for (int j = number->parts_count - 1; j >= 0; j--) {
      uint8_t new_part = (number->parts[j] >> 1)
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

bool bigint_multiply(BigInt *number, long long multiply_by) {
  //TODO
}

bool bigint_print(BigInt *number, FILE *stream) {
  //TODO
}
