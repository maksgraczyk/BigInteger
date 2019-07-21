#include "bigint.h"
#include <stdlib.h>

BigInt *bigint_new(void) {
  BigInt *number = calloc(1, sizeof(BigInt));
  return number;
}

void bigint_free(BigInt *number) {
  free(number->parts);
  free(number);
}

BigInt *bigint_convert(long long number) {
  //TODO
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
