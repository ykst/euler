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
 * もうpandigitalはええて。イタレートするものを作って、大きい数から素数チェックでおk
 */

DECLARE_VECTOR(int);

static uint32_t check_catproduct(int result[9], int range)
{
    uint32_t x = 0;
    int i;

    for(i = 0; i < range + 1; i++) {
        x *= 10;
        x += result[i];
    }

    return (is_prime(x) ? x : 0);
}

static void iter_pandigital(int used[9], int result[9], int range, int pivot, uint32_t *p_result)
{
    if(pivot == range+1) {
        *p_result = check_catproduct(result, range);
        return;
    }

    int i;

    for(i = range; i >= 0; i--) {
        if(!used[i]) {
            result[pivot] = i + 1;
            used[i] = 1;
            iter_pandigital(used, result, range, pivot + 1, p_result);
            used[i] = 0;
            if(*p_result) return;
        }
    }
}


int main(int argc, char **argv)
{
    int n = 0;
    int x = 0;

    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'x':
                x = atoi(optarg);
                break;
        }
    }

    uint32_t result = 1;

    vector(int) *vstep = vector_new(int, 100);

    int used[9] = {};
    int digits[9] = {};

    for(int i = 8; i > 0; i--) {
        result = 0;
        iter_pandigital(used, digits, i, 0, &result);
        if(result) break;
    }

    DUMPD(result); 

    vector_delete(vstep);

    return 0;
}
