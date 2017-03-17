#include "test_thread.h"
CPPUNIT_TEST_SUITE_REGISTRATION( test_thread );

#include <hq_thread.h>
#include <hq_errorcode.h>

#include <unistd.h>

#define RUN_CHECKEQUAL(exp, value_should_be) do {\
	rc = (exp);\
	CPPUNIT_ASSERT_EQUAL((RESULT)value_should_be, rc);\
} while (0)

void test_thread::setUp() {
}

void test_thread::tearDown() {
}

void* test_thread_func1(void* param) {
	CPPUNIT_ASSERT_EQUAL((UINT32)0, *(UINT32*)param);
	*(UINT32*)param += 1024;
	return (void*)(UINTSYS)*(UINT32*)param;
}

void test_thread::TC_01_01() {
	HQThread thread;
	UINTSYS in = 0;
	UINTSYS out;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(thread.Create(test_thread_func1, (void*)(&in)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread.Destroy(&out), HQRESULT_SUCCESS);
	CPPUNIT_ASSERT_EQUAL((UINTSYS)1024, in);
	CPPUNIT_ASSERT_EQUAL((UINTSYS)1024, out);
}

void* test_thread_func2(void* param) {
	HQThreadMutex* pmutex = (HQThreadMutex*)param;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(pmutex->Lock(), HQRESULT_SUCCESS);
	sleep(1);
	RUN_CHECKEQUAL(pmutex->UnLock(), HQRESULT_SUCCESS);
	return 0;
}

void* test_thread_func22(void* param) {
	HQThreadMutex* pmutex = (HQThreadMutex*)param;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(pmutex->Lock(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(pmutex->Lock(), HQRESULT_SUCCESS);
	sleep(1);
	RUN_CHECKEQUAL(pmutex->UnLock(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(pmutex->UnLock(), HQRESULT_SUCCESS);
	return 0;
}

void test_thread::TC_02_01() {
	HQThread thread1, thread2;
	HQThreadMutex mutex;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(mutex.Create(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func2, (void*)(&mutex)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Create(test_thread_func2, (void*)(&mutex)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(mutex.Destroy(), HQRESULT_SUCCESS);
}

void test_thread::TC_02_02() {
	HQThread thread1, thread2;
	HQThreadMutex mutex;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(mutex.Create(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func22, (void*)(&mutex)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Create(test_thread_func22, (void*)(&mutex)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(mutex.Destroy(), HQRESULT_SUCCESS);
}

void test_thread::TC_02_03() {
	HQThreadMutex mutex;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(mutex.Create(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(mutex.UnLock(), HQRESULT_THREADMUTEX_UNLOCKFAIL);
	RUN_CHECKEQUAL(mutex.Destroy(), HQRESULT_SUCCESS);
}

void* test_thread_func3(void* param) {
	HQThreadMutex* pmutex = (HQThreadMutex*)param;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(pmutex->Lock(), HQRESULT_SUCCESS);
	sleep(1);
	RUN_CHECKEQUAL(pmutex->UnLock(), HQRESULT_SUCCESS);
	return 0;
}

void test_thread::TC_03_01() {
	HQThread thread1, thread2;
	HQThreadMutex mutex;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(mutex.Create(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func3, (void*)(&mutex)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Create(test_thread_func3, (void*)(&mutex)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(mutex.Destroy(), HQRESULT_SUCCESS);
}

void* test_thread_func4(void* param) {
	HQThreadSemaphore* psema = (HQThreadSemaphore*)param;
	RESULT rc = HQRESULT_SUCCESS;
	for (int i = 0; i < 101; ++ i)
		RUN_CHECKEQUAL(psema->Wait(), HQRESULT_SUCCESS);
	return 0;
}

void test_thread::TC_04_01() {
	HQThread thread1;
	HQThreadSemaphore sema;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(sema.Create(100), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func4, (void*)(&sema)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(sema.Release(1), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(sema.Destroy(), HQRESULT_SUCCESS);
}

struct event_struct {
	HQThreadEvent 	event;
	HQAtomic		data;
};

void* test_thread_func5A(void* param) {
	event_struct* pstruct = (event_struct*)param;
	HQThreadEvent* pevent = &(pstruct->event);
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(pevent->Wait(), HQRESULT_SUCCESS);
	pstruct->data.Add(1);
	return 0;
}

void* test_thread_func5B(void* param) {
	event_struct* pstruct = (event_struct*)param;
	HQThreadEvent* pevent = &(pstruct->event);
	pstruct->data.Set(pstruct->data.Get() * 2);
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(pevent->SignalAll(), HQRESULT_SUCCESS);
	return 0;
}

void test_thread::TC_05_01() {
	HQThread thread1, thread2, thread3;
	HQThreadMutex mutex;
	event_struct str;
	RESULT rc = HQRESULT_SUCCESS;
	RUN_CHECKEQUAL(mutex.Create(), HQRESULT_SUCCESS);
	str.data.Set(1);
	RUN_CHECKEQUAL(str.event.Create(&mutex), HQRESULT_SUCCESS);

	RUN_CHECKEQUAL(thread2.Create(test_thread_func5A, (void*)(&str)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread3.Create(test_thread_func5A, (void*)(&str)), HQRESULT_SUCCESS);
	sleep(2);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func5B, (void*)(&str)), HQRESULT_SUCCESS);

	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread3.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(str.event.Destroy(), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(mutex.Destroy(), HQRESULT_SUCCESS);

	CPPUNIT_ASSERT_EQUAL((INT32)4, str.data.Get());
}

void test_thread::TC_05_02() {
	HQThread thread1, thread2, thread3;
	event_struct str;
	RESULT rc = HQRESULT_SUCCESS;
	str.data.Set(1);
	RUN_CHECKEQUAL(str.event.Create(), HQRESULT_SUCCESS);

	RUN_CHECKEQUAL(thread2.Create(test_thread_func5A, (void*)(&str)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread3.Create(test_thread_func5A, (void*)(&str)), HQRESULT_SUCCESS);
	sleep(2);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func5B, (void*)(&str)), HQRESULT_SUCCESS);

	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread3.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(str.event.Destroy(), HQRESULT_SUCCESS);

	CPPUNIT_ASSERT_EQUAL((INT32)4, str.data.Get());
}

void* test_thread_func6(void* param) {
	HQAtomic* patom = (HQAtomic*)param;
	for (int i = 0; i < 1000; ++i)
		patom->Add(1);
	return 0;
}

void test_thread::TC_06_01() {
	HQThread thread1, thread2, thread3;
	HQAtomic atom;
	RESULT rc = HQRESULT_SUCCESS;
	atom.Set(0);
	RUN_CHECKEQUAL(thread1.Create(test_thread_func6, (void*)(&atom)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Create(test_thread_func6, (void*)(&atom)), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread3.Create(test_thread_func6, (void*)(&atom)), HQRESULT_SUCCESS);

	RUN_CHECKEQUAL(thread1.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread2.Destroy(NULL), HQRESULT_SUCCESS);
	RUN_CHECKEQUAL(thread3.Destroy(NULL), HQRESULT_SUCCESS);

	long value = atom.Get();
	CPPUNIT_ASSERT_EQUAL((long)3000, value);
}
