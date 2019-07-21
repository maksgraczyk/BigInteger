#include "bigint.h"
#include <stdlib.h>

#define MASK_8 0x00000000000000FF

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
  
  long long current_mask = MASK_8;

  int parts_count = 0;

  while ((number & current_mask) != 0) {
    parts_count++;
    current_mask <<= 8;
  }

  result->parts_count = parts_count;
  result->parts = calloc(parts_count, sizeof(uint8_t));

  if (!result->parts) {
    bigint_free(result);
    return NULL;
  }

  for (int i = 0; i < parts_count; i++) {
    uint8_t part = (number & (MASK_8 << (8 * i))) >> (8 * i);
    result->parts[i] = part;
  }

  return result;
}

bool bigint_add(BigInt *number, BigInt *to_add) {
  //TODO
}

void bigint_lsl(BigInt *number, int bits) {
  //TODO
}

void bigint_lsr(BigInt *number, int bits) {
  //TODO
}

void bigint_asr(BigInt *number, int bits) {
  //TODO
}

bool bigint_multiply(BigInt *number, long long multiply_by) {
  //TODO
}

bool bigint_print(BigInt *number, FILE *stream) {
  //TODO
}
