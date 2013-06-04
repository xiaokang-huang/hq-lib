#include <hq_window.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>

class InternalWindow : PUBLIC MemoryAllocatorBase {
PUBLIC:
	InternalWindow();
	~InternalWindow();

	void CreateWindow();
	void DestoryWindow();

PUBLIC:
	Display*		m_pDisplay;
	Window			m_Window;
	XVisualInfo*	m_pVi;
	Atom			m_close_buttom;
	GLXContext		m_Glcontext;
};

InternalWindow::InternalWindow() {
	m_pDisplay = NULL;
	m_Window = 0;
	m_close_buttom = 0;
	m_pVi = NULL;
	m_Glcontext = NULL;
}

InternalWindow::~InternalWindow() {

}

void InternalWindow::CreateWindow() {
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

	m_pDisplay = XOpenDisplay( NULL );
	Window root = DefaultRootWindow(m_pDisplay);
	m_pVi = glXChooseVisual(m_pDisplay, 0, att);
	Colormap cmap = XCreateColormap(m_pDisplay, root, m_pVi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask = ExposureMask | KeyPressMask;

	m_Window = XCreateWindow(m_pDisplay, root, 0, 0, 600, 600, 0, m_pVi->depth, InputOutput, m_pVi->visual, CWColormap | CWEventMask, &swa);
	XMapWindow(m_pDisplay, m_Window);

	m_close_buttom = XInternAtom(m_pDisplay, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(m_pDisplay, m_Window, &m_close_buttom, 1);

	m_Glcontext = glXCreateContext(m_pDisplay, m_pVi, NULL, GL_TRUE);
	glXMakeCurrent(m_pDisplay, m_Window, m_Glcontext);
}

void InternalWindow::DestoryWindow() {
	glXMakeCurrent(m_pDisplay, None, NULL);
	glXDestroyContext(m_pDisplay, m_Glcontext);
	XDestroyWindow(m_pDisplay, m_Window);
	XFree(m_pVi);
	XCloseDisplay(m_pDisplay);

	m_pDisplay = NULL;
	//m_Window = NULL;
	m_close_buttom = 0;
	m_pVi = NULL;
	m_Glcontext = NULL;
}

/* ======================================================================
 * HQWindow
 * */
HQWindow::HQWindow(const UINT32 nTracerIdx) : MemoryManagedBase(nTracerIdx), m_pInternal(NULL) {
	m_windowinfo.fullscreen = FALSE;
	m_windowinfo.posx = 10;
	m_windowinfo.posy = 10;
	m_windowinfo.width = 800;
	m_windowinfo.height = 600;
}

HQWindow::~HQWindow() {
	Destroy();
}

void HQWindow::Create(HQWindow::Info* pinfo) {
	if (pinfo) {
		m_windowinfo = *pinfo;
	}
	m_pInternal = Managed_New(InternalWindow, ());
	m_pInternal->CreateWindow();
}

void HQWindow::Destroy() {
	if (m_pInternal) {
		m_pInternal->DestoryWindow();
		Managed_Delete(m_pInternal);
		m_pInternal = NULL;
	}
}
