#include <hq_thread_pool.h>
#include <hq_errorcode.h>
#include <stdio.h>

/*
 * function macro
 */
#define PRINT_NODE(pnode) DEBUG_PRINT("\nNode[%p]Func[%p]Prev[%p]Next[%p]", pnode, pnode->_context_data.context_func, pnode->_prev, pnode->_next)

#define PRINT_LIST(list)  do {\
	ContextNode* pnode = list[LISTHEAD];\
	int nodecount = 0;\
	DEBUG_PRINT("\n%s [%p]->[%p]", #list, list[LISTHEAD], list[LISTTAIL]);\
	while (pnode) {\
		DEBUG_PRINT("\nNode[%p]Func[%p]Prev[%p]Next[%p]", pnode, pnode->_context_data.context_func, pnode->_prev, pnode->_next);\
		pnode = pnode->_next;\
		++ nodecount;\
	};\
	DEBUG_PRINT("\nNode count: %d", nodecount);\
} while (0)
#define PRINT_FREELIST PRINT_LIST(m_pFreeList)
#define PRINT_WAITLIST PRINT_LIST(m_pWaitList)

const UINT32 CONTEXTNUMCOEF	= 3;
const UINT32 CONTEXTNUM		= 1 << CONTEXTNUMCOEF;
const UINT32 DEPENDENCY_NUM = CONTEXTNUM;
const UINT16 NULLCONTEXTNODE = -1;

const UINT32 LISTHEAD = 0;
const UINT32 LISTTAIL = 1;

static void* group_func(void*) {	return NULL;	}
const WorkThreadContext GROUPWORK = {	group_func,	(void*)-1,	0	};
const WorkThreadContext TERMWORK  = {	NULL,		(void*)-1,	0	};
#define IS_GROUP_JOB(context)		(context.context_func == GROUPWORK.context_func && context.context_param == GROUPWORK.context_param)
#define IS_TEMINATE_JOB(context)	(context.context_func == TERMWORK.context_func && context.context_param == TERMWORK.context_param)

#define IS_POOLINITED (m_nThreadNum != 0)

#define GET_NODE_INDEX(pnode) ((pnode) - m_pWorkingContext)
#define GET_FIRST_NODE(list, pnode) do {\
	pnode = list[LISTHEAD];\
	list[LISTHEAD] = pnode->_next;\
	pnode->Detach();\
	if (list[LISTHEAD] == NULL)	list[LISTTAIL] = NULL;\
} while (0)
#define PUSHBACK_NODE(list, pnode) do {\
	if (list[LISTTAIL]) {\
		list[LISTTAIL]->InsertAfter(pnode);\
		list[LISTTAIL] = pnode;\
	} else {\
		list[LISTTAIL] = list[LISTHEAD] = pnode;\
	}\
} while (0)

#define NODEHANDLE_PACK(magicnum, idx)	((magicnum << 16) | (idx & 0xFFFF))
#define NODEHANDLE_GETMAGICNUM(handle)	(handle >> 16)
#define NODEHANDLE_GETINDEX(handle)		(handle & 0xFFFF)
#define UPDATEMAGICNUM(magicnum)		(magicnum = (magicnum + 1) & 0x7FFF)

struct ContextNode : PUBLIC MemoryAllocatorBase {
#define _GET_ARRAY_NUM (_adependency[DEPENDENCY_NUM - 1])
	WorkThreadContext	_context_data;
	ContextNode* 		_next;
	ContextNode*		_prev;
	UINT32				_ndependency;	//number of nodes that this node depend on
	UINT8				_adependency[DEPENDENCY_NUM];	//array of nodes that depend on this node, the last element is the actrual size of the array, so the number of dependency node must <= (DEPENDENCY_NUM - 1)
	INT32				_handle;

	void ResetValue() {
		_handle = HQThreadPool::INVALID_HANDLE;
		_ndependency = 0;
		_GET_ARRAY_NUM = 0;
	}

	void Detach() {
		if (_prev)	_prev->_next = _next;
		if (_next)	_next->_prev = _prev;
		_next = NULL;
		_prev = NULL;
	}

	void InsertBefore(ContextNode* pnode) {
		pnode->_next = this;
		pnode->_prev = this->_prev;
		if (this->_prev)	this->_prev->_next = pnode;
		this->_prev = pnode;
	}

	void InsertAfter(ContextNode* pnode) {
		pnode->_next = this->_next;
		pnode->_prev = this;
		if (this->_next)	this->_next->_prev = pnode;
		this->_next = pnode;
	}
	
	inline bool IsDependented() {
		return (_GET_ARRAY_NUM > 0);
	}

	inline void AddDependent(UINT8 idx_nextnode, ContextNode* pWorkingContext) {
		++ pWorkingContext[(UINT32)idx_nextnode]._ndependency;
		_adependency[_GET_ARRAY_NUM ++] = idx_nextnode;
	}
	
	void ReleaseAllDependents(ContextNode* pWorkingContext) {
		for (UINT8 i = 0; i < _GET_ARRAY_NUM; ++i) {
			-- pWorkingContext[(UINT32) _adependency[i] ]._ndependency;
		}
		_GET_ARRAY_NUM = 0;
	}
#undef _GET_ARRAY_NUM
};

struct ThreadNode : PUBLIC MemoryAllocatorBase {
	HQThread		_thread;
	HQThreadPool*	_pool;
	UINT32			_index;
};

/*
 * private function
 */
ContextNode* HQThreadPool::select_context() {
	ContextNode* pselected = NULL;
	{
		pselected = m_pWaitList[LISTHEAD];
		int only_terminate_flag = 0;
		if (IS_TEMINATE_JOB(pselected->_context_data))
			only_terminate_flag = 1;
		while (pselected && pselected->_ndependency) {
			pselected = pselected->_next;
		}
		if (only_terminate_flag == 0 && pselected && IS_TEMINATE_JOB(pselected->_context_data))
			pselected = NULL;
		if (pselected == m_pWaitList[LISTHEAD])
			m_pWaitList[LISTHEAD] = pselected->_next;
		if (pselected == m_pWaitList[LISTTAIL])
			m_pWaitList[LISTTAIL] = pselected->_prev;
		if (pselected != NULL) {
			pselected->Detach();
		}
	}
	return pselected;
}

INT32 HQThreadPool::add_workcontext(const WorkThreadContext& context) {
	ContextNode* pnode = NULL;
	INT32 handle = INVALID_HANDLE;
	m_oFreeSemaphore.Wait();
	{
		m_oWorkingContextMutex.Lock();
		//PRINT_FREELIST;
		GET_FIRST_NODE(m_pFreeList, pnode);
		pnode->_context_data = context;
		PUSHBACK_NODE(m_pWaitList, pnode);
		int idx = GET_NODE_INDEX(pnode);
		handle = pnode->_handle = NODEHANDLE_PACK(m_nMagicNum, idx);
		UPDATEMAGICNUM(m_nMagicNum);
		//handle group
		if (m_GroupHandle != INVALID_HANDLE) {
			//add this node to group begin node dependency list
			ContextNode* pnode_groupbegin = m_pWorkingContext + NODEHANDLE_GETINDEX(m_GroupHandle);
			if (pnode_groupbegin->_handle != INVALID_HANDLE)
				pnode_groupbegin->AddDependent(idx, m_pWorkingContext);
			//add group end node to this node dependency list
			int idx_groupend = GET_NODE_INDEX(m_pGroupEndContext);
			pnode->AddDependent(idx_groupend, m_pWorkingContext);
		}
		m_oWorkingContextMutex.UnLock();
	}
	m_oWaitSemaphore.Release(1);
	return handle;
}

void HQThreadPool::set_previousnode(INT8 idx_current_node, ContextNode* prev_node) {
	prev_node->AddDependent(idx_current_node, m_pWorkingContext);
}

void HQThreadPool::prepare_groupendcontext() {
	m_oFreeSemaphore.Wait();
	{
		m_oWorkingContextMutex.Lock();
		GET_FIRST_NODE(m_pFreeList, m_pGroupEndContext);
		m_pGroupEndContext->_context_data = GROUPWORK;
		m_oWorkingContextMutex.UnLock();
	}
}

INT32 HQThreadPool::add_groupendcontext() {
	INT32 handle;
	{
		m_oWorkingContextMutex.Lock();
		PUSHBACK_NODE(m_pWaitList, m_pGroupEndContext);
		int idx = GET_NODE_INDEX(m_pGroupEndContext);
		handle = m_pGroupEndContext->_handle = NODEHANDLE_PACK(m_nMagicNum, idx);
		UPDATEMAGICNUM(m_nMagicNum);
		m_oWorkingContextMutex.UnLock();
	}
	m_oWaitSemaphore.Release(1);
	return handle;
}

void* HQThreadPool::thread_function(void* param) {
	ThreadNode* info = (ThreadNode*)param;
	ContextNode* selected_node = NULL;
	INT32 working_flag = 1;
	while (working_flag) {
		INT32 need_find_node = 1;
		while (need_find_node) {
			info->_pool->m_oWaitSemaphore.Wait();
			info->_pool->m_oWorkingContextMutex.Lock();
			{
				selected_node = info->_pool->select_context();
				if (selected_node == NULL) {
					info->_pool->m_oWaitSemaphore.Release(1);
					info->_pool->m_oDependencyEvent.Wait(0);
				} else {
					need_find_node = 0;
				}
			}
			info->_pool->m_oWorkingContextMutex.UnLock();
		}
		if (IS_TEMINATE_JOB(selected_node->_context_data)) {
			working_flag = 0;
		} else {
			selected_node->_context_data.context_func(selected_node->_context_data.context_param);
		}
		info->_pool->m_oWorkingContextMutex.Lock();
		{
			if (selected_node->IsDependented()) {
				selected_node->ReleaseAllDependents(info->_pool->m_pWorkingContext);
				info->_pool->m_oDependencyEvent.SignalAll(0);
			}
			selected_node->ResetValue();
			PUSHBACK_NODE(info->_pool->m_pFreeList, selected_node);
		}
		info->_pool->m_oWorkingContextMutex.UnLock();
		info->_pool->m_oFreeSemaphore.Release(1);
	}
	return 0;
}

RESULT HQThreadPool::initialize_contextarray() {
	m_pWorkingContext = Managed_NewN(CONTEXTNUM, ContextNode);
	if (m_pWorkingContext == NULL)
		return HQRESULT_COMMON_NOTENOUGHMEMORY;
	for (UINT32 i = 0; i < CONTEXTNUM; ++i) {
		m_pWorkingContext[i]._next = (i == CONTEXTNUM - 1)? NULL : m_pWorkingContext + i + 1;
		m_pWorkingContext[i]._prev = (i == 0)? NULL : m_pWorkingContext + i - 1;
		m_pWorkingContext[i]._context_data.context_func = NULL;
		m_pWorkingContext[i]._context_data.context_param = 0;
		m_pWorkingContext[i]._context_data.context_priority = 0;
		m_pWorkingContext[i].ResetValue();
	}
	m_pFreeList[LISTHEAD] = m_pWorkingContext + 0;
	m_pFreeList[LISTTAIL] = m_pWorkingContext + (CONTEXTNUM - 1);
	m_pWaitList[LISTHEAD] = NULL;
	m_pWaitList[LISTTAIL] = NULL;

	return HQRESULT_SUCCESS;
}

void HQThreadPool::finallize_contextarray() {
	if (m_pWorkingContext) {
		Managed_DeleteN(m_pWorkingContext);
		m_pWorkingContext = NULL;
	}
}

RESULT HQThreadPool::initialize_mutex() {
	RESULT rc = HQRESULT_SUCCESS;
	rc = m_oWorkingContextMutex.Create();	CHECK_RETURN(rc != HQRESULT_SUCCESS, HQRESULT_COMMON_UNKNOW);
	rc = m_oWaitSemaphore.Create(0);		CHECK_RETURN(rc != HQRESULT_SUCCESS, HQRESULT_COMMON_UNKNOW);
	rc = m_oFreeSemaphore.Create(CONTEXTNUM);	CHECK_RETURN(rc != HQRESULT_SUCCESS, HQRESULT_COMMON_UNKNOW);
	rc = m_oDependencyEvent.Create(&m_oWorkingContextMutex);	CHECK_RETURN(rc != HQRESULT_SUCCESS, HQRESULT_COMMON_UNKNOW);
	return HQRESULT_SUCCESS;
}

void HQThreadPool::finallize_mutex() {
	if (m_pThread) {
		m_oDependencyEvent.Destroy();
		m_oWaitSemaphore.Destroy();
		m_oFreeSemaphore.Destroy();
		m_oWorkingContextMutex.Destroy();
	}
}

RESULT HQThreadPool::initialize_threads() {
	RESULT rc;
	m_pThread = Managed_NewN(m_nThreadNum, ThreadNode);
	if (m_pThread == NULL)
		return HQRESULT_COMMON_NOTENOUGHMEMORY;
	for (UINT32 i = 0; i < m_nThreadNum; ++i) {
		m_pThread[i]._pool = this;
		m_pThread[i]._index = i;
		rc = m_pThread[i]._thread.Create(thread_function, m_pThread + i);
		CHECK_RETURN(rc != HQRESULT_SUCCESS, rc);
	}
	return HQRESULT_SUCCESS;
}

void HQThreadPool::finallize_threads() {
	for (UINT32 i = 0; i < m_nThreadNum; ++i) {
		add_workcontext(TERMWORK);
	}
	for (UINT32 i = 0; i < m_nThreadNum; ++i) {
		m_pThread[i]._thread.Destroy(NULL);
	}
	Managed_DeleteN(m_pThread);
	m_nThreadNum = 0;
}

/*
 * exported function
 */
HQThreadPool::HQThreadPool(const HQThreadPool::Info* info) : MemoryManagedBase(info->nTracerIdx) {
	m_pWorkingContext = NULL;
	m_nThreadNum = 0;
	m_pThread = NULL;
	m_pFreeList[LISTHEAD] = NULL;
	m_pFreeList[LISTTAIL] = NULL;
	m_pWaitList[LISTHEAD] = NULL;
	m_pWaitList[LISTTAIL] = NULL;
	m_nMagicNum = 0;
	m_GroupHandle = INVALID_HANDLE;
	m_pGroupEndContext = NULL;
}

HQThreadPool::~HQThreadPool() {
	Finalize();
}

RESULT HQThreadPool::Initialize(UINT32 threadnum) {
	RESULT rc = HQRESULT_SUCCESS;
	Finalize();
	m_nThreadNum = (threadnum)? threadnum : GetProcessNum();
	rc = initialize_contextarray();	CHECK_RETURN(rc != HQRESULT_SUCCESS, rc);
	rc = initialize_mutex();		CHECK_RETURN(rc != HQRESULT_SUCCESS, rc);
	rc = initialize_threads();		CHECK_RETURN(rc != HQRESULT_SUCCESS, rc);
	return HQRESULT_SUCCESS;
}

void HQThreadPool::Finalize() {
	if (!IS_POOLINITED)
		return;
	finallize_threads();
	finallize_mutex();
	finallize_contextarray();
}

RESULT HQThreadPool::AddWorkContext(const WorkThreadContext& context, INT32* pHandle) {
	INT32 handle = add_workcontext(context);
	if (pHandle)
		*pHandle = handle;
	return HQRESULT_SUCCESS;
}

RESULT HQThreadPool::SetPreviousNode(INT32 Handle) {
	if (Handle == HQThreadPool::INVALID_HANDLE) {
		return HQRESULT_SUCCESS;
	}
	m_oWorkingContextMutex.Lock();
	INT8 idx_prevnode = (INT8)NODEHANDLE_GETINDEX(Handle);
	ContextNode* pprev = m_pWorkingContext + idx_prevnode;
	if (pprev->_handle == HQThreadPool::INVALID_HANDLE) {
		m_oWorkingContextMutex.UnLock();
		return HQRESULT_SUCCESS;
	}
	UINT8 idx_nextnode = (UINT8)GET_NODE_INDEX(m_pFreeList[LISTHEAD]);
	pprev->AddDependent(idx_nextnode, m_pWorkingContext);
	m_oWorkingContextMutex.UnLock();
	return HQRESULT_SUCCESS;
}

RESULT HQThreadPool::BeginGroup(INT32* pHandle) {
	if (m_GroupHandle != INVALID_HANDLE)
		return HQRESULT_THREADPOOL_ERRORSTATUS;
	m_GroupHandle = add_workcontext(GROUPWORK);
	prepare_groupendcontext();
	if (pHandle)
		*pHandle = m_GroupHandle;
	return HQRESULT_SUCCESS;
}
RESULT HQThreadPool::EndGroup(INT32* pHandle) {
	if (m_GroupHandle == INVALID_HANDLE)
		return HQRESULT_THREADPOOL_ERRORSTATUS;
	INT32 handle = add_groupendcontext();
	if (pHandle)
		*pHandle = handle;
	m_GroupHandle = INVALID_HANDLE;
	return HQRESULT_SUCCESS;
}
