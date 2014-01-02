#ifndef _HQOS_H_
#define _HQOS_H_

#include <hq_config.h>

// ======================================================
//	predefined include
// ======================================================
#if defined(CONFIG_WINDOWS)
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
#elif defined(CONFIG_LINUX)
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
#endif

// ======================================================
//	system related data type
// ======================================================
//typedef long unsigned int	_SIZE;
#if defined(CONFIG_WINDOWS)
	typedef size_t _SIZE;
#elif defined(CONFIG_LINUX)
	typedef size_t _SIZE;
#endif

// ======================================================
//	inline
// ======================================================
#define INLINE inline
// ======================================================
//	DLL API
// ======================================================
#if defined(CONFIG_WINDOWS)
#define SHAREDAPI __deci
#endif
// ======================================================
//	Access control
// ======================================================
#if ( defined(_ANDORID))
	#define PUBLIC	public
	#define PRIVATE	public
	#define PROTECTED	public
#else
	#define PUBLIC	public
	#define PRIVATE	private
	#define PROTECTED	protected
#endif
// ======================================================
//	Thread
// ======================================================
#if defined(CONFIG_WINDOWS)
	#include <windows.h>
	#define THREADHANDLE 	HANDLE
	#define THREADMUTEX		HANDLE
	#define THREADSPINLOCK	HANDLE
	#define THREADSEMAPHORE	HANDLE
	#define THREADEVENT		HANDLE
#elif defined(CONFIG_LINUX)
	#include <pthread.h>
	#include <semaphore.h>
	#define THREADHANDLE 	pthread_t
	#define THREADMUTEX		pthread_mutex_t
	#define THREADSPINLOCK	pthread_spinlock_t
	#define THREADSEMAPHORE	sem_t
	#define THREADEVENT		pthread_cond_t
#endif
// ======================================================
//	SystemInfo
// ======================================================
UINT32	GetProcessNum ();
// ======================================================
//	print and assert
// ======================================================
#if defined(CONFIG_WINDOWS)
	#define PRINT printf
	#define ASSERT assert
#elif defined(CONFIG_LINUX)
	#define PRINT printf
	#define ASSERT assert
#endif

// ======================================================
//	stdlib
// ======================================================
#if defined(CONFIG_WINDOWS)
	#include <hq_stdlib.h>
#elif defined(CONFIG_LINUX)
	#include <hq_stdlib.h>
#endif

// ======================================================
//	CAS operation
// ======================================================
#if defined(CONFIG_WINDOWS)
	#define COMPAREANDSWAP(dest, oldvalue, newvalue) InterlockedCompareExchange(dest, newvalue, oldvalue)
#else
	#define COMPAREANDSWAP(dest, oldvalue, newvalue) __sync_val_compare_and_swap(dest, oldvalue, newvalue)
#endif

// ======================================================
//	some constant and types
// ======================================================
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif
#ifndef NULL
#define NULL	0
#endif

// ======================================================
//	opengl
// ======================================================
#if defined(CONFIG_WINDOWS)
#elif defined(CONFIG_LINUX)
	#include <GL/gl.h>
	#include <GL/glx.h>
#endif
// ======================================================
//	other
// ======================================================
#include <math.h>

#if defined(CONFIG_LITTLE_ENDING)
	#define FOURCC(a,b,c,d) ( ((UINT32)a) | ( ((UINT32)b) << 8 ) | ( ((UINT32)c) << 16 ) | ( ((UINT32)d) << 24 ) )
#endif
#if defined(CONFIG_BIG_ENDING)
	#define FOURCC(a,b,c,d) ( ((UINT32)d) | ( ((UINT32)c) << 8 ) | ( ((UINT32)b) << 16 ) | ( ((UINT32)a) << 24 ) )
#endif

#endif//_HQOS_H_
