#ifndef _UTILS_BIGNUM_H_
#define _UTILS_BIGNUM_H_

#include <stdint.h>

#include <common.h>

typedef struct bignum_uint {
    uint32_t *buf; /* 0:smallest */
    uint32_t dim;
    uint32_t area;
} bignum_uint_t;

#define bignum_delete(h) ({if(h) { FREE((h)->buf); FREE(h);} })
#define BIGNUM_EXPAND(h, dim,action) while((h)->area < (dim)) { \
    (h)->area *= 2; \
    ASSERT((h)->buf = realloc((h)->buf, (h)->area * sizeof(uint32_t)), action); }


static inline int bignum_uint_reset(struct bignum_uint *h, uint32_t initial)
{
    REALLOC(h->buf, 2, return FAILURE);
    h->area = 2;
    h->dim = 1;
    h->buf[0] = initial;

    return SUCCESS;
}

static inline struct bignum_uint *bignum_uint_create(uint32_t initial)
{
    struct bignum_uint *h = NULL;

    TALLOC(h, return NULL);
    TALLOCS(h->buf, 2, goto error);
    h->dim = 1;
    h->area = 2;
    h->buf[0] = initial;
    return h;
error:
    bignum_delete(h);
    return NULL;
}

static inline int bignum_uint_assign(struct bignum_uint *src, struct bignum_uint *dst)
{
    ASSERT(src && dst, return FAILURE);

    BIGNUM_EXPAND(dst, src->dim, return FAILURE);
    
    int i;

    for(i = 0; i < src->dim; i++) {
        dst->buf[i] = src->buf[i];
    }
    
    for(; i < dst->dim; i++) {
        dst->buf[i] = 0;
    }

    return SUCCESS;
}

static inline struct bignum_uint *bignum_uint_clone(struct bignum_uint *p)
{
    struct bignum_uint *h = NULL;

    ASSERT(h = bignum_uint_create(0), return NULL);

    ASSERT(bignum_uint_assign(p,h) == SUCCESS, goto error);

    return h;
error:
    bignum_delete(h);
    return NULL;
}


static inline int bignum_uint_add(
        struct bignum_uint *lhs, 
        struct bignum_uint *rhs,
        struct bignum_uint *dst)
{
    uint32_t i;

    uint32_t dim = max(lhs->dim, rhs->dim);

    BIGNUM_EXPAND(lhs, dim + 1, return FAILURE);
    BIGNUM_EXPAND(rhs, dim + 1, return FAILURE);
    BIGNUM_EXPAND(dst, dim + 1, return FAILURE);

    uint32_t tmp, l, r, carry = 0;
    for(i = 0; i < dim; i++) {
        l = lhs->buf[i];
        r = rhs->buf[i];

        tmp = l + r + carry;

        if(tmp >= 1000000000) {
            tmp -= 1000000000;
            carry = 1;
        } else {
            carry = 0;
        }

        dst->buf[i] = tmp;
    }

    dst->buf[i] = carry;
    dst->dim = dim + carry;

    return SUCCESS;
}

/*
static inline int bignum_uint_mul_base(
        struct bignum_uint *lhs, 
        uint32_t           mul,
        struct bignum_uint *dst)
{
    uint32_t i;
    int ret = FAILURE;

    struct bignum_uint *tmp_org = NULL;

    ASSERT(tmp_org = bignum_uint_clone(lhs), return FAILURE);
    
    bignum_uint_reset(dst, 0);

    struct bignum_uint *accum = dst;
    struct bignum_uint *tmp = tmp_org;

    for(i = 1; i < mul; i++) {
        ASSERT(bignum_uint_add(lhs, tmp, accum) == SUCCESS, goto error);
        accum = swap(accum, tmp);
    }

    if(dst != accum) ASSERT(bignum_uint_assign(accum, dst) == SUCCESS, goto error);
    ret = SUCCESS;
error:
    bignum_delete(tmp_org);
    return ret;

}
*/


static inline void bignum_uint_dump(
        struct bignum_uint *h)
{
    int i;

    for(i = h->dim - 1; i >= 0; i--) {
        printf("%09u", h->buf[i]);
    }
    printf(" :%d\n", h->dim);
}

static inline void bignum_uint_digits(
        struct bignum_uint *h, uint32_t *p_digits)
{
    uint32_t tmp;
    uint32_t digits = 0;

    for(tmp = h->buf[h->dim - 1]; tmp > 0; tmp /= 10) ++digits;
    digits += (h->dim - 1) * 9; 
    *p_digits = digits;
}
#endif
