
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
    uintptr_t         slab;               /* slab���ӵ����ܹ��ж���ҳ */
    ngx_slab_page_t  *next;        /* ָ����һҳָ�� */
    uintptr_t         prev;
};

/* nginx���ڴ�slab���� */
typedef struct {
    ngx_shmtx_sh_t    lock;

    size_t            min_size;         /* ��С���䵥Ԫ */
    size_t            min_shift;       /* ��С���䵥Ԫ��Ӧλ�� */

    ngx_slab_page_t  *pages;      /* ҳ���� */
    ngx_slab_page_t  *last;
    ngx_slab_page_t   free;         /* ����ҳ����������ҳʱ�����ǽ����յ�ҳ��ӵ�free�ײ� */

    u_char           *start;             /* �ɷ���ռ����ʼ��ַ */
    u_char           *end;              /* �ڴ��Ľ�����ַ */

    ngx_shmtx_t       mutex;       /* �������õ��� */

    u_char           *log_ctx;
    u_char            zero;

    unsigned          log_nomem:1;

    void             *data;
    void             *addr;               /* slab��Ӧ�ڴ���׵�ַ */
} ngx_slab_pool_t;


void ngx_slab_init(ngx_slab_pool_t *pool);
void *ngx_slab_alloc(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_alloc_locked(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_calloc(ngx_slab_pool_t *pool, size_t size);
void *ngx_slab_calloc_locked(ngx_slab_pool_t *pool, size_t size);
void ngx_slab_free(ngx_slab_pool_t *pool, void *p);
void ngx_slab_free_locked(ngx_slab_pool_t *pool, void *p);


#endif /* _NGX_SLAB_H_INCLUDED_ */
