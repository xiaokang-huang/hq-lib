#include <hq_engine.h>
#include <hq_errorcode.h>
#include <hq_thread_pool.h>
#include <hq_util.h>
#include <hq_event_def.h>

#define FUNC_ENTER()	//DEBUG_PRINT("Enter <%s>\n", __FUNCTION__);
#define FUNC_END()		//DEBUG_PRINT("End   <%s>\n", __FUNCTION__);

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
	//HQEngine* pengine = (HQEngine*)usr_ptr;
	if (type == HQEVENTTYPE_SYSTEM_EXIT) {
	}
}

void* HQEngine::swap_back_nodes(HQThreadPoolFast* pool, void* param) {
	FUNC_ENTER();
	{
		HQEngine* pengine = (HQEngine*)param;
		hq_msleep(33);
		if (pengine->m_status == STATUS_EXIT) {
			pengine->m_event.SignalAll();
		} else {
			WorkThreadContextFast context1(HQEngine::handle_event, param);
			WorkThreadContextFast context2(HQEngine::render_front_nodes, param);
			pool->PutContext(context1, NULL);
			pool->PutContext(context2, NULL);
		}	
	}
	FUNC_END();
	return NULL;
}

void* HQEngine::update_back_nodes(HQThreadPoolFast* pool, void* param) {
	FUNC_ENTER();
	WorkThreadContextFast context(HQEngine::swap_back_nodes, param);
	pool->PutContext(context, NULL);
	FUNC_END();
	return NULL;
}

void* HQEngine::render_front_nodes(HQThreadPoolFast* pool, void* param) {
	FUNC_ENTER();
	WorkThreadContextFast context(HQEngine::swap_framebuffer, param);
	pool->PutContext(context, NULL);
	FUNC_END();
	return NULL;
}

void* HQEngine::swap_framebuffer(HQThreadPoolFast* pool, void* param) {
	FUNC_ENTER();
	HQEngine* pengine = (HQEngine*)param;
	pengine->m_window.AttachCurrentThread();
	pengine->m_render.ClearBackBuffer(1, 0, 0, 0);
	pengine->m_render.SwapScreenBuffer();
	pengine->m_window.DetachThread();
	FUNC_END();
	return NULL;
}

void* HQEngine::handle_event(HQThreadPoolFast* pool, void* param) {
	FUNC_ENTER();
	{
		HQEngine* pengine = (HQEngine*)param;
		HQEventStructure event;
		UINT32 event_get;
		do {
			event_get = pengine->m_window.GetEvent(&event, 1);		
			if (event._type == HQEVENTTYPE_SYSTEM_EXIT) {
				pengine->m_status = STATUS_EXIT;
			}
		} while (event_get != 0);

		WorkThreadContextFast context(HQEngine::update_back_nodes, param);
		pool->PutContext(context, NULL);
	}
	FUNC_END();
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
						m_window(INDEX_COMMON_MEMORY) {

}

HQEngine::~HQEngine() {
	if (m_nThreadNum) {
		Finalize();
	}
}

RESULT HQEngine::Initialize() {
	initialize_window();
	m_render.Initialize(&m_window);
	initialize_threadpool();
	m_event.Create();

	m_status = STATUS_READY;

	return HQRESULT_SUCCESS;
}

RESULT HQEngine::Finalize() {
	m_event.Destroy();
	finalize_threadpool();
	finalize_window();

	return HQRESULT_SUCCESS;
}

RESULT HQEngine::Start() {
	WorkThreadContextFast context(HQEngine::handle_event, this);
	m_pThreadPool->PutContext(context, NULL);
	m_event.Wait();
	return HQRESULT_SUCCESS;
}
