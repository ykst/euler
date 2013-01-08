#ifndef _COMMON_LIST_H_
#define _COMMON_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "common.h"

struct list_t {
    struct list_t *next;
};

struct list_head_t {
    struct list_t *list;
    struct list_t *tail;
    int (*cleaner)(struct list_t *);
};

static inline void list_destroy(struct list_head_t *head);
static inline int list_add(struct list_head_t *head, struct list_t *elem);
static inline int list_push(struct list_head_t *head, struct list_t *elem);
static inline struct list_t *list_pop(struct list_head_t *head);
static inline int list_del(struct list_head_t *head,struct list_t *item);
static inline struct list_t *list_find_pop(struct list_head_t *head,struct list_t *item);
static inline int list_map(struct list_head_t *head,int (*func)(struct list_t *,void *),void *param);
static inline struct list_t *list_select(struct list_head_t *head, int (*cond)(struct list_t *,void *), void *state);
static inline int list_length(struct list_head_t *head);
typedef int (*list_map_f)(struct list_t *, void *);

/* obtain a base pointer from an embedded field pointer */
#ifndef container_of
#define container_of(type,ptr,member) ({ const typeof( ((type *)0)->member) *__mptr=(ptr); (type *) ( (char *)__mptr - offsetof(type,member));})
#endif

/* casual static type cast using container_of macro */
#define list_upcast(p,l) ({ p = container_of(typeof(*p),l,list_entry);})

/* O(n):pseudo-syntax for list iteration */
#define list_closure(head,var)  \
    for((head)->list ? list_upcast(var,((head)->list)) : ((var) = NULL ); \
	(var); \
	(var)->list_entry.next ? list_upcast(var,((var)->list_entry.next)) : ((var) = NULL ))

/* O(n):query length of the list */
static inline int list_length(struct list_head_t *head)
{
    DASSERT(head,HDIE_FAILURE_D);

    int cnt = 0;
    struct list_t *e;

    for(e = head->list; e; e = e->next) {
        ++cnt;
    }

    return cnt;
}

/* O(n):perform callback function 'func' with parameter 'param' iteratively. */
static inline int list_map(struct list_head_t *head, list_map_f func, void *param)
{
    struct list_t *p;
    p = head->list;
    while(p) {
        ASSERT(func(p,param)==SUCCESS,HDIE_FAILURE_D);
        p = p->next;
    }
    return SUCCESS;
}

/* O(n):apply list_del until destroy all elems.
   we assume all the deletion must succeed as long as an elem was given :p */
static inline void list_destroy(struct list_head_t *head)
{
    while(list_del(head, head->list)==SUCCESS);
} 

/* O(n):remove a given elememt from a list. 
 * if deallocator is set, then call it. otherwise do nothing */ 
static inline int list_del(struct list_head_t *head,struct list_t *item)
{
    DASSERT(head,HDIE_FAILURE_D);

    if(item == NULL) return FAILURE;

    struct list_t *list = head->list;
    struct list_t *prev= NULL;

    while(list!=NULL) {

        if(item == list){
            if(prev==NULL) {
                head->list = item->next;
            } else {
                prev->next = item->next;
            }
            
            if(item == head->tail) {
                head->tail = prev;
            }

            if(head->cleaner != NULL) {
                ASSERT(head->cleaner(item)==SUCCESS,HDIE_FAILURE_D);
            }

            
            return SUCCESS;
        }

        prev = list;
        list = list->next;
    }

    HDIE_FAILURE_D;
}

/* list_del without clean function */
static inline struct list_t *list_find_pop(struct list_head_t *head,struct list_t *item)
{
    DASSERT(head,HDIE_NULL_D);

    if(item == NULL) return NULL;

    struct list_t *list = head->list;
    struct list_t *prev= NULL;

    while(list!=NULL) {

        if(item == list){
            if(prev==NULL) {
                head->list = item->next;
            } else {
                prev->next = item->next;
            }
            
            if(item == head->tail) {
                head->tail = prev;
            }

            return item;
        }

        prev = list;
        list = list->next;
    }

    HDIE_NULL_D;
}

/* O(1):add elem to list (FIFO). we do not alloc any memory here */
static inline int list_add(struct list_head_t *head, struct list_t *elem)
{
    DASSERT(head,HDIE_FAILURE_D);
    DASSERT(elem,HDIE_FAILURE_D);
    
    if(!head->list) {
        head->list = elem;
        head->tail = elem;
    } else {
        DASSERT(head->tail, HDIE_FAILURE_D);
        head->tail->next = elem;
        head->tail = elem;
    }

    /* this is critical: if not set to null, endless loop will occur in mapping functions when list elements are rearranged */
    elem->next = NULL;

    return SUCCESS;
}

/* O(1):add elem to list (LIFO). we do not alloc any memory here */
static inline int list_push(struct list_head_t *head, struct list_t *elem)
{
    DASSERT(head,HDIE_FAILURE_D);
    DASSERT(elem,HDIE_FAILURE_D);
    
    struct list_t *old_list = head->list;
    head->list = elem;
    elem->next = old_list;

    if(!head->tail) {
        head->tail = elem;
    }

    return SUCCESS;
}

/* O(1):pop elem from list (LIFO). NOTE we simply detach an element, no clean, no free occurs*/
static inline struct list_t *list_pop(struct list_head_t *head)
{
    DASSERT(head,HDIE_NULL_D);
    struct list_t *ret = NULL;
    
    ret = head->list;
    if(ret) {
        head->list = ret->next;
        ret->next = NULL;
        if(ret == head->tail) {
            head->tail = NULL;
        }
    }

    return ret;
}



/* O(n):apply 'cond' callback to elems iteratively: 
 * if 'cond' returns non-zero first time, then return the elem as "selected" */
static inline struct list_t *list_select(struct list_head_t *head, list_map_f cond, void *state)
{
    struct list_t *p;

    DASSERT(head, HDIE_NULL_D);

    p = head->list;

    while(p) {
        if(cond(p,state)) {
            return  p;
        }
        p = p->next;
    }

    return NULL;
}

/* O(n): reverse list */
static inline int list_reverse(struct list_head_t *head)
{
    DASSERT(head,HDIE_FAILURE_D);

    struct list_t *prev = NULL;
    struct list_t *next;
    struct list_t *e;

    next = head->list;
    head->tail = head->list;

    while(next) {
        e = next;
        next = e->next;
        e->next = prev;
        prev = e;
    }

    head->list = prev;


    return SUCCESS;
}

#ifdef __cplusplus
}
#endif
#endif
