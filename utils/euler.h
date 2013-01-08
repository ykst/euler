#ifndef __UTILS_EULER_H_
#define __UTILS_EULER_H_

#include <stdbool.h>
#include <stdint.h>

#define POW(x,i) ({typeof(x) ____pow_result = 1;for(int ____pow_i = 0; ____pow_i < (i); ____pow_i++) ____pow_result *= (x); ____pow_result;})

/* project euler specific utility */
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

static inline int rotate(int num, int digits,int rot)
{
    return num = POW(10, digits - rot)* (num % POW(10,rot)) + num / POW(10, rot);
}

static inline bool is_palindromic(uint32_t base,uint32_t x)
{
    uint32_t y = 0;
    uint32_t tmp = x;

    while(tmp > 0) {
        y *= base;
        y += tmp % base; 
        tmp /= base;
    }

    return x == y;
}

static inline int gcd(int a, int b)
{
    if(a == 0) return b;

    while(b != 0) {
        if(a > b) {
            a -= b;
        } else {
            b -= a;
        }
    }

    return a;
}

static inline int comb(int a, int b)
{
    int ta = 1;
    int tb = 1;
    int g;

    if(b > a / 2) b = a - b;

    for(int i = 0; i < b; i++) {
        ta *= a - i;
        tb *= i + 1;
        g = gcd(ta,tb);
        tb /= g;
        ta /= g;
    }

    return ta / tb;
}

int compare_int(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

int compare_int_down(const void *a, const void *b)
{
    return *(int*)b - *(int*)a;
}


#endif
