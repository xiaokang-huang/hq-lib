#ifndef _HQWINDOW_H_
#define _HQWINDOW_H_

#include <hq_type.h>
#include <hq_os.h>
#include <hq_memory_mgr.h>
#include <hq_event_def.h>
#include <rendersystem/hq_rendersystem.h>

class InternalWindow;

class HQWindow : PUBLIC MemoryManagedBase {
PUBLIC:
	struct Info {
		BOOLEAN	fullscreen;
		UINT32	posx;
		UINT32	posy;
		UINT32	width;
		UINT32	height;
		EventCb	event_cb;
	};
PUBLIC:
	HQWindow(const UINT32 nTracerIdx);
	~HQWindow();

	void Create(Info* pinfo = NULL);
	void Destroy();

	void SetFullScreen(BOOLEAN on);
PRIVATE:
	Info			m_windowinfo;
	InternalWindow* m_pInternal;
};

#endif//_HQWINDOW_H_
