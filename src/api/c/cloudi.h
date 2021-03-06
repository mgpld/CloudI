/* -*-Mode:C;coding:utf-8;tab-width:4;c-basic-offset:4;indent-tabs-mode:()-*-
 * ex: set ft=c fenc=utf-8 sts=4 ts=4 sw=4 et nomod:
 *
 * MIT License
 *
 * Copyright (c) 2011-2017 Michael Truog <mjtruog at gmail dot com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef CLOUDI_HPP
#warning "Only need to include cloudi.h (C), not cloudi.hpp (C++)"
#endif
#ifndef CLOUDI_H
#define CLOUDI_H

#include <stdint.h>

#if defined __cplusplus
extern "C"
{
#endif

#define CLOUDI_MAX_BUFFERSIZE 2147483648U /* 2GB */

typedef struct cloudi_instance_t
{
    void * state;
    int fd_in;
    int fd_out;
    int use_header;
    int initialization_complete;
    int terminate;
    uint32_t buffer_size;
    void * lookup;
    void * buffer_send;
    void * buffer_recv;
    uint32_t buffer_recv_index;
    void * buffer_call;
    void * poll_timer;
    void * request_timer;
    uint32_t request_timeout;
    uint32_t process_index;
    uint32_t process_count;
    uint32_t process_count_max;
    uint32_t process_count_min;
    char * prefix;
    uint32_t timeout_initialize;
    uint32_t timeout_async;
    uint32_t timeout_sync;
    uint32_t timeout_terminate;
    int8_t priority_default;
    uint8_t request_timeout_adjustment;
    char * response_info;
    uint32_t response_info_size;
    char * response;
    uint32_t response_size;
    char * trans_id;          /* always 16 characters (128 bits) length */
    uint32_t trans_id_count;
    uint32_t subscribe_count;

} cloudi_instance_t;

/* request_type values */
#define CLOUDI_ASYNC     1
#define CLOUDI_SYNC     -1

typedef void (*cloudi_callback_t)(int const request_type,
                                  char const * const name,
                                  char const * const pattern,
                                  void const * const request_info,
                                  uint32_t const request_info_size,
                                  void const * const request,
                                  uint32_t const request_size,
                                  uint32_t timeout,
                                  int8_t priority,
                                  char const * const trans_id,
                                  char const * const pid,
                                  uint32_t const pid_size,
                                  void * state,
                                  cloudi_instance_t * p);

#define cloudi_get_response(p)               ((p)->response)
#define cloudi_get_response_size(p)          ((p)->response_size)
#define cloudi_get_response_info(p)          ((p)->response_info)
#define cloudi_get_response_info_size(p)     ((p)->response_info_size)
#define cloudi_get_trans_id_count(p)         ((p)->trans_id_count)
#define cloudi_get_trans_id(p, i)            (&((p)->trans_id[i * 16]))
#define cloudi_get_subscribe_count(p)        ((p)->subscribe_count)
#define cloudi_get_process_index(p)          ((p)->process_index)
#define cloudi_get_process_count(p)          ((p)->process_count)
#define cloudi_get_process_count_max(p)      ((p)->process_count_max)
#define cloudi_get_process_count_min(p)      ((p)->process_count_min)
#define cloudi_get_prefix(p)                 ((p)->prefix)
#define cloudi_get_timeout_initialize(p)     ((p)->timeout_initialize)
#define cloudi_get_timeout_async(p)          ((p)->timeout_async)
#define cloudi_get_timeout_sync(p)           ((p)->timeout_sync)
#define cloudi_get_timeout_terminate(p)      ((p)->timeout_terminate)
#define cloudi_get_priority_default(p)       ((p)->priority_default)

int cloudi_initialize(cloudi_instance_t * p,
                      unsigned int const thread_index,
                      void * state);

void * cloudi_destroy(cloudi_instance_t * p);

int cloudi_initialize_thread_count(unsigned int * const thread_count);

int cloudi_subscribe(cloudi_instance_t * p,
                     char const * const pattern,
                     cloudi_callback_t f);

int cloudi_subscribe_count(cloudi_instance_t * p,
                           char const * const pattern);

int cloudi_unsubscribe(cloudi_instance_t * p,
                       char const * const pattern);

int cloudi_send_async(cloudi_instance_t * p,
                      char const * const name,
                      void const * const request,
                      uint32_t const request_size);

int cloudi_send_async_(cloudi_instance_t * p,
                       char const * const name,
                       void const * const request_info,
                       uint32_t const request_info_size,
                       void const * const request,
                       uint32_t const request_size,
                       uint32_t timeout,
                       int8_t const priority);

int cloudi_send_sync(cloudi_instance_t * p,
                     char const * const name,
                     void const * const request,
                     uint32_t const request_size);

int cloudi_send_sync_(cloudi_instance_t * p,
                      char const * const name,
                      void const * const request_info,
                      uint32_t const request_info_size,
                      void const * const request,
                      uint32_t const request_size,
                      uint32_t timeout,
                      int8_t const priority);

int cloudi_mcast_async(cloudi_instance_t * p,
                       char const * const name,
                       void const * const request,
                       uint32_t const request_size);

int cloudi_mcast_async_(cloudi_instance_t * p,
                        char const * const name,
                        void const * const request_info,
                        uint32_t const request_info_size,
                        void const * const request,
                        uint32_t const request_size,
                        uint32_t timeout,
                        int8_t const priority);

int cloudi_forward(cloudi_instance_t * p,
                   int const request_type,
                   char const * const name,
                   void const * const request_info,
                   uint32_t const request_info_size,
                   void const * const request,
                   uint32_t const request_size,
                   uint32_t timeout,
                   int8_t const priority,
                   char const * const trans_id,
                   char const * const pid,
                   uint32_t const pid_size);

int cloudi_forward_async(cloudi_instance_t * p,
                         char const * const name,
                         void const * const request_info,
                         uint32_t const request_info_size,
                         void const * const request,
                         uint32_t const request_size,
                         uint32_t timeout,
                         int8_t const priority,
                         char const * const trans_id,
                         char const * const pid,
                         uint32_t const pid_size);

int cloudi_forward_sync(cloudi_instance_t * p,
                        char const * const name,
                        void const * const request_info,
                        uint32_t const request_info_size,
                        void const * const request,
                        uint32_t const request_size,
                        uint32_t timeout,
                        int8_t const priority,
                        char const * const trans_id,
                        char const * const pid,
                        uint32_t const pid_size);

int cloudi_return(cloudi_instance_t * p,
                  int const request_type,
                  char const * const name,
                  char const * const pattern,
                  void const * const response_info,
                  uint32_t const response_info_size,
                  void const * const response,
                  uint32_t const response_size,
                  uint32_t timeout,
                  char const * const trans_id,
                  char const * const pid,
                  uint32_t const pid_size);

int cloudi_return_async(cloudi_instance_t * p,
                        char const * const name,
                        char const * const pattern,
                        void const * const response_info,
                        uint32_t const response_info_size,
                        void const * const response,
                        uint32_t const response_size,
                        uint32_t timeout,
                        char const * const trans_id,
                        char const * const pid,
                        uint32_t const pid_size);

int cloudi_return_sync(cloudi_instance_t * p,
                       char const * const name,
                       char const * const pattern,
                       void const * const response_info,
                       uint32_t const response_info_size,
                       void const * const response,
                       uint32_t const response_size,
                       uint32_t timeout,
                       char const * const trans_id,
                       char const * const pid,
                       uint32_t const pid_size);

int cloudi_recv_async(cloudi_instance_t * p,
                      uint32_t timeout,
                      char const * const trans_id,
                      int consume);

int cloudi_poll(cloudi_instance_t * p,
                int timeout);

char const ** cloudi_info_key_value_parse(void const * const request_info,
                                          uint32_t const request_info_size);
void cloudi_info_key_value_destroy(char const ** p);

enum
{
    cloudi_success                             =   0,
    // programs can use exit status values [1..6] without conflicts
    // with internal cloudi error conditions

    // API specific errors
    cloudi_terminate                           = 110, // cloudi_error_poll_HUP
    cloudi_timeout                             =   7,
    cloudi_error_function_parameter            =   8,
    cloudi_error_read_underflow                =   9,
    cloudi_error_ei_decode                     =  10,
    // reuse some exit status values from os_spawn
    cloudi_invalid_input                       =  11,
    cloudi_out_of_memory                       =  12,
    // reuse some exit status values from GEPD
    cloudi_error_read_EAGAIN                   =  81,
    cloudi_error_read_EBADF                    =  82,
    cloudi_error_read_EFAULT                   =  83,
    cloudi_error_read_EINTR                    =  84,
    cloudi_error_read_EINVAL                   =  85,
    cloudi_error_read_EIO                      =  86,
    cloudi_error_read_EISDIR                   =  87,
    cloudi_error_read_null                     =  88,
    cloudi_error_read_overflow                 =  89,
    cloudi_error_read_unknown                  =  90,
    cloudi_error_write_EAGAIN                  =  91,
    cloudi_error_write_EBADF                   =  92,
    cloudi_error_write_EFAULT                  =  93,
    cloudi_error_write_EFBIG                   =  94,
    cloudi_error_write_EINTR                   =  95,
    cloudi_error_write_EINVAL                  =  96,
    cloudi_error_write_EIO                     =  97,
    cloudi_error_write_ENOSPC                  =  98,
    cloudi_error_write_EPIPE                   =  99,
    cloudi_error_write_null                    = 100,
    cloudi_error_write_overflow                = 101,
    cloudi_error_write_unknown                 = 102,
    cloudi_error_ei_encode                     = 103,
    cloudi_error_poll_EBADF                    = 104,
    cloudi_error_poll_EFAULT                   = 105,
    cloudi_error_poll_EINTR                    = 106,
    cloudi_error_poll_EINVAL                   = 107,
    cloudi_error_poll_ENOMEM                   = 108,
    cloudi_error_poll_ERR                      = 109,
    cloudi_error_poll_HUP                      = 110,
    cloudi_error_poll_NVAL                     = 111,
    cloudi_error_poll_unknown                  = 112
};

#if defined __cplusplus
}
#endif

#endif /* CLOUDI_H */

