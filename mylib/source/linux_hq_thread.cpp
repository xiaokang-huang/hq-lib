#include <hq_thread.h>
#include <hq_errorcode.h>
#include <pthread.h>

// ======================================================
// HQThread
// ======================================================
class HQNativeThreadFunc {
PUBLIC:
	static void* thread_func(void *arg);
};

void* HQNativeThreadFunc::thread_func(void *arg) {
	HQThread* pthis = (HQThread*)arg;
	return (pthis->m_func)(pthis->m_param);
}

RESULT HQThread::Create(HQTHREADFUNC func, void* param) {
	m_func = func;
	m_param = param;
	pthread_create(&m_thread, NULL, HQNativeThreadFunc::thread_func, this);
	return HQRESULT_SUCCESS;
}

RESULT HQThread::Destroy(UINTSYS* poutput) {
	void* presult = NULL;
	pthread_join(m_thread, &presult);
	if (poutput)
		*poutput = (UINTSYS)presult;
	return HQRESULT_SUCCESS;
}

// ======================================================
// HQAtomic
// ======================================================
void HQAtomic::Set(INT32 value) {
	m_value = value;
}

INT32 HQAtomic::Get() const {
	return m_value;
}

INT32 HQAtomic::Add(INT32 value) {
	return __sync_add_and_fetch(&m_value, value);
}

INT32 HQAtomic::Sub(INT32 value) {
	return __sync_sub_and_fetch(&m_value, value);
}

INT32 HQAtomic::And(INT32 value) {
	return __sync_and_and_fetch(&m_value, value);
}

INT32 HQAtomic::Or (INT32 value) {
	return __sync_or_and_fetch(&m_value, value);
}

INT32 HQAtomic::Xor(INT32 value) {
	return __sync_xor_and_fetch(&m_value, value);
}

BOOLEAN HQAtomic::CompareAndSwap(INT32 oldvalue, INT32 newvalue) {
	return __sync_bool_compare_and_swap(&m_value, oldvalue, newvalue);
}
// ======================================================
// HQThreadMutex
// ======================================================
RESULT HQThreadMutex::Create() {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	if (pthread_mutex_init(&m_instance, &attr) == 0)
		return HQRESULT_SUCCESS;
	else
		return HQRESULT_THREAD_CREATEFAIL;
}

RESULT HQThreadMutex::Destroy() {
	pthread_mutex_destroy(&m_instance);
	return HQRESULT_SUCCESS;
}

RESULT HQThreadMutex::Lock() {
	if (pthread_mutex_lock(&m_instance) == 0)
		return HQRESULT_SUCCESS;
	else
		return HQRESULT_THREADMUTEX_LOCKFAIL;
}

RESULT HQThreadMutex::UnLock() {
	if (pthread_mutex_unlock(&m_instance) == 0)
		return HQRESULT_SUCCESS;
	else
		return HQRESULT_THREADMUTEX_UNLOCKFAIL;
}

// ======================================================
// HQThreadSpinLock
// ======================================================
RESULT HQThreadSpinLock::Create() {
	if (pthread_spin_init(&m_instance, 0) == 0)
			return HQRESULT_SUCCESS;
		else
			return HQRESULT_THREADSPIN_CREATEFAIL;
}

RESULT HQThreadSpinLock::Destroy() {
	pthread_spin_destroy(&m_instance);
		return HQRESULT_SUCCESS;
}

RESULT HQThreadSpinLock::Lock() {
	if (pthread_spin_lock(&m_instance) == 0)
			return HQRESULT_SUCCESS;
		else
			return HQRESULT_THREADSPIN_LOCKFAIL;
}

RESULT HQThreadSpinLock::UnLock() {
	if (pthread_spin_unlock(&m_instance) == 0)
		return HQRESULT_SUCCESS;
	else
		return HQRESULT_THREADSPIN_UNLOCKFAIL;
}

// ======================================================
// HQThreadSemaphore
// ======================================================
RESULT HQThreadSemaphore::Create(UINT32 value) {
	if (sem_init(&m_instance, 0, (unsigned int)value) == 0)
		return HQRESULT_SUCCESS;
	else
		return HQRESULT_THREADSEM_CREATEFAIL;
}
RESULT HQThreadSemaphore::Destroy() {
	sem_destroy(&m_instance);
	return HQRESULT_SUCCESS;
}
RESULT HQThreadSemaphore::Wait() {
	if (sem_wait(&m_instance) == 0)
		return HQRESULT_SUCCESS;
	else
		return HQRESULT_THREADSEM_WAITFAIL;
}
RESULT HQThreadSemaphore::Release(UINT32 value) {
	for (UINT32 i = 0; i < value; ++ i) {
		if (sem_post(&m_instance) != 0)
			return HQRESULT_THREADSEM_RELEASEFAIL;
	}
	return HQRESULT_SUCCESS;
}

// ======================================================
// HQThreadEvent
// ======================================================
RESULT HQThreadEvent::Create(HQThreadMutex* pmutex) {
	int rc;
	m_pmutex = NULL;
	if (pmutex == NULL) {
		rc = m_mutex.Create();
		if (rc != HQRESULT_SUCCESS)
			return HQRESULT_THREAD_CREATEFAIL;
	} else {
		m_pmutex = pmutex;
	}
	rc = pthread_cond_init(&m_instance, NULL);
	if (rc != 0) {
		return HQRESULT_THREADEVENT_CREATEFAIL;
	}

	return HQRESULT_SUCCESS;
}
RESULT HQThreadEvent::Destroy() {
	pthread_cond_destroy(&m_instance);
	if (m_pmutex == NULL)
		m_mutex.Destroy();
	return HQRESULT_SUCCESS;
}

#define HANDLE_MUTEX(func) \
	if (blockmutex) {\
		rc = pmutex->func();\
		CHECK_RETURN(rc != HQRESULT_SUCCESS, rc);\
	}
RESULT HQThreadEvent::Wait(int blockmutex) {
	RESULT rc;
	HQThreadMutex* pmutex = (m_pmutex)? m_pmutex : &m_mutex;
	HANDLE_MUTEX(Lock);
	rc = (pthread_cond_wait(&m_instance, &(pmutex->m_instance)) != 0)? HQRESULT_THREADEVENT_WAITFAIL : HQRESULT_SUCCESS;
	HANDLE_MUTEX(UnLock);
	return rc;

}
RESULT HQThreadEvent::SignalAll(int blockmutex) {
	RESULT rc;
	HQThreadMutex* pmutex = (m_pmutex)? m_pmutex : &m_mutex;
	HANDLE_MUTEX(Lock);
	rc = (pthread_cond_broadcast(&m_instance) != 0)? HQRESULT_THREADEVENT_WAITFAIL : HQRESULT_SUCCESS;
	HANDLE_MUTEX(UnLock);
	return rc;
}
#undef HANDLE_MUTEX
