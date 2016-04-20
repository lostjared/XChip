#include<wXChip/GLWindow.h>


BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
EVT_SIZE(GLCanvas::resized)
EVT_KEY_DOWN(GLCanvas::keyPressed)
EVT_KEY_UP(GLCanvas::keyReleased)
EVT_PAINT(GLCanvas::render)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GLWindow, wxFrame)
END_EVENT_TABLE()

GLWindow::GLWindow(const std::string title, const wxPoint &pos, const wxSize &size) :
wxFrame(NULL, wxID_ANY, title, pos, size, wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 32, 0};
 
	_canvas = new GLCanvas( (wxFrame*) this, args);
	sizer->Add(_canvas, 1, wxEXPAND);
 
	SetSizer(sizer);
	SetAutoLayout(true);
}


GLCanvas::GLCanvas(wxFrame* parent, int* args) :
wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxSize(320, 240), wxFULL_REPAINT_ON_RESIZE)
{
	_context = new wxGLContext(this);
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

GLCanvas::~GLCanvas()
{
	delete _context;
}

void GLCanvas::render(wxPaintEvent& evt)
{
	if(!IsShown()) return;
	
	wxGLCanvas::SetCurrent(*_context);
	wxPaintDC(this);
 
	glClearColor(0, 0, 0 , 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glFlush();
	SwapBuffers();
	
}
void GLCanvas::resized(wxSizeEvent& evt)
{
	Refresh();
	
}
void GLCanvas::keyPressed(wxKeyEvent& event)
{
	
}
void GLCanvas::keyReleased(wxKeyEvent& event)
{
	
	
}