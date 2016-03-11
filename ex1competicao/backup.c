#include <stdio.h>
#include <stdlib.h>

#define LIMIT 100000000 /*size of integers array*/

int main(int argc, char *argv[]){
    unsigned long long int i,j;
    int *primes;
    int z = 2, n, k;

    n = atoi(argv[1]);

    primes = malloc(sizeof(int)*LIMIT);

    k = LIMIT/2;

    primes[2] = 1;
    for (i=3;i<LIMIT;i+=2)
        primes[i]=1;

    for (i=3;i<k;i+=2)
        if (primes[i])
            for (j=i*i;j<LIMIT;j+=2*i)
                primes[j]=0;

    if (n == 1)
        printf("%llu",2);
    else {
	    for (i=3;i<LIMIT;i+=2) {
	        if (primes[i]) {
	          if(z == n)
	            printf("%llu",i);
	          z++;
	        }
	    }
	}

return 0;
}
