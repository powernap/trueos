/*-
 * Copyright (c) 2010 Isilon Systems, Inc.
 * Copyright (c) 2010 iX Systems, Inc.
 * Copyright (c) 2010 Panasas, Inc.
 * Copyright (c) 2013-2016 Mellanox Technologies, Ltd.
 * Copyright (c) 2015 François Tigeot
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */
#ifndef	_LINUX_COMPILER_H_
#define	_LINUX_COMPILER_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/syslog.h>

#include <asm/types.h>
#define __user
#define __kernel
#define __safe
#define __force
#define __nocast
#define __iomem
#define __chk_user_ptr(x)		0
#define __chk_io_ptr(x)			0
#define __builtin_warning(x, y...)	(1)
#define __acquires(x)
#define __releases(x)
#define __acquire(x)			0
#define __release(x)			0
#define __cond_lock(x,c)		(c)
#define	__bitwise
#define __devinitdata
#define	__deprecated
#define __init
#define	__devinit
#define	__devexit
#define __exit
#define	__rcu
#define __malloc

#define __weak		__attribute__((weak))


#define ACCESS_PRIVATE(p, member) ((p)->member)

#define	__attribute_const__		__attribute__((__const__))
#undef __always_inline
#define	__always_inline			inline
#define	____cacheline_aligned		__aligned(CACHE_LINE_SIZE)

#define	likely(x)			__builtin_expect(!!(x), 1)
#define	unlikely(x)			__builtin_expect(!!(x), 0)
#define typeof(x)			__typeof(x)

#define	uninitialized_var(x)		x = x
#define	__always_unused			__unused
#define	__maybe_unused			__unused
#define	__must_check			__result_use_check

#define	__printf(a,b)			__printflike(a,b)

#define	barrier()			__asm__ __volatile__("": : :"memory")

#define	___PASTE(a,b) a##b
#define	__PASTE(a,b) ___PASTE(a,b)


#ifndef PRINT_UNIMPLEMENTED
#define PRINT_UNIMPLEMENTED 0
#endif

#define UNIMPLEMENTED_ONCE()			\
	do {					\
		static int seen = 0;		\
									\
		if (seen == 0 && PRINT_UNIMPLEMENTED) {					\
			log(LOG_WARNING, "%s not implemented -- see your local kernel hacker\n", __FUNCTION__); \
			seen = 1;					\
		}							\
	} while (0)

#define DODGY_ONCE()			\
	do {					\
		static int seen = 0;		\
									\
		if (seen == 0 && PRINT_UNIMPLEMENTED) {					\
			log(LOG_WARNING, "%s is dodgy -- see your local kernel hacker\n", __FUNCTION__); \
			seen = 1;					\
		}							\
	} while (0)

#define UNIMPLEMENTED()	UNIMPLEMENTED_ONCE()
#define WARN_NOT() 	UNIMPLEMENTED_ONCE()
#define DODGY() DODGY_ONCE();

#define __READ_ONCE_SIZE                                                \
({                                                                      \
        switch (size) {                                                 \
        case 1: *(__u8 *)res = *(volatile __u8 *)(uintptr_t)p; break;	\
        case 2: *(__u16 *)res = *(volatile __u16 *)(uintptr_t)p; break;	\
        case 4: *(__u32 *)res = *(volatile __u32 *)(uintptr_t)p; break;	\
        case 8: *(__u64 *)res = *(volatile __u64 *)(uintptr_t)p; break;	\
        default:                                                        \
                barrier();                                              \
                __builtin_memcpy((void *)res, (const void *)(uintptr_t)p, size); \
                barrier();                                              \
        }                                                               \
})

static __always_inline
void __read_once_size(const volatile void *p, void *res, int size)
{
        __READ_ONCE_SIZE;
}

#define __READ_ONCE(x)                                           \
({                                                                      \
        union { typeof(x) __val; char __c[1]; } __u;                    \
        __read_once_size(&(x), __u.__c, sizeof(x));             \
        __u.__val;                                                      \
})
#define READ_ONCE(x) __READ_ONCE(x)

#define	ACCESS_ONCE(x)			(*(volatile __typeof(x) *)&(x))
  
#define	WRITE_ONCE(x,v) do {		\
	barrier();			\
	ACCESS_ONCE(x) = (v);		\
	barrier();			\
} while (0)

#define	lockless_dereference(p) READ_ONCE(p)

#define	_AT(T,X)	((T)(X))

#endif	/* _LINUX_COMPILER_H_ */
