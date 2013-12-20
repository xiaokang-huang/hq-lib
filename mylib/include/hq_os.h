#ifndef _HQOS_H_
#define _HQOS_H_

// ======================================================
//	preinclude header
// ======================================================


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
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
	#define PRINT printf
	#define ASSERT assert
#else // (defined(__LINUX))
	#include <stdio.h>
	#include <stdlib.h>
	#include <assert.h>
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

const int MultiplyDeBruijnBitPosition32[32] = {
  0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9
};
const int MultiplyDeBruijnBitPosition64[64] = {
	0, 1, 48,2, 57,49,28,3, 61,58,50,42,38,29,17,4,
	62,55,59,36,53,51,43,22,45,39,33,30,24,18,12,5,
	63,47,56,27,60,41,37,16,54,35,52,21,44,32,23,11,
	46,26,40,15,34,20,31,10,25,14,19,9, 13,8, 7, 6
};
#define hq_is2pow(x)		(((x)&(-x)) == (x))
#define hq_isodd(x)			((x)&0x1)
#define hq_iseven(x)		(!((x)&0x1))
#define hq_fastlog2_32(x)	(MultiplyDeBruijnBitPosition32[((unsigned long)(((x) & (-x)) * 0x077CB531U)) >> 27])
#define hq_fastlog2_64(x)	(MultiplyDeBruijnBitPosition64[((unsigned long)(((x) & (-x)) * 0x3f79d71b4cb0a89UL)) >> 27])

INLINE int hq_fastabs(int x)		{	int y = x >> 31;	return (x ^ y) - y;	}
INLINE int hq_count_hbit(int x)		{	int	count = 0;	while(x) {	++count;	x = x&(x-1);	}	return count;}
INLINE UINT32 hq_uptopow2(UINT32 x)	{	x--;x|=x>>1;x|=x>>2;x|=x>>4;x|=x>>8;x|=x>>16;x++;return x;	}
INLINE UINT64 hq_uptopow2(UINT64 x)	{	x--;x|=x>>1;x|=x>>2;x|=x>>4;x|=x>>8;x|=x>>16;x|=x>>32;x++;return x;	}

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
#endif//_HQOS_H_
