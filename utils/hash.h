#ifndef _COMMON_HASH_H
#define _COMMON_HASH_H

#ifdef __cplusplus
extern "C" {
#endif
#include "common.h"
#include "list.h"

/******************************************************************************
 *              DATA STRUCTURES
 ******************************************************************************/
typedef uintptr_t hash_key_t;

struct __hash_table {
    struct list_head_t *pool;
    size_t buf_size;
    hash_key_t size;
    hash_key_t num_items;
};

struct __hash_entry {
    struct list_t list_entry;
    hash_key_t key;
    void *value;
};

/******************************************************************************
 *              FUNCTION DECLARATIONS
 ******************************************************************************/
typedef struct __hash_table *hash_handle;
static inline hash_handle hash_create(int size, size_t buf_size);
static inline void hash_destroy(hash_handle h);
static inline int hash_exist(hash_handle h, hash_key_t key);
static inline int hash_add(hash_handle h, hash_key_t key, void *val);
static inline int hash_del(hash_handle h, hash_key_t key);
static inline void *hash_lookup(hash_handle h, hash_key_t key);
static inline hash_key_t __hash_buf(int size, hash_key_t key, size_t buf_size);

/******************************************************************************
 *              INLINE FUNCTIONS
 ******************************************************************************/
static inline int __clean_hash_entry(struct list_t *e)
{
    struct __hash_entry *p;
    list_upcast(p,e);

    FREE(p);
    return SUCCESS;
}

static inline struct __hash_entry *__new_hash_entry(hash_key_t key, void *value)
{
    struct __hash_entry *nh;
    TALLOC(nh,return NULL);
    nh->key = key;
    nh->value = value;
    return nh;
}

static inline void hash_destroy(hash_handle h)
{
    int i;
    if(h){
        if(h->pool){
            for(i = 0;i < h->size;i++) {
                list_destroy(&(h->pool[i]));
            }
            FREE(h->pool);
        }

        FREE(h);
    }
}

static inline hash_handle hash_create(int size, size_t buf_size)
{
    hash_handle nh = NULL;

    DASSERT(size > 0, return NULL);

    TALLOC(nh,goto error);

    TALLOCS(nh->pool,size, goto error);

    int i;
    for(i = 0; i < size; i++) {
        nh->pool[i].cleaner = &__clean_hash_entry;
    }

    nh->size = size;
    nh->buf_size = buf_size;
    return nh;
error:
    hash_destroy(nh);
    return NULL;
}


static inline struct __hash_entry *__find_entry(hash_handle h, int pos, hash_key_t key)   
{
    struct __hash_entry *p = NULL;

    list_closure(&(h->pool[pos]), p) {
        if (p->key == key) break;
    }

    return p;
}

static inline int hash_exist(hash_handle h, hash_key_t key)
{
    int pos =  __hash_buf(h->size,key,h->buf_size);
    return __find_entry(h,pos,key) != NULL;
}

static inline void *hash_lookup(hash_handle h, hash_key_t key)
{
    int pos =  __hash_buf(h->size,key,h->buf_size);
    struct __hash_entry *p;
    return (p = __find_entry(h,pos,key))? p->value : NULL;
}
    
static inline int hash_add(hash_handle h, hash_key_t key, void *val)
{
    struct __hash_entry *p;
    int pos =  __hash_buf(h->size,key,h->buf_size);

    ASSERT(p = __new_hash_entry(key,val), 
        return FAILURE);
    
    list_add(&(h->pool[pos]), &p->list_entry);

    h->num_items += 1;
    return SUCCESS;
}

static inline int hash_del(hash_handle h, hash_key_t key)
{
    struct __hash_entry *p;
    int pos =  __hash_buf(h->size,key,h->buf_size);

    MUST((p = __find_entry(h,pos,key)) != NULL,
        return FAILURE);

    MUST(list_del(&(h->pool[pos]), &p->list_entry) == SUCCESS,
        return FAILURE);

    h->num_items -= 1;

    return SUCCESS;
}

static inline hash_key_t __hash_buf(int size, hash_key_t key, size_t buf_size)
{
    hash_key_t v=(key / buf_size) % size;
    return  v;
}

#ifdef __cplusplus
}
#endif
#endif
