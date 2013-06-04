#ifndef _HQENGINE_H_
#define _HQENGINE_H_

#include <hq_type.h>
#include <hq_threadpool_fast.h>
#include <hq_memory_mgr.h>
#include <hq_window.h>

class HQEngine : PUBLIC MemoryManagedBase {
PUBLIC:
	HQEngine();
	~HQEngine();

	RESULT Initialize();
	RESULT Finalize();
PRIVATE:
	RESULT initialzie_threadpool();
	RESULT finalize_threadpool();
PRIVATE:
	HQThreadPoolFast*	m_pThreadPool;
	UINT32				m_nThreadNum;

	HQWindow		m_window;
};

#endif//_HQENGINE_H_
