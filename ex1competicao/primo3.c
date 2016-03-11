#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LIMIT 1000000000 /*size of integers array*/

int main(int argc, char *argv[]){
    unsigned long long int i,j, k;
    int *primes;
    int z = 2, n;

    n = atoi(argv[1]);

    primes = malloc(sizeof(int)*LIMIT);

    if (n <= 100)
        k = n*10;
    else
        k = n*(log(n) + log(log(n)));

    if(k >= LIMIT)
        k = LIMIT/2;


    primes[2] = 1;
    for (i=3;i<k;i+=2)
        primes[i]=1;

    for (i=3;i<k;i+=2)
        if (primes[i])
            for (j=i*i;j<k;j+=2*i)
                primes[j]=0;

    if (n == 1)
        printf("%d",2);
    else {
        for (i=3;i<k;i+=2) {
            if (primes[i]) {
              if(z == n)
                printf("%llu",i);
              z++;
            }
        }
    }

return 0;
}
