#include <hq_engine.h>
#include <hq_errorcode.h>
#include <hq_thread_pool.h>
#include <hq_util.h>
#include <hq_event_def.h>

enum {
	INDEX_COMMON_MEMORY = 0,
	INDEX_RESOURCE_MEMORY,
};

/*
 * ------------------------------------------------------------
 * static function and data
 * ------------------------------------------------------------
 */

void HQEngine::EngineEventCallBack::DoCallBack(HQEventType type, HQEventData data) {
	HQEngine* pengine = (HQEngine*)usr_ptr;
	if (type == HQEVENTTYPE_SYSTEM_EXIT) {
		pengine->m_status = HQEngine::STATE_EXIT;
	}
}

/*
 * ------------------------------------------------------------
 * private function
 * ------------------------------------------------------------
 */

RESULT HQEngine::initialize_threadpool() {
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

RESULT HQEngine::initialize_window() {
	HQWindow::Info info = {
			FALSE,
			200,
			200,
			800,
			600,
			NULL
	};
	m_window.Create(&info);
	return HQRESULT_SUCCESS;
}

RESULT HQEngine::finalize_window() {
	m_window.Destroy();
	return HQRESULT_SUCCESS;
}

/*
 * ------------------------------------------------------------
 * public function
 * ------------------------------------------------------------
 */

HQEngine::HQEngine() :	MemoryManagedBase(INDEX_COMMON_MEMORY),
						m_pThreadPool(NULL),
						m_nThreadNum(0),
						m_window(INDEX_COMMON_MEMORY),
						m_status(STATE_EXIT) {

}

HQEngine::~HQEngine() {
	Finalize();
}

RESULT HQEngine::Initialize() {
	initialize_threadpool();
	initialize_window();

	m_status = STATE_READY;

	return HQRESULT_SUCCESS;
}

RESULT HQEngine::Finalize() {
	m_status = STATE_EXIT;

	finalize_window();
	finalize_threadpool();

	return HQRESULT_SUCCESS;
}

RESULT HQEngine::Start() {
	HQEventStructure event;
	while (TRUE) {
		m_window.GetEvent(&event);
		if (event._type == HQEVENTTYPE_SYSTEM_EXIT) {
			m_status = STATE_EXIT;
			break;
		}
	}
	return HQRESULT_SUCCESS;
}
