#include "bigint.h"
#include <stdlib.h>

int main(void) {
  int n;
  printf("Enter n: ");
  scanf("%d", &n);

  BigInt *number = bigint_convert(1);

  if (!number) {
    printf("Conversion error!\n");
    return EXIT_FAILURE;
  }

  for (int i = 2; i <= n; i++) {
    BigInt *result = bigint_multiply(number, i);
    if (!result) {
      printf("Multiplication error!\n");
      return EXIT_FAILURE;
    }

    bigint_free(number);
    number = result;
  }

  printf("%d! = ", n);
  bigint_print(number, stdout);
  printf("\n");

  return EXIT_SUCCESS;
}
