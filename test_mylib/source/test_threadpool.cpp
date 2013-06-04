#include "test_threadpool.h"
CPPUNIT_TEST_SUITE_REGISTRATION( test_threadpool );

#include <hq_thread_pool.h>
#include <hq_errorcode.h>

#define INIT_THREADPOOL(threadnum) \
	RESULT rc = HQRESULT_SUCCESS;\
	HQThreadPool::Info info = {\
			0\
	};\
	HQThreadPool thread_pool(&info);\
	rc = thread_pool.Initialize(3);\
	CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);

#define DEINIT_THREADPOOL() \
	thread_pool.Finalize();\

void test_threadpool::setUp() {
}

void test_threadpool::tearDown() {
}

static void* thread_func_add(void* param) {
	UINT32* data = (UINT32*)param;
	*data += 1;
	return 0;
}

static void* thread_func_add_atomic(void* param) {
	HQAtomic* data = (HQAtomic*)param;
	data->Add(1);
	return 0;
}

static void* thread_func_mul(void* param) {
	UINT32* data = (UINT32*)param;
	*data *= 2;
	return 0;
}

void test_threadpool::TC_01_01() {
	INIT_THREADPOOL(3);

	UINT32 param = 15;
	WorkThreadContext thecontext;
	thecontext.context_func = thread_func_add;
	thecontext.context_param = &param;

	rc = thread_pool.AddWorkContext(thecontext, NULL);
	CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)16, param);
}

void test_threadpool::TC_01_02() {
	INIT_THREADPOOL(2);

	HQAtomic param;
	param.Set(15);
	WorkThreadContext thecontext;
	thecontext.context_func = thread_func_add_atomic;
	thecontext.context_param = &param;

	for (int i = 0; i < 100; ++i) {
		rc = thread_pool.AddWorkContext(thecontext, NULL);
		CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
	}
	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((INT32)(15+100), param.Get());
}

void test_threadpool::TC_01_03() {
	INIT_THREADPOOL(3);

	UINT32 param = 1;
	WorkThreadContext thecontext;
	thecontext.context_param = &param;
	INT32 handle = HQThreadPool::INVALID_HANDLE;
	HQTHREADFUNC funclist[] = {
		thread_func_add,
		thread_func_mul,
	};
	UINT32 listsize = sizeof(funclist) / sizeof(funclist[0]);
	for (int i = 0; i < 10; ++i) {
		rc = thread_pool.SetPreviousNode(handle);
		CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
		thecontext.context_func = funclist[i % listsize];
		rc = thread_pool.AddWorkContext(thecontext, &handle);
		CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
	}
	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)94, param);
}



void test_threadpool::TC_02_01() {
	INIT_THREADPOOL(3);
	UINT32 param = 1;
	WorkThreadContext thecontext;
	INT32 handle = HQThreadPool::INVALID_HANDLE;
	for (int i = 0; i < 100; ++i) {
		rc = thread_pool.SetPreviousNode(handle);	CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
		rc = thread_pool.BeginGroup();				CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
		{
			thecontext.context_func  = thread_func_add;
			thecontext.context_param = &param;
			rc = thread_pool.AddWorkContext(thecontext);	CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
		}
		rc = thread_pool.EndGroup(&handle);			CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);
	}
	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)(1+100), param);
}
