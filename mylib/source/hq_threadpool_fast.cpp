#include <hq_threadpool_fast.h>
#include <hq_errorcode.h>

#define IS_TERMWORK(pworknode) ((pworknode)->context.context_func == NULL)
#define GROUP_READY_MASK	(0x1 << 16)

struct ContextNodeFast : PUBLIC MemoryAllocatorBase {
	WorkThreadContextFast context;
	WorkThreadContestGroupFast* group;
	ContextNodeFast() : context(NULL, NULL), group(NULL) {}
};

struct ThreadNodeFast : PUBLIC MemoryAllocatorBase {
	HQThread			_thread;
	HQThreadPoolFast*	_pool;
	UINT32				_index;
};

struct WorkThreadContestGroupFast : PUBLIC WorkThreadContextFast {
	HQAtomic group_magicnum;

	WorkThreadContestGroupFast(HQTHREADFUNCFAST func, void* param) : WorkThreadContextFast(func, param) {
		group_magicnum.Set(0);
	}

	void ReleaseSelf() {
		delete this;
	}
};

void* HQThreadPoolFast::threadpool_func(void* param) {
	ThreadNodeFast* pthreadnode = (ThreadNodeFast*)param;
	BOOLEAN running = TRUE;
	UINT32 idx;

	while (running) {
		pthreadnode->_pool->m_WaitSemaphore.Wait();
		pthreadnode->_pool->m_pWaitQueue->PopFront(&idx);

		ContextNodeFast* pcontextnode = (pthreadnode->_pool->m_pContextNodeArray) + idx;
		if ( IS_TERMWORK(pcontextnode) ) {
			running = FALSE;
		} else {
			pcontextnode->context.context_func(pthreadnode->_pool, pcontextnode->context.context_param);
		}

		if (pcontextnode->group) {
			INT32 ref = pcontextnode->group->group_magicnum.Sub(1);
			if (ref == (GROUP_READY_MASK)) {
				pthreadnode->_pool->add_work((WorkThreadContextFast*)pcontextnode->group, NULL);
				Managed_Delete_S(pcontextnode->group);
			}
		}

		pthreadnode->_pool->m_pFreeQueue->PushBack(idx);
		pthreadnode->_pool->m_FreeSemaphore.Release(1);
	}
	return NULL;
}

void HQThreadPoolFast::initialize_locks() {
	UINT32 num = m_pFreeQueue->GetSize();
	m_FreeSemaphore.Create(num);
	m_WaitSemaphore.Create(0);
}

void HQThreadPoolFast::finalize_locks() {
	m_FreeSemaphore.Destroy();
	m_WaitSemaphore.Destroy();
}

void HQThreadPoolFast::initialize_thread(UINT32 threadnum) {
	finalize_thread();
	m_nThreadNum = (threadnum == 0)? GetProcessNum() : threadnum;
	m_pThread = Managed_NewN(m_nThreadNum, ThreadNodeFast);
	for (UINT32 i = 0; i < m_nThreadNum; ++i) {
		m_pThread[i]._index = i;
		m_pThread[i]._pool = this;
		m_pThread[i]._thread.Create(threadpool_func, (void*)(m_pThread + i));
	}
}

void HQThreadPoolFast::finalize_thread() {
	if (m_pThread == NULL)	return;
	for (UINT32 i = 0; i < m_nThreadNum; ++i) {
		add_termwork();
	}
	for (UINT32 i = 0; i < m_nThreadNum; ++i) {
		m_pThread[i]._thread.Destroy(NULL);
	}
	Managed_DeleteN_S(m_pThread);
	m_nThreadNum = 0;
}

void HQThreadPoolFast::initialize_context(UINT32 contextarraysize) {
	finalize_context();
	m_pContextNodeArray = Managed_NewN(contextarraysize, ContextNodeFast);
	m_pFreeQueue = Managed_New(ContextQueue, (contextarraysize, m_nTracerIdx));
	m_pWaitQueue = Managed_New(ContextQueue, (contextarraysize, m_nTracerIdx));

	for (UINT32 i = 0; i < contextarraysize; ++i) {
		m_pFreeQueue->PushBack(i);
	}
}

void HQThreadPoolFast::finalize_context() {
	if (m_pContextNodeArray == NULL)	return;
	Managed_Delete_S(m_pContextNodeArray);
	Managed_Delete_S(m_pFreeQueue);
	Managed_Delete_S(m_pWaitQueue);
}

void HQThreadPoolFast::initialize(UINT32 threadnum, UINT32 contextarraysize) {
	initialize_context(contextarraysize);
	initialize_locks();
	initialize_thread(threadnum);
}

void HQThreadPoolFast::finalize() {
	finalize_thread();
	finalize_locks();
	finalize_context();
}

void HQThreadPoolFast::add_work(const WorkThreadContextFast* pcontext, WorkThreadContestGroupFast* pgroup) {
	UINT32 idx = 0;
	BOOLEAN ret;
	m_FreeSemaphore.Wait();
	ret = m_pFreeQueue->PopFront(&idx);
	ASSERT(ret == TRUE);

	m_pContextNodeArray[idx].context = *pcontext;
	m_pContextNodeArray[idx].group = pgroup;
	if (pgroup) {
		pgroup->group_magicnum.Add(1);
	}

	m_pWaitQueue->PushBack(idx);
	m_WaitSemaphore.Release(1);
}

void HQThreadPoolFast::add_termwork() {
	WorkThreadContextFast term(NULL, NULL);
	add_work(&term, NULL);
}

HQThreadPoolFast::HQThreadPoolFast(const Info* info) :
		MemoryManagedBase(info->nTracerIdx), m_pContextNodeArray(NULL),
		m_pFreeQueue(NULL), m_pWaitQueue(NULL), m_nThreadNum(0), m_pThread(NULL) {
}

HQThreadPoolFast::~HQThreadPoolFast() {
	Finalize();
}

RESULT HQThreadPoolFast::Initialize(UINT32 threadnum, UINT32 contextarraysize) {
	contextarraysize = hq_uptopow2(contextarraysize);
	initialize(threadnum, contextarraysize);
	return HQRESULT_SUCCESS;
}

void HQThreadPoolFast::Finalize() {
	finalize();
}

HQHANDLE HQThreadPoolFast::PutContext(const WorkThreadContextFast& context, WorkThreadContestGroupFast* pgroup) {
	add_work(&context, pgroup);
	return 0;
}

WorkThreadContestGroupFast* HQThreadPoolFast::CreateGroup(const WorkThreadContextFast& group_end_work) {
	WorkThreadContestGroupFast* node = Managed_New(WorkThreadContestGroupFast, (group_end_work.context_func, group_end_work.context_param));
	return node;
}

void HQThreadPoolFast::SetGroupReady(WorkThreadContestGroupFast* pgroup) {
	INT32 ret = pgroup->group_magicnum.Or(GROUP_READY_MASK);
	if (ret == (GROUP_READY_MASK)) {
		add_work(pgroup, NULL);
		Managed_Delete_S(pgroup);
	}
}
