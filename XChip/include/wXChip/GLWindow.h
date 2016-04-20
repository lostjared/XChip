#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#include<wx/wx.h>
#include<wx/glcanvas.h>
#ifdef __WXMAC__
#include<OpenGL/gl.h>
#include<OpenGL/glu.h>
#else
#include<GL/gl.h>
#incldue<GL/glu.h>
#endif


class GLCanvas : public wxGLCanvas
{
	wxGLContext *_context;
public:
	GLCanvas(wxFrame* parent, int* args);
	virtual ~GLCanvas();
	
	void render(wxPaintEvent& evt);
	void resized(wxSizeEvent& evt);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	
	
	DECLARE_EVENT_TABLE()
};

class GLWindow : public wxFrame
{
public:
	
	GLCanvas *_canvas;
	
	GLWindow(const std::string title, const wxPoint &p, const wxSize &size);
	
	DECLARE_EVENT_TABLE()
};












#endif