#ifndef __UTILS_VECTOR_H_
#define __UTILS_VECTOR_H_

#define DECLARE_VECTOR(t) struct vector_##t {t *buf; size_t len; size_t idx;}

#define vector(t) struct vector_##t

#define vector_new(t, s) NEW(vector(t),.buf = calloc(s,sizeof(t)), .len = (s), .idx = 0)

#define vector_delete(p) ({if(p) {FREE((p)->buf); FREE(p)} })

#define vector_multiply(p) ({typeof(*(p)->buf) ___vector_multiply_r = 1; \
        for( int ___vector_multiply_i = 0; ___vector_multiply_i < (p)->idx; ___vector_multiply_i++) { \
        ___vector_multiply_r *= (p)->buf[___vector_multiply_i]; \
        } \
        ___vector_multiply_r; })

#define vector_closure(vec, item) for( int ___vector_closure_i = 0, (item) = (vec)->buf[0]; ___vector_closure_i < (vec)->idx; (item) = (vec)->buf[++___vector_closure_i]) 

#define vector_sum(p) ({typeof(*(p)->buf) ___vector_sum_r = 0; \
        for( int ___vector_sum_i = 0; ___vector_sum_i < (p)->idx; ___vector_sum_i++) { \
        ___vector_sum_r += (p)->buf[___vector_sum_i]; \
        } \
        ___vector_sum_r; })

#define vector_pushback(p, e) ({\
    if((p)->idx >= (p)->len) {\
        p->len *= 2; \
        REALLOC((p)->buf, (p)->len, ERR("realloc failed\n");exit(1)); \
        for(int _____i = (p)->idx; _____i < (p)->len; _____i++) {\
            (p)->buf[_____i] = 0; \
        } \
    } \
    (p)->buf[(p)->idx++] = (e); })

#endif
