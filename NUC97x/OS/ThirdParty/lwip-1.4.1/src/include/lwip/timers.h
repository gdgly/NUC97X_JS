/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *         Simon Goldschmidt
 *
 */
#ifndef __LWIP_TIMERS_H__
#define __LWIP_TIMERS_H__

#include "lwip/opt.h"

/* Timers are not supported when NO_SYS==1 and NO_SYS_NO_TIMERS==1 */
#define LWIP_TIMERS (!NO_SYS || (NO_SYS && !NO_SYS_NO_TIMERS))

#if LWIP_TIMERS

#include "lwip/err.h"
#if !NO_SYS
#include "lwip/sys.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LWIP_DEBUG_TIMERNAMES
#ifdef LWIP_DEBUG
#define LWIP_DEBUG_TIMERNAMES SYS_DEBUG
#else /* LWIP_DEBUG */
#define LWIP_DEBUG_TIMERNAMES 0
#endif /* LWIP_DEBUG*/
#endif

/** Function prototype for a timeout callback function. Register such a function
 * using sys_timeout().
 *
 * @param arg Additional argument to pass to the function - set up by sys_timeout()
 */
typedef void (* sys_timeout_handler)(void *arg);

struct sys_timeo {
  struct sys_timeo *next;//指向下一个定时器
  u32_t time;//定时值（ms），在加入链表时这个值会进行调整
  sys_timeout_handler h; //定时器回调函数
  void *arg; //回调函数传入参数
#if LWIP_DEBUG_TIMERNAMES
  const char* handler_name;
#endif /* LWIP_DEBUG_TIMERNAMES */
};

void sys_timeouts_init(void);

#if LWIP_DEBUG_TIMERNAMES
void sys_timeout_debug(u32_t msecs, sys_timeout_handler handler, void *arg, const char* handler_name);
#define sys_timeout(msecs, handler, arg) sys_timeout_debug(msecs, handler, arg, #handler)
#else /* LWIP_DEBUG_TIMERNAMES */
void sys_timeout(u32_t msecs, sys_timeout_handler handler, void *arg);
#endif /* LWIP_DEBUG_TIMERNAMES */

void sys_untimeout(sys_timeout_handler handler, void *arg);
#if NO_SYS
void sys_check_timeouts(void);
void sys_restart_timeouts(void);
#else /* NO_SYS */
void sys_timeouts_mbox_fetch(sys_mbox_t *mbox, void **msg);
#endif /* NO_SYS */


#ifdef __cplusplus
}
#endif

#endif /* LWIP_TIMERS */
#endif /* __LWIP_TIMERS_H__ */
