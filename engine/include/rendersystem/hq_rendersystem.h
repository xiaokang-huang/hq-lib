#ifndef _HQRENDERSYSTEM_H_
#define _HQRENDERSYSTEM_H_

#include <hq_type.h>
#include <hq_memory_mgr.h>
#include <hq_window.h>

class HQRenderSystem {
PUBLIC:
	HQRenderSystem();

	RESULT Initialize(HQWindow* pwin);
	RESULT ClearBackBuffer(REAL R, REAL G, REAL B, REAL A);
	RESULT SwapScreenBuffer();
PRIVATE:
	HQHANDLE window;
};

#endif//_HQRENDERSYSTEM_H_
