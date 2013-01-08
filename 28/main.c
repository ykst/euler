#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note 
 * ほとんど解くだけだが、多項式をきちんと計算しないでsumに頼ってると将来は暗い
 */

static inline int accum_corner(int step, int base, int *p_accum)
{

    if(base == 0) {
        *p_accum += 1;
        return 1;
    }

    int diff = step * 4 - 4 + base;

    *p_accum += 4 * base + 10 * (step - 1);

    return diff;
}

int main(int argc, char **argv)
{
    int n = 1001;
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
            case 'n':
                n = atoi(optarg);
                break;
        }
    }

    int i;
    int base = 0;
    int result = 0;

    for(i = 1; i <= n; i += 2) {
        base = accum_corner(i, base, &result);
    }

    DUMPD(result);
                    
    return 0;
}
