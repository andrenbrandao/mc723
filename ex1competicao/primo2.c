#include <stdio.h>
#include <stdlib.h>

#define LIMIT 100000000 /*size of integers array*/

int main(int argc, char *argv[])
{
    int inputNumber, n; // users input
    int TheseArePrime = 0; // variable used in the array that stores the prime numbers found
    int *isPrime;
    int totalPrimes = 0;

    inputNumber = atoi(argv[1]);

    isPrime = malloc(sizeof(int)*inputNumber);

    n = inputNumber / 2;

     for (int i = 0; i < inputNumber; i++)
     {
        isPrime[i] = i + 1;
     }

     for (int i = 1; i < n; i++)
     {
         for (int j = i;  j<= (n-i)/(2*i+1); j++)
         {
            isPrime[i + j + 2 * i * j] = 0;/*From this list, remove all
                                                  numbers of the form i + j + 2ij    */
         }
     }
        if (inputNumber >= 2)
        {
            isPrime[TheseArePrime++] = 2;/*this IF statement adds 2 to the output since 2 is a prime number    */
            totalPrimes++;
        }
         for (int i = 1; i < n; i++)
        {
            if (isPrime[i] != 0)
            {
                isPrime[TheseArePrime++]=i*2+1;
                                                    /*The remaining numbers are
                                                    doubled and incremented by
                                                    one, giving a list of the
                                                    odd prime numbers (i.e., all
                                                    primes except the only even
                                                    prime 2) below 2n + 2.  
                                                     */
                totalPrimes++;
            }
        }
 
/*  Output Prime Numbers */
 
        for (int x = 0; x < totalPrimes; x++)
        {
            if (isPrime[x]!= 0)
            {
                printf("%d\n", isPrime[x]);//outputs all prime numbers found
            }
            else
            {
                break;
            }
        }

                    // NOTE: this outputs the number of primes up to number input by the user.
    return 0;
}