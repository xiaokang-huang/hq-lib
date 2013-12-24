#ifndef _HQOS_H_
#define _HQOS_H_

// ======================================================
//	predefined include
// ======================================================
#if ( defined(_WIN32) || defined(_WINDOWS))
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
#else // (defined(__LINUX))
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
#endif

// ======================================================
//	system related data type
// ======================================================
//typedef long unsigned int	_SIZE;
#if ( defined(_WIN32) || defined(_WINDOWS))
	typedef size_t _SIZE;
#else//linux
	typedef size_t _SIZE;
#endif

// ======================================================
//	inline
// ======================================================
#define INLINE inline
// ======================================================
//	DLL API
// ======================================================
#if ( defined(_WIN32) || defined(_WINDOWS))
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
#if ( defined(_WIN32) || defined(_WINDOWS))
	#include <windows.h>
	#define THREADHANDLE 	HANDLE
	#define THREADMUTEX		HANDLE
	#define THREADSPINLOCK	HANDLE
	#define THREADSEMAPHORE	HANDLE
	#define THREADEVENT		HANDLE
#else // (defined(__LINUX))
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
#if ( defined(_WIN32) || defined(_WINDOWS))
	#define PRINT printf
	#define ASSERT assert
#else // (defined(__LINUX))
	#define PRINT printf
	#define ASSERT assert
#endif

// ======================================================
//	stdlib
// ======================================================
#if ( defined(_WIN32) || defined(_WINDOWS))
	#include <hq_stdlib.h>
#else // (defined(__LINUX))
	#include <hq_stdlib.h>
#endif

// ======================================================
//	CAS operation
// ======================================================
#if ( defined(_WIN32) || defined(_WINDOWS))
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
#if ( defined(_WIN32) || defined(_WINDOWS))
#else
	#include <GL/gl.h>
	#include <GL/glx.h>
#endif
// ======================================================
//	other
// ======================================================
#include <math.h>
#define FOURCC(a,b,c,d) ( ((UINT32)d) | ( ((UINT32)c) << 8 ) | ( ((UINT32)b) << 16 ) | ( ((UINT32)a) << 24 ) )
#endif//_HQOS_H_
