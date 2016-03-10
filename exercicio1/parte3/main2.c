#include <stdio.h>
#include "calc_primo2.h"

main(int argc, char *argv[])
{
  int n, i;
  int count = 0;

  n = atoi(argv[1]);

  for(i = 2; i <= n; i ++) {
    if(primo(i))
      count++;
  }

  printf("Existem %d primos entre 1 e %d\n", count, n);
}