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

/* Note:
 * 単純にメモリをつなげていけば簡単なはずだが、
 * オフセット計算やってがっつりバグる
 */

DECLARE_VECTOR(int);


int main(int argc, char **argv)
{
    int t = 0;

    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
            case 'n':
                t = atoi(optarg);
                break;
        }
    }

    uint32_t result = 1;

    vector(int) *vstep = vector_new(int, 100);

    int i = 0;
    int pivot = 1; 
    int base = 10;

    while(i < 1000000) {
        i += pivot * (base - base / 10); 
        ++pivot;
        base *= 10;
        vector_pushback(vstep, i);
    }
    
    for(base = 1; base <= 1000000; base *= 10) {
        if(t > 0) base = t;
        i = 1;
        int last = 0;

        vector_closure(vstep, pivot) {
            if(base <= pivot) break;
            i++;
            last = pivot;
        }
        int pbase;

        pbase = POW(10, i - 1) - 1;
        int ofs = (base - last);
        ofs = (ofs % i == 0) ? ofs / i : ofs / i+ 1;
        int num = pbase + ofs;
        int dn;
        int dofs = (i - (base - last) % i) % i;
        DUMPD(dofs);
        dn = num / POW(10, dofs);
        dn %= 10;
        DUMPD(base);
        DUMPD(dn);
        result *= dn;
        if(t > 0) break;
    }

    DUMPD(result); 

    vector_delete(vstep);

    return 0;
}
