#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include <common.h>
#include <utils/vector.h>
#include <utils/euler.h>
#include <utils/btable.h>
#include <utils/bignum.h>

/* Note:
 * またかよと重いつつ。完全にやるだけ
 */

DECLARE_VECTOR(bignum_uint_t);

int primes[7] = {2,3,5,7,11,13,17};

static uint64_t check(int digits[10])
{
    int i;
    uint64_t x = 0;
    int tmp;

    
    for(i = 0; i < 10; i++) {
        x *= 10;
        x += digits[i];
    }
    

    for(i = 1; i < 8; i++) {
        tmp = digits[i] * 100 + digits[i + 1] * 10 + digits[i + 2];
        if(tmp % primes[i - 1] != 0) return 0;
    }
    
    for(i = 1; i < 8; i++) {
        tmp = digits[i] * 100 + digits[i + 1] * 10 + digits[i + 2];
    }
    
    DUMPL(x);

    return x;
}

static void iter_pandigital(
        int used[10], 
        int digits[10], 
        int pivot, 
        uint64_t *p_result)
{
    if(pivot == 10) {
        *p_result += check(digits);
        return;
    }

    int i;

    for(i = 0; i < 10; i++) {
        if(!used[i]) {
            digits[pivot] = i;
            used[i] = 1;
            iter_pandigital(used, digits, pivot + 1, p_result);
            used[i] = 0;
        }
    }
}

int main(int argc, char **argv)
{
    int n_in = 0;
    int x_in = 0;

    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                n_in = atoi(optarg);
                break;
            case 'x':
                x_in = atoi(optarg);
                break;
        }
    }


    uint64_t result = 0;

    int digits[10] = {};
    int used[10] = {};

    iter_pandigital(used,digits,0,&result);

    DUMPL(result); 

    return 0;
}
