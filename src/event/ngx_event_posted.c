
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>


ngx_queue_t  ngx_posted_accept_events;  /* accept事件队列 */
ngx_queue_t  ngx_posted_events;         /* 普通事件队列 */


/* 处理post队列中的事件 */
void
ngx_event_process_posted(ngx_cycle_t *cycle, ngx_queue_t *posted)
{
    ngx_queue_t  *q;
    ngx_event_t  *ev;

    /* 判断队列是否为NULL */
    while (!ngx_queue_empty(posted)) {

        q = ngx_queue_head(posted);

        /* 取出队列中事件 */
        ev = ngx_queue_data(q, ngx_event_t, queue);

        ngx_log_debug1(NGX_LOG_DEBUG_EVENT, cycle->log, 0,
                      "posted event %p", ev);

        /* 将事件从队列中删除 */
        ngx_delete_posted_event(ev);

        /* 处理事件 */
        ev->handler(ev);
    }
}
