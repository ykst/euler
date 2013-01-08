#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note 
 * ほとんどそのままで、計算量的にも大丈夫的な。本来はハッシュを使うべきだっただろう。
 * 0,1と、負の数を素数判定から外し損ねて1ミス
 */

static inline bool is_prime(int x)
{
    int limit = sqrt(x);
    int i;

    if(x <= 1) return false;

    if((x & 1) || x == 2 || x == 3) {
        for(i = 3; i  <= limit; i += 2) {
            if((x % i) == 0) {
                return false;
            }
        }
        return true;
    }

    return false;
}

static inline int calc_quadratic(int x, int coef_a, int coef_b)
{
    return x * x + x * coef_a + coef_b;
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
            case 'n':
                DUMPD(calc_quadratic(atoi(optarg), 1, 41));
                exit(EXIT_SUCCESS);
            case 'p':
                DUMPS(is_prime(atoi(optarg)) ? "yes" : "no");
                exit(EXIT_SUCCESS);
        }
    }

    int coeff_bs[1000] = {};
    int num_bs = 0;
    int i,j,n;

    j = 0;
    for(i = 0; i < 1000; i++) {
        if(is_prime(i)) {
            coeff_bs[j++] = i;
        }
    }

    num_bs = j;

    int result = -1;
    int mn = 0;

    for(i = -999; i < 999; i++) {
        for(j = 0; j < num_bs; j++) {
            n = 0;

            while(is_prime(calc_quadratic(n, i, coeff_bs[j]))) {
                ++n;
            }

            if(n > mn) {
                mn = n;
                result = coeff_bs[j] * i;
            }
        }
        printf(".");
    }

    DUMPD(result);
                    
    return 0;
}
