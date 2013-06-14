#include <hq_window.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <hq_thread.h>

#include <linux_hq_internalwindow.h>

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
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	m_pDisplay = XOpenDisplay( NULL );
	Window root = DefaultRootWindow(m_pDisplay);
	m_pVi = glXChooseVisual(m_pDisplay, 0, att);
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
	glXMakeCurrent(m_pDisplay, m_Window, m_Glcontext);

	create_event_thread();
}

void InternalWindow::DestoryWindow() {
	destory_event_thread();

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

void InternalWindow::GetEvent(HQEventStructure* event) {
	XEvent			m_event;
	XNextEvent(m_pDisplay, &m_event);
	printf("Get Event: Type is 0x%x\n", m_event.type);
	switch (m_event.type) {
	case ClientMessage:
		if (m_close_buttom == (Atom)(m_event.xclient.data.l[0])) {
			event->Set(HQEVENTTYPE_SYSTEM_EXIT, 0);
		}
		break;
	}
}

void InternalWindow::create_event_thread() {
	//m_eventthread.Create(event_thread_func, this);
}

void InternalWindow::destory_event_thread() {
	//m_eventthread.Destroy(NULL);
}

void InternalWindow::deal_with_keyevent(XEvent* pevent) {
	int count;
	int buffer_size = 80;
	char buffer[80];
	KeySym keysym;

	count = XLookupString(&(pevent->xkey), buffer, buffer_size, &keysym, NULL);
	printf("\tCount=%d, ASCII=%x, shift=%s\n", count, buffer[0], (pevent->xkey.state&ShiftMask)? "on":"off");
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
		m_pInternal = NULL;
	}
}

void HQWindow::GetEvent(HQEventStructure* event) {
	((InternalWindow*)m_pInternal)->GetEvent(event);
}

HQHANDLE HQWindow::GetHandle() {
	return m_pInternal;
}
