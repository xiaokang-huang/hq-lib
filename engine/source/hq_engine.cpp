#include <hq_engine.h>
#include <hq_errorcode.h>
#include <hq_thread_pool.h>
#include <hq_util.h>

enum {
	INDEX_COMMON_MEMORY = 0,
	INDEX_RESOURCE_MEMORY,
};

RESULT HQEngine::initialzie_threadpool() {
	m_nThreadNum = GetProcessNum();
	HQThreadPoolFast::Info info = {	INDEX_COMMON_MEMORY	};
	m_pThreadPool = new(INDEX_COMMON_MEMORY) HQThreadPoolFast(&info);
	return HQRESULT_SUCCESS;
}

RESULT HQEngine::finalize_threadpool() {
	if (m_pThreadPool) {
		Managed_Delete_S(m_pThreadPool);
		m_nThreadNum = 0;
	}
	return HQRESULT_SUCCESS;
}

HQEngine::HQEngine() : MemoryManagedBase(INDEX_COMMON_MEMORY),
		m_pThreadPool(NULL),
		m_nThreadNum(0),
		m_window(INDEX_COMMON_MEMORY) {

}

HQEngine::~HQEngine() {
	Finalize();
}

RESULT HQEngine::Initialize() {
	initialzie_threadpool();

	return HQRESULT_SUCCESS;
}

RESULT HQEngine::Finalize() {
	finalize_threadpool();

	return HQRESULT_SUCCESS;
}
