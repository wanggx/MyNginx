
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SLAB_H_INCLUDED_
#define _NGX_SLAB_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_slab_page_s  ngx_slab_page_t;

struct ngx_slab_page_s {
    uintptr_t         slab;               /* slab池子当中总共有多少页 */
    ngx_slab_page_t  *next;        /* 指向下一页指针 */
    uintptr_t         prev;
};

/* nginx的内存slab机制 */
typedef struct {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;         /* 最小分配单元 */
    size_t            min_shift;       /* 最小分配单元对应位移 */

    ngx_slab_page_t  *pages;      /* 页数组 */
    ngx_slab_page_t  *last;
    ngx_slab_page_t   free;         /* 空闲页链表，当回收页时，总是将回收的页添加到free首部 */

    u_char           *start;             /* 可分配空间的起始地址 */
    u_char           *end;              /* 内存块的结束地址 */

    ngx_shmtx_t       mutex;       /* 保护作用的锁 */

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    void             *data;
    void             *addr;               /* slab对应内存的首地址 */
} ngx_slab_pool_t;


void ngx_slab_init(ngx_slab_pool_t *pool);
void *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
void ngx_slab_free(ngx_slab_pool_t *pool, void *p);
void ngx_slab_free_locked(ngx_slab_pool_t *pool, void *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
