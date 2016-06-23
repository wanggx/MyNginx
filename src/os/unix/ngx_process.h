
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PROCESS_H_INCLUDED_
#define _NGX_PROCESS_H_INCLUDED_


#include <ngx_setaffinity.h>
#include <ngx_setproctitle.h>


typedef pid_t       ngx_pid_t;

#define NGX_INVALID_PID  -1

typedef void (*ngx_spawn_proc_pt) (ngx_cycle_t *cycle, void *data);

typedef struct {
    ngx_pid_t           pid;        /* 进程id*/
    int                 status;     /* 进程退出状态 */

    /* 进程的channel，通过socketpair创建的，主要用来进程通信 */
    ngx_socket_t        channel[2]; 

    ngx_spawn_proc_pt   proc; /* 进程的初始化函数，在每次创建完worker时调用 */
    void               *data; /* 进程初始化传递的参数 */
    char               *name; /* 进程的名称如worker process或者master process*/

    /* 记录进程的创建方式，如果worker进程死掉，则master会再次创建一个worker进程 */
    unsigned            respawn:1;  
    unsigned            just_spawn:1;
    unsigned            detached:1;
    unsigned            exiting:1;         /* 正在退出 */
    unsigned            exited:1;          /* 已经退出，1表示已退出 */
} ngx_process_t;


typedef struct {
    char         *path;
    char         *name;
    char *const  *argv;
    char *const  *envp;
} ngx_exec_ctx_t;


#define NGX_MAX_PROCESSES         1024
/* 子进程退出时，父进程不会再次创建 */
#define NGX_PROCESS_NORESPAWN     -1   
/* master重新load的时候，会重新创建worker进程，
  * 然后将之前的worker进程都给关闭掉，JUST就是表示进程是刚刚创建的
  */
#define NGX_PROCESS_JUST_SPAWN    -2
/* 子进程异常退出时，master会重新创建它，如当worker和cache manager异常退出时，父进程会重新创建它 */
#define NGX_PROCESS_RESPAWN       -3   
#define NGX_PROCESS_JUST_RESPAWN  -4
/* 热代码替换，就是不重启nginx的情况下进行软件升级 */
#define NGX_PROCESS_DETACHED      -5


#define ngx_getpid   getpid

#ifndef ngx_log_pid
#define ngx_log_pid  ngx_pid
#endif


ngx_pid_t ngx_spawn_process(ngx_cycle_t *cycle,
    ngx_spawn_proc_pt proc, void *data, char *name, ngx_int_t respawn);
ngx_pid_t ngx_execute(ngx_cycle_t *cycle, ngx_exec_ctx_t *ctx);
ngx_int_t ngx_init_signals(ngx_log_t *log);
void ngx_debug_point(void);


#if (NGX_HAVE_SCHED_YIELD)
#define ngx_sched_yield()  sched_yield()
#else
#define ngx_sched_yield()  usleep(1)
#endif


extern int            ngx_argc;
extern char         **ngx_argv;
extern char         **ngx_os_argv;

extern ngx_pid_t      ngx_pid;
extern ngx_socket_t   ngx_channel;
extern ngx_int_t      ngx_process_slot;
extern ngx_int_t      ngx_last_process;
extern ngx_process_t  ngx_processes[NGX_MAX_PROCESSES];


#endif /* _NGX_PROCESS_H_INCLUDED_ */
