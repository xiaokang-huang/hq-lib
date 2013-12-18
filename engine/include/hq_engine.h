#ifndef _HQENGINE_H_
#define _HQENGINE_H_

#include <hq_type.h>
#include <hq_threadpool_fast.h>
#include <hq_memory_mgr.h>
#include <hq_window.h>
#include <rendersystem/hq_rendersystem.h>

class HQEngine : PUBLIC MemoryManagedBase {
	class EngineEventCallBack : PUBLIC EventCallBackBase {
		virtual void DoCallBack(HQEventType type, HQEventData data);
	};
	typedef enum {
		STATUS_ERROR = -1,
		STATUS_RUNNING,
		STATUS_READY,
		STATUS_EXIT,
	} EngineStatus;

PUBLIC:
	HQEngine();
	~HQEngine();

	RESULT Initialize();
	RESULT Finalize();
	RESULT Start();
PRIVATE:
	RESULT initialize_threadpool();
	RESULT finalize_threadpool();
	RESULT initialize_window();
	RESULT finalize_window();

	static void* thread_func(HQThreadPoolFast* pool, void* param);

	static void* swap_back_nodes(HQThreadPoolFast* pool, void* param);
	static void* update_back_nodes(HQThreadPoolFast* pool, void* param);
	static void* render_front_nodes(HQThreadPoolFast* pool, void* param);
	static void* swap_framebuffer(HQThreadPoolFast* pool, void* param);
	static void* handle_event(HQThreadPoolFast* pool, void* param);
PRIVATE:
	HQThreadPoolFast*	m_pThreadPool;
	UINT32				m_nThreadNum;

	HQWindow			m_window;
	HQRenderSystem		m_render;

	EngineStatus		m_status;
	HQThreadEvent		m_event;
};

#endif//_HQENGINE_H_
