
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;


/* 一个长的链表会被分成很多部分，每个部分都存放nalloc个元素 
 * 每一个部分里面内存是连续的 
 */
struct ngx_list_part_s {
    void             *elts;  /* 每个部分数据的内存地址 */
    ngx_uint_t        nelts; /* 每个部分已用的元素个数超过nalloc则表示该部分已被用完 */
    ngx_list_part_t  *next;
};

/* 链表的头部 */
typedef struct {
    ngx_list_part_t  *last;   /* 指向最后一个部分 */
    ngx_list_part_t   part;   /* 第一个部分 */
    size_t            size;   /* 链表中元素大小 */
    ngx_uint_t        nalloc; /* 给链表分配元素的大小 */
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
