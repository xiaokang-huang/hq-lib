#ifndef _HQTHREAD_H_
#define _HQTHREAD_H_

#include <hq_type.h>
#include <hq_os.h>

// ======================================================
// HQThread
// ======================================================
typedef void*(*HQTHREADFUNC)(void*);

class HQNativeThreadFunc;
class HQThread;
class HQAtomic;
class HQThreadMutex;
class HQThreadSpinLock;
class HQThreadSemaphore;
class HQThreadEvent;

class HQThread {
PUBLIC:
	RESULT Create(HQTHREADFUNC func, void* param);
	RESULT Destroy(UINTSYS* poutput);
PRIVATE:
	friend class HQNativeThreadFunc;
PRIVATE:
	THREADHANDLE	m_thread;
	HQTHREADFUNC	m_func;
	void*			m_param;
};

// ======================================================
// HQAtomic
// ======================================================
class HQAtomic {
PUBLIC:
	HQAtomic() : m_value(0) {}
	void Set(INT32 value);
	INT32 Get() const;
	INT32 Add(INT32 value);
	INT32 Sub(INT32 value);
	INT32 And(INT32 value);
	INT32 Or (INT32 value);
	INT32 Xor(INT32 value);
	BOOLEAN CompareAndSwap(INT32 oldvalue, INT32 newvalue);
PRIVATE:
	INT32 m_value;
};

// ======================================================
// HQThreadMutex
// ======================================================
class HQThreadMutex {
PUBLIC:
	RESULT Create();
	RESULT Destroy();
	RESULT Lock();
	RESULT UnLock();
PRIVATE:
	friend class HQThreadEvent;
PRIVATE:
	THREADMUTEX	m_instance;
};

// ======================================================
// HQThreadSpinLock
// ======================================================
class HQThreadSpinLock {
PUBLIC:
	RESULT Create();
	RESULT Destroy();
	RESULT Lock();
	RESULT UnLock();
PRIVATE:
	THREADSPINLOCK m_instance;
};

// ======================================================
// HQThreadSemaphore
// ======================================================
class HQThreadSemaphore {
PUBLIC:
	RESULT Create(UINT32 value);
	RESULT Destroy();
	RESULT Wait();
	RESULT Release(UINT32 value);
PRIVATE:
	THREADSEMAPHORE m_instance;
};

// ======================================================
// HQThreadEvent
// ======================================================
class HQThreadEvent {
PUBLIC:
	RESULT Create(HQThreadMutex* pmutex = NULL);
	RESULT Destroy();
	RESULT Wait(int blockmutex = 1);
	RESULT SignalAll(int blockmutex = 1);
PRIVATE:
	THREADEVENT	m_instance;
	HQThreadMutex m_mutex;
	HQThreadMutex* m_pmutex;
};

#endif//_HQTHREAD_H_

