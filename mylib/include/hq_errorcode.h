#ifndef _HQERRORCODE_H_
#define _HQERRORCODE_H_

#include "hq_type.h"

#define CHECK_RETURN(cond, ret) do {\
	if (cond) {\
		return ret;\
	}\
} while(0)

#define CHECK_GOTO(cond, where) do {\
	if (cond) {\
		goto where;\
	}\
} while(0)

#define CHECK_SET(cond, exp) do {\
	if (cond) {\
		(exp);\
	}\
} while(0)

#define CHECK_SET_GOTO(cond, exp, where) do {\
	if (cond) {\
		(exp);\
		goto where;\
	}\
} while(0)

// ======================================================
// define some invalid value
// ======================================================
const RESULT HQPOSITION_INVALID = -1;

// ======================================================
// begin error code
// ======================================================
const RESULT HQRESULT_SUCCESS = 0;
// ======================================================
// common error code
// ======================================================
const RESULT HQRESULT_COMMON_BASE = 0x0;
const RESULT HQRESULT_COMMON_UNKNOW				= HQRESULT_COMMON_BASE + 0x01;
const RESULT HQRESULT_COMMON_NOTENOUGHMEMORY	= HQRESULT_COMMON_BASE + 0x02;
// ======================================================
// thread related
// ======================================================
const RESULT HQRESULT_THREAD_BASE = 0x100;
const RESULT HQRESULT_THREAD_CREATEFAIL			= HQRESULT_THREAD_BASE + 0x01;

const RESULT HQRESULT_THREADMUTEX_CREATEFAIL	= HQRESULT_THREAD_BASE + 0x02;
const RESULT HQRESULT_THREADMUTEX_LOCKFAIL		= HQRESULT_THREAD_BASE + 0x03;
const RESULT HQRESULT_THREADMUTEX_UNLOCKFAIL	= HQRESULT_THREAD_BASE + 0x04;

const RESULT HQRESULT_THREADSPIN_CREATEFAIL		= HQRESULT_THREAD_BASE + 0x05;
const RESULT HQRESULT_THREADSPIN_LOCKFAIL		= HQRESULT_THREAD_BASE + 0x06;
const RESULT HQRESULT_THREADSPIN_UNLOCKFAIL		= HQRESULT_THREAD_BASE + 0x07;

const RESULT HQRESULT_THREADSEM_CREATEFAIL		= HQRESULT_THREAD_BASE + 0x08;
const RESULT HQRESULT_THREADSEM_WAITFAIL		= HQRESULT_THREAD_BASE + 0x09;
const RESULT HQRESULT_THREADSEM_RELEASEFAIL		= HQRESULT_THREAD_BASE + 0x0A;

const RESULT HQRESULT_THREADEVENT_CREATEFAIL	= HQRESULT_THREAD_BASE + 0x0B;
const RESULT HQRESULT_THREADEVENT_WAITFAIL		= HQRESULT_THREAD_BASE + 0x0C;
const RESULT HQRESULT_THREADEVENT_SIGNALFAIL	= HQRESULT_THREAD_BASE + 0x0D;

// ======================================================
// thread pool related
// ======================================================
const RESULT HQRESULT_THREADPOOL_BASE = 0x200;
const RESULT HQRESULT_THREADPOOL_ERRORSTATUS = HQRESULT_THREADPOOL_BASE + 0x01;
const RESULT HQRESULT_THREADPOOL_INVALIDPARM = HQRESULT_THREADPOOL_BASE + 0x02;

#endif//_HOERRORCODE_H_
