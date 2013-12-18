#include <hq_window.h>
#include <hq_thread.h>
#include <hq_util.h>
#include <linux_hq_internalwindow.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

InternalWindow::InternalWindow() {
	m_pDisplay = NULL;
	m_Window = 0;
	m_close_buttom = 0;
	m_pVi = NULL;
	m_Glcontext = NULL;
	m_windowinfo = NULL;
}

InternalWindow::~InternalWindow() {

}

void InternalWindow::CreateWindow(HQWindow::Info* windowinfo) {
	m_windowinfo = windowinfo;
	GLint att[] = {
		GLX_RGBA,
	    GLX_DEPTH_SIZE, 24,
	    GLX_DOUBLEBUFFER,
	    None
	};
	XInitThreads();

	m_pDisplay = XOpenDisplay( NULL );
	Window root = DefaultRootWindow(m_pDisplay);
	m_pVi = glXChooseVisual(m_pDisplay, 0, att);
	if (m_pVi == NULL) {
		DEBUG_PRINT("VI is NULL\n");
	}
	Colormap cmap = XCreateColormap(m_pDisplay, root, m_pVi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

	m_Window = XCreateWindow(m_pDisplay, root,
			m_windowinfo->posx, m_windowinfo->posy, m_windowinfo->width, m_windowinfo->height,
			0, m_pVi->depth, InputOutput, m_pVi->visual, CWColormap | CWEventMask, &swa);
	XMapWindow(m_pDisplay, m_Window);

	m_close_buttom = XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(m_pDisplay, m_Window, &m_close_buttom, 1);

	m_Glcontext = glXCreateContext(m_pDisplay, m_pVi, NULL, GL_TRUE);
	ASSERT(glXMakeCurrent(m_pDisplay, None, NULL));
}

void InternalWindow::DestoryWindow() {

	glXMakeCurrent(m_pDisplay, None, NULL);
	glXDestroyContext(m_pDisplay, m_Glcontext);
	XDestroyWindow(m_pDisplay, m_Window);
	XFree(m_pVi);
	XCloseDisplay(m_pDisplay);

	m_pDisplay = NULL;
	m_Window = 0;
	m_close_buttom = 0;
	m_pVi = NULL;
	m_Glcontext = NULL;
	m_windowinfo = NULL;
}

UINT32 InternalWindow::GetEvent(HQEventStructure* event_array, UINT32 array_size) {
	XEvent			m_event;
	UINT32 num = (UINT32)XEventsQueued(m_pDisplay, QueuedAlready);
	if (num >= array_size)	num = array_size;
	UINT32 i;
	for (i = 0; i < num; ++i) {
		XNextEvent(m_pDisplay, &m_event);
		switch (m_event.type) {
		case Expose:
			XWindowAttributes       gwa;
			XGetWindowAttributes(m_pDisplay, m_Window, &gwa);
			m_windowinfo->width = gwa.width;
			m_windowinfo->height = gwa.height;
			break;
		case ClientMessage:
			if (m_close_buttom == (Atom)(m_event.xclient.data.l[0])) {
				event_array[i ++].Set(HQEVENTTYPE_SYSTEM_EXIT, 0);
			}
			break;
		}
	}
	return i;
}

void InternalWindow::AttachCurrentThread() {
	ASSERT(glXMakeCurrent(m_pDisplay, m_Window, m_Glcontext));
	//glEnable(GL_DEPTH_TEST);
	//glViewport(0, 0, m_windowinfo->width, m_windowinfo->height);
}

void InternalWindow::DetachThread() {
	glXMakeCurrent(m_pDisplay, None, NULL);
}

void InternalWindow::LockScreen() {
	//XLockDisplay(m_pDisplay);
}
void InternalWindow::UnlockScreen() {
	//XUnlockDisplay(m_pDisplay);
}

/* ======================================================================
 * HQWindow
 * */
HQWindow::HQWindow(const UINT32 nTracerIdx) : MemoryManagedBase(nTracerIdx), m_pInternal(0) {
	m_windowinfo.fullscreen = FALSE;
	m_windowinfo.posx = 10;
	m_windowinfo.posy = 10;
	m_windowinfo.width = 800;
	m_windowinfo.height = 600;
	m_windowinfo.event_cb = NULL;
}

HQWindow::~HQWindow() {
	Destroy();
}

void HQWindow::Create(HQWindow::Info* pinfo) {
	if (pinfo) {
		m_windowinfo = *pinfo;
	}
	m_pInternal = (HQHANDLE)Managed_New(InternalWindow, ());
	((InternalWindow*)m_pInternal)->CreateWindow(&m_windowinfo);
}

void HQWindow::Destroy() {
	if (m_pInternal) {
		((InternalWindow*)m_pInternal)->DestoryWindow();
		Managed_Delete((InternalWindow*)m_pInternal);
		m_pInternal = 0;
	}
}

UINT32 HQWindow::GetEvent(HQEventStructure* event_array, UINT32 array_size) {
	return ((InternalWindow*)m_pInternal)->GetEvent(event_array, array_size);
}

HQHANDLE HQWindow::GetHandle() {
	return m_pInternal;
}

void HQWindow::AttachCurrentThread() {
	((InternalWindow*)m_pInternal)->AttachCurrentThread();
}

void HQWindow::DetachThread() {
	((InternalWindow*)m_pInternal)->DetachThread();
}
