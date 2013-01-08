#ifndef __UTILS_BTABLE_H_
#define __UTILS_BTABLE_H_

#include <stdint.h>

/* 32bit table utility */
#define BTABLE_SET(a,i) ((a)[(i)/32] |= (1 << ((i) % 32)))
#define BTABLE_GET(a,i) ((a)[(i)/32] & (1 << ((i) % 32)))
#define BTABLE_CLR(a,i) ((a)[(i)/32] &= ~(1 << ((i) % 32)))

static inline void btable_set(uint32_t *a, size_t i)
{
    a[i/32] |= (1 << (i % 32));
}

static inline uint32_t btable_get(uint32_t *a, size_t i)
{
    return a[i/32] & (1 << (i % 32));
}

static inline void btable_clr(uint32_t *a, size_t i)
{
    a[i/32] &= ~(1 << (i % 32));
}

#endif
