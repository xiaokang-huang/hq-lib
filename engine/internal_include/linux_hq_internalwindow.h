#ifndef LINUXHQINTERNALWINDOW_H_
#define LINUXHQINTERNALWINDOW_H_

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>

class InternalWindow : PUBLIC MemoryAllocatorBase {
PUBLIC:
	InternalWindow();
	~InternalWindow();

	void CreateWindow(HQWindow::Info* windowinfo);
	void DestoryWindow();
	UINT32 GetEvent(HQEventStructure* event_array, UINT32 array_size);

	void LockScreen();
	void UnlockScreen();
	void AttachCurrentThread();
	void DetachThread();

PUBLIC:
	Display*		m_pDisplay;
	Window			m_Window;
	XVisualInfo*	m_pVi;
	Atom			m_close_buttom;
	GLXContext		m_Glcontext;

	HQWindow::Info*	m_windowinfo;
};

#endif//LINUXHQINTERNALWINDOW_H_
