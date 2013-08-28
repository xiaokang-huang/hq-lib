#include <hq_engine.h>
#include <hq_errorcode.h>
#include <hq_thread_pool.h>
#include <hq_util.h>
#include <hq_event_def.h>

#include <unistd.h>

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

void* HQEngine::thread_func(HQThreadPoolFast* pool, void* param) {
	printf("%s Enter\n", __FUNCTION__);
	HQEngine* pengine = (HQEngine*)param;
	pengine->m_window.AttachCurrentThread();
	printf("%s AttachCurrentThread\n", __FUNCTION__);

	HQEventStructure event;
	UINT32 event_get;
	do {
		event_get = pengine->m_window.GetEvent(&event, 1);
		if (event._type == HQEVENTTYPE_SYSTEM_EXIT) {
			pengine->m_status = STATE_EXIT;
			return NULL;
		}
	} while (event_get != 0);

	static int i = 0;
	i = (i + 1) & 3;
	REAL color[4][4] = {
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1},
		{1, 1, 1, 1}
	};
	pengine->m_render.ClearBackBuffer(color[i][0], color[i][1], color[i][2], color[i][3]);
	pengine->m_render.SwapScreenBuffer();

	usleep(33000);
	WorkThreadContextFast context(thread_func, param);
	pool->PutContext(context, NULL);
	printf("%s Exit\n", __FUNCTION__);
	return NULL;
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
	m_pThreadPool->Initialize(m_nThreadNum, DefaultContextNum);
	return HQRESULT_SUCCESS;
}

RESULT HQEngine::finalize_threadpool() {
	if (m_pThreadPool) {
		m_pThreadPool->Finalize();
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
	m_render.Initialize(&m_window);

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
	WorkThreadContextFast context(HQEngine::thread_func, this);
	m_pThreadPool->PutContext(context, NULL);
	while (m_status != STATE_EXIT) {
		sleep(1);
	}
	return HQRESULT_SUCCESS;
}
