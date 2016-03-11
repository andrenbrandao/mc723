#include <stdio.h>
#include <stdlib.h>

#define LIMIT 100000000 /*size of integers array*/

int main(int argc, char *argv[]){
    unsigned long long int i,j;
    int *primes;
    int z = 1, n;

    n = atoi(argv[1]);

    primes = malloc(sizeof(int)*LIMIT);

#pragma omp parallel for private(i) schedule(dynamic)
    for (i=2;i<LIMIT;i++)
        primes[i]=1;

    for (i=2;i<LIMIT;i++)
        if (primes[i])
            for (j=i;i*j<LIMIT;j++)
                primes[i*j]=0;

#pragma omp parallel for private(i) schedule(dynamic)
    for (i=2;i<LIMIT;i++) {
        if (primes[i]) {
          if(z == n)
            printf("%dth prime = %d\n",z,i);
          z++;
        }
    }

return 0;
}