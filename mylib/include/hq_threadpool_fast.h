#ifndef HQTHEADPOOLFAST_H_
#define HQTHEADPOOLFAST_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_thread.h>
#include <hq_memory_mgr.h>
#include <container/hq_concurrentqueue.h>

const UINT32 DefaultContextNum = 256;

struct ContextNodeFast;
class HQThreadPoolFast;
struct ThreadNodeFast;
struct WorkThreadContestGroupFast;
typedef void*(*HQTHREADFUNCFAST)(HQThreadPoolFast*, void*);

struct WorkThreadContextFast : PUBLIC MemoryAllocatorBase {
	HQTHREADFUNCFAST			context_func;
	void*						context_param;
	WorkThreadContextFast(HQTHREADFUNCFAST func, void* param) : context_func(func), context_param(param) {}
};

class HQThreadPoolFast : PUBLIC MemoryManagedBase {
PUBLIC:
	struct Info {
		UINT32	nTracerIdx;
		HQTHREADFUNCFAST	init_func;
		HQTHREADFUNCFAST	final_func;
	};
PUBLIC:
	HQThreadPoolFast(const Info* info);
	~HQThreadPoolFast();

	RESULT Initialize(UINT32 threadnum, UINT32 contextarraysize);
	void Finalize();

	HQHANDLE PutContext(const WorkThreadContextFast& context, WorkThreadContestGroupFast* pgroup = NULL);
	WorkThreadContestGroupFast* CreateGroup(const WorkThreadContextFast& group_end_work);
	void SetGroupReady(WorkThreadContestGroupFast* pgroup);

PRIVATE:
	void initialize_locks();
	void finalize_locks();

	void initialize_thread(UINT32 threadnum);
	void finalize_thread();

	void initialize_context(UINT32 contextarraysize);
	void finalize_context();

	void initialize(UINT32 threadnum, UINT32 contextarraysize);
	void finalize();

	void add_work(const WorkThreadContextFast* pcontext, WorkThreadContestGroupFast* pgroup);
	void add_termwork();
PRIVATE:
	static void* threadpool_func(void* param);
PRIVATE:
	typedef HQConcurrentRingQueue<UINT32> ContextQueue;
	ContextNodeFast*	m_pContextNodeArray;
	ContextQueue*		m_pFreeQueue;
	ContextQueue*		m_pWaitQueue;

	UINT32				m_nThreadNum;
	HQAtomic			m_nThreadCurr;
	ThreadNodeFast*		m_pThread;

	HQThreadSemaphore	m_FreeSemaphore;
	HQThreadSemaphore	m_WaitSemaphore;

	HQTHREADFUNCFAST	m_InitFunc;
	HQTHREADFUNCFAST	m_FinalFunc;
};

#endif// HQTHEADPOOLFAST_H_
