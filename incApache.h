/*
 * incApache.h: definizioni per il web server
 *
 * Programma sviluppato a supporto del laboratorio di
 * Sistemi di Elaborazione e Trasmissione del corso di laurea
 * in Informatica classe L-31 presso l'Università degli Studi di
 * Genova per l'anno accademico 2024/2025.
 *
 * Copyright (C) 2012-2014 by Giovanni Chiola <chiolag@acm.org>
 * Copyright (C) 2015-2016 by Giovanni Lagorio <giovanni.lagorio@unige.it>
 * Copyright (C) 2016-2024  by Giovanni Chiola <chiolag@acm.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef incApache_h
#define incApache_h

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif

#define _XOPEN_SOURCE 500      /* glibc2 needs this */
#define _BSD_SOURCE            /* glibc2 needs this */
#define _XOPEN_SOURCE_EXTENDED /* glibc2 needs this */

#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_CONNECTIONS 4
#define MAX_COOKIES 256

#ifdef INCaPACHE_8_1

#define FREE_SLOT (-1)
#define RESERVED_SLOT (-2)

#define MAX_THREADS                                                            \
    (4 * MAX_CONNECTIONS) /*** this should be > 2*MAXCONNECTION ***/
#define SEND_RESPONSE send_resp_thread
struct response_params {
        int code;
        int cookie;
        int is_http1_0;
        char *filename;
        struct stat *p_stat;
};

extern int client_sockets[];
extern pthread_t *to_join[];
extern pthread_mutex_t threads_mutex;
extern int no_response_threads[];
extern int no_free_threads;
extern struct response_params thread_params[];
extern int find_unused_thread_idx(int conn_no);
extern void join_all_threads(int conn_no);
extern void join_prev_thread(int thrd_no);
extern void send_resp_thread(int out_socket, int response_code, int cookie,
                             int is_http1_0, int connection_idx,
                             int new_thread_idx, char *filename,
                             struct stat *stat_p);

#else /* #ifdef INCaPACHE_8_1 */

#define SEND_RESPONSE send_response

#endif /* #ifdef INCaPACHE_8_1 */

#define MAX_TIME_STR 40
#define MAX_HEADER_SIZE 10000
#define BACKLOG 10

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1

extern int listen_fd;
extern FILE *mime_request_stream, *mime_reply_stream;
extern pthread_t thread_ids[];
extern int connection_no[];
extern pthread_mutex_t accept_mutex;
extern pthread_mutex_t mime_mutex;

extern void *my_malloc(size_t size);
extern char *my_strdup(const char *const s);
extern time_t my_timegm(struct tm *tm);
extern void fail(const char *const msg);
extern void fail_errno(const char *const msg);
extern ssize_t send_all(int fd, const char *ptr, size_t n, int flags);
extern void *client_connection_thread(void *vp);
extern char *get_mime_type(char *filename);

extern void send_response(int client_fd, int response_code, int cookie,
#ifdef INCaPACHE_8_1
                          int is_http1_0, int thread_no,
#endif
                          char *filename, struct stat *stat_p);
extern void manage_http_requests(int client_fd
#ifdef INCaPACHE_8_1
                                 ,
                                 int connection_no
#endif
);

#endif /* #ifndef incApache_h */
