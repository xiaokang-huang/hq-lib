#include <rendersystem/hq_rendersystem.h>
#include <hq_errorcode.h>
#include <linux_hq_internalwindow.h>

#include <GL/gl.h>
#include <GL/glx.h>

HQRenderSystem::HQRenderSystem() : window(NULL) {
}

RESULT HQRenderSystem::Initialize(HQWindow* pwin) {
	window = pwin->GetHandle();
	return HQRESULT_SUCCESS;
}

RESULT HQRenderSystem::ClearBackBuffer(REAL R, REAL G, REAL B, REAL A) {
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return HQRESULT_SUCCESS;
}

RESULT HQRenderSystem::SwapScreenBuffer() {
	InternalWindow* internal_win = (InternalWindow*)window;
	glXSwapBuffers(internal_win->m_pDisplay, internal_win->m_Window);
	return HQRESULT_SUCCESS;
}
