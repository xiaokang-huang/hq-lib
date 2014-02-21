#include "test_threadpool_fast.h"
CPPUNIT_TEST_SUITE_REGISTRATION( test_threadpool_fast );

#include <hq_threadpool_fast.h>
#include <hq_errorcode.h>
#include <unistd.h>

#define INIT_THREADPOOL(threadnum, arraysize) \
	RESULT rc = HQRESULT_SUCCESS;\
	HQThreadPoolFast::Info info = {\
			0, NULL, NULL\
	};\
	HQThreadPoolFast thread_pool(&info);\
	rc = thread_pool.Initialize(threadnum, arraysize);\
	CPPUNIT_ASSERT_EQUAL(HQRESULT_SUCCESS, rc);

#define DEINIT_THREADPOOL() \
	thread_pool.Finalize();\

void test_threadpool_fast::setUp() {
}

void test_threadpool_fast::tearDown() {
}

static void* atomic_add(HQThreadPoolFast* ppool, void* param) {
	HQAtomic* patom = (HQAtomic*)param;
	patom->Add(1);
	return NULL;
}

void test_threadpool_fast::TC_01_01() {
	INIT_THREADPOOL(3, 128);

	HQAtomic atom;
	WorkThreadContextFast context( atomic_add, &atom	);

	for (UINT32 i = 0;  i < 256; ++i) {
		thread_pool.PutContext(context);
	}

	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((INT32)256, atom.Get());
}

struct test_struct {
	UINT32			data;
	HQThreadMutex	mutex;
	HQThreadEvent	event;
};

static void* add_and_push(HQThreadPoolFast* ppool, void* param) {
	test_struct* pstruct = (test_struct*)param;
	pstruct->data += 1;

	if (pstruct->data < 256) {
		WorkThreadContextFast context(	add_and_push, param	);
		ppool->PutContext(context);
	} else {
		usleep(1000000);
		pstruct->event.SignalAll();
	}

	return NULL;
}

void test_threadpool_fast::TC_01_02() {
	INIT_THREADPOOL(3, 128);

	test_struct data;
	data.data = 0;
	data.event.Create();


	WorkThreadContextFast context(	add_and_push, &data	);
	thread_pool.PutContext(context);

	data.event.Wait();
	DEINIT_THREADPOOL();
	data.event.Destroy();
	CPPUNIT_ASSERT_EQUAL((UINT32)256, data.data);
}

void test_threadpool_fast::TC_01_03() {
#define LOOP_NUM 4
	INIT_THREADPOOL(3, 128);

	UINT32 data = 0;
	WorkThreadContextFast context(	atomic_add, &data	);
	WorkThreadContestGroupFast* group = thread_pool.CreateGroup(context);
	for (UINT32 i = 0;  i < 4; ++i) {
		thread_pool.PutContext(context, group);
		usleep(100000);
	}
	thread_pool.SetGroupReady(group);

	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)(LOOP_NUM+1), data);
#undef LOOP_NUM
}

void test_threadpool_fast::TC_01_04() {
#define LOOP_NUM 256
	INIT_THREADPOOL(4, 512);

	UINT32 data = 0;
	WorkThreadContextFast context(	atomic_add, &data	);
	WorkThreadContestGroupFast* group = thread_pool.CreateGroup(context);
	for (UINT32 i = 0;  i < LOOP_NUM; ++i) {
		thread_pool.PutContext(context, group);
	}
	thread_pool.SetGroupReady(group);

	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)(LOOP_NUM+1), data);
#undef LOOP_NUM
}

void test_threadpool_fast::TC_01_05() {
#define LOOP_NUM 512
	INIT_THREADPOOL(4, 32);

	UINT32 data = 0;
	WorkThreadContextFast context(	atomic_add, &data	);
	WorkThreadContestGroupFast* group = thread_pool.CreateGroup(context);
	for (UINT32 i = 0;  i < LOOP_NUM; ++i) {
		thread_pool.PutContext(context, group);
		usleep(20000);
	}
	thread_pool.SetGroupReady(group);

	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)(LOOP_NUM+1), data);
#undef LOOP_NUM
}

static void* atomic_add_sleep(HQThreadPoolFast* ppool, void* param) {
	HQAtomic* patom = (HQAtomic*)param;
	patom->Add(1);
	usleep(20000);
	return NULL;
}

void test_threadpool_fast::TC_01_06() {
#define LOOP_NUM 4
	INIT_THREADPOOL(4, 512);

	UINT32 data = 0;
	WorkThreadContextFast context(	atomic_add_sleep, &data	);
	WorkThreadContestGroupFast* group = thread_pool.CreateGroup(context);
	for (UINT32 i = 0;  i < LOOP_NUM; ++i) {
		thread_pool.PutContext(context, group);
	}
	thread_pool.SetGroupReady(group);

	DEINIT_THREADPOOL();
	CPPUNIT_ASSERT_EQUAL((UINT32)(LOOP_NUM+1), data);
#undef LOOP_NUM
}
