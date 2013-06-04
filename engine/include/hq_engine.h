#ifndef _HQENGINE_H_
#define _HQENGINE_H_

#include <hq_type.h>
#include <hq_threadpool_fast.h>
#include <hq_window.h>

class HQEngine : PUBLIC MemoryManagedBase {
PUBLIC:
	HQEngine();
	~HQEngine();
PRIVATE:
	HQThreadPoolFast*	m_pthreadpool;
	HQWindow		m_window;
};

#endif//_HQENGINE_H_
