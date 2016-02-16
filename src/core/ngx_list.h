
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;


/* һ����������ᱻ�ֳɺܶಿ�֣�ÿ�����ֶ����nalloc��Ԫ�� 
 * ÿһ�����������ڴ��������� 
 */
struct ngx_list_part_s {
    void             *elts;  /* ÿ���������ݵ��ڴ��ַ */
    ngx_uint_t        nelts; /* ÿ���������õ�Ԫ�ظ�������nalloc���ʾ�ò����ѱ����� */
    ngx_list_part_t  *next;
};

/* �����ͷ�� */
typedef struct {
    ngx_list_part_t  *last;   /* ָ�����һ������ */
    ngx_list_part_t   part;   /* ��һ������ */
    size_t            size;   /* ������Ԫ�ش�С */
    ngx_uint_t        nalloc; /* ���������Ԫ�صĴ�С */
    ngx_pool_t       *pool;
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    list->part.nelts = 0;
    list->part.next = NULL;
    list->last = &list->part;
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
