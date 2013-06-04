#ifndef _HQTHREADPOOL_H_
#define _HQTHREADPOOL_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_thread.h>
#include <hq_memory_mgr.h>

struct WorkThreadContext {
	HQTHREADFUNC	context_func;
	void*			context_param;
	UINT32			context_priority;
};

struct ContextNode;
struct ThreadNode;

class HQThreadPool : PUBLIC MemoryManagedBase {
PUBLIC:
	enum {	INVALID_HANDLE = -1	};
	struct Info {
		UINT32	nTracerIdx;
	};
PUBLIC:
	HQThreadPool(const Info* info);
	~HQThreadPool();

	RESULT Initialize(UINT32 threadnum);
	void Finalize();

	RESULT AddWorkContext(const WorkThreadContext& context, INT32* pHandle = NULL);
	RESULT SetPreviousNode(INT32 Handle);
	RESULT BeginGroup(INT32* pHandle = NULL);
	RESULT EndGroup(INT32* pHandle = NULL);

	INLINE UINT32 GetThreadNum() {	return m_nThreadNum;	}
	INLINE BOOLEAN IsInitialized() {	return (m_nThreadNum)? TRUE:FALSE;	}
PRIVATE:
	RESULT initialize_contextarray();
	void finallize_contextarray();

	RESULT initialize_mutex();
	void finallize_mutex();

	RESULT initialize_threads();
	void finallize_threads();

	ContextNode* select_context();
	INT32 add_workcontext(const WorkThreadContext& context);
	void set_previousnode(INT8 idx_current_node, ContextNode* prev_node);

	void prepare_groupendcontext();
	INT32 add_groupendcontext();

	static void* thread_function (void* param);
PRIVATE:
	ContextNode*	m_pWorkingContext;
	UINT32			m_nThreadNum;
	ThreadNode*		m_pThread;

	HQThreadMutex		m_oWorkingContextMutex;
	HQThreadSemaphore	m_oWaitSemaphore;
	HQThreadSemaphore	m_oFreeSemaphore;
	HQThreadEvent		m_oDependencyEvent;

	ContextNode*	m_pFreeList[2];
	ContextNode*	m_pWaitList[2];

	INT32			m_GroupHandle;
	ContextNode*	m_pGroupEndContext;

	INT32			m_nMagicNum;
};

#endif//_HQTHREADPOOL_H_
