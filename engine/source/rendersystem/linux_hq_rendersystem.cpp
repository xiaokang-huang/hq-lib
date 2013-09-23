#include <rendersystem/hq_rendersystem.h>
#include <hq_errorcode.h>
#include <linux_hq_internalwindow.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#define FUNC_ENTER() //DEBUG_PRINT("%s Enter\n", __FUNCTION__)
#define FUNC_EXIT()	 //DEBUG_PRINT("%s Exit\n", __FUNCTION__)

HQRenderSystem::HQRenderSystem() : window(0) {
}

RESULT HQRenderSystem::Initialize(HQWindow* pwin) {
	window = pwin->GetHandle();
	return HQRESULT_SUCCESS;
}

RESULT HQRenderSystem::ClearBackBuffer(REAL R, REAL G, REAL B, REAL A) {
	FUNC_ENTER();
	glClearColor(R, G, B, A);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	glBegin(GL_QUADS);
	glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
	glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
	glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
	glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
	glEnd();
	FUNC_EXIT();
	return HQRESULT_SUCCESS;
}

RESULT HQRenderSystem::SwapScreenBuffer() {
	FUNC_ENTER();
	InternalWindow* internal_win = (InternalWindow*)window;
	glXSwapBuffers(internal_win->m_pDisplay, internal_win->m_Window);
	FUNC_EXIT();
	return HQRESULT_SUCCESS;
}
