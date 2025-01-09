#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

int factorial_mod(int n, int mod);

int main(){
    srand(0);
    const int NUMBER_COUNT = 6000;
    const int MINIMUM = 100000; // 10^5
    const int MAXIMUM = 1000000;// 10^6
    int range_length = MAXIMUM - MINIMUM;

    int numbers[NUMBER_COUNT];
    
    for(int i = 0; i < NUMBER_COUNT; i++){
        numbers[i] = (rand() % range_length) + MINIMUM;
    }
    
    // printf("initialized!\n");

    for(int i = 0; i < NUMBER_COUNT; i++){
        numbers[i] = factorial_mod(numbers[i], numbers[i] + 1);
        // printf("factorial_mod for i: %d calculated.\n", i);
    }
    
    long long sum = 0;
    for(int i = 0; i < NUMBER_COUNT; i++){
        sum += (long long)numbers[i];
    }
    printf("sum: %lld\n", sum);
}

int factorial_mod(int n, int mod){
    int result = 1;
    for(int i = 2; i <= n; i++){
        result *= i;
        result %= mod;
    }
    return result;
}
// n! % (n + 1)