#include "Window.h"

#pragma message("Compiling Windowing System for GLFW. Including GLFW, GLEW, and OpenGL32 Libraries")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shell32.lib")

// Possibly remove this in favor of a Singleton structured Window class
Window *Window::ActiveInstance; // static The Active Input and Render Target

Window::Window(int x, int y, int w, int h, std::string title)
	:
	Position(iVec2(x,y)),
	Size(iVec2(w,h)),
	Title(title)
{
	INIT_ASSERT(glfwInit(), "Initializing GLFW");
	glfwWindowHint(GLFW_SAMPLES, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE,   GLFW_OPENGL_CORE_PROFILE);

	if (!(glContext = glfwCreateWindow(w, h, title.c_str(), NULL, NULL)))
	{// Create GLFW Window, Asign its value to glContext, and test && respond to its succcess
		glfwTerminate();  // Kill if Creation Failed
		__debugbreak();
		exit(EXIT_FAILURE);
	}
	SetasActiveWindow();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{// 6/1/2019 Move to Glad instead of GLEW
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		__debugbreak();
	}
	gladLoadGL();

    glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
///	glfwSwapInterval(60);
	// --------------- Initialize Matrices ----------------------------------------------------
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
 
	glEnable(GL_TEXTURE_2D);// I do not believe this has any effect
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	//glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_MULTISAMPLE);
	//-------------------------------------------------------------------------------------------      
	glViewport(0, 0, w, h);
	FramesPerSecond.Set(10060);
}

// ======================================== STATIC METHODS ===================================================
//------------------------------------------------------------------------------------------------------------

Window* Window::ActiveWindow()
{ // Returns the current Render and Input target for the Window
	return ActiveInstance;
};
GLFWwindow* Window::RenderingContext()
{// Return the Native Rendering Window for whatever windowing system is in use GLFW in this case
	return ActiveInstance->glContext;
}
void Window::SetasActiveWindow()
{// Sets the Windows Context as Active and sets the Global Window pointer to Active.
	ActiveInstance = this;
	glfwMakeContextCurrent(glContext);
}
void Window::SetWindowTitle(std::string _text)
{// Duh
	Title = _text;
}
Window const *GetWindowHandle(GLFWwindow *win)
{// Pulls the Set user pointer out of the GLFW Window Context
	return (Window*)glfwGetWindowUserPointer(win);
}
void const Window::HideMouseCursor() const
{
	glfwSetInputMode(Window::RenderingContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void const Window::ShowMouseCursor() const
{
	glfwSetInputMode(Window::RenderingContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
bool const Window::Alive() const
{// To check if a Window is currently Active and has been all Libraries have been Initialized
	return !glfwWindowShouldClose(glContext);
}



void Window::SetPosition(int x, int y)
{
	glfwSetWindowPos(glContext, x, y);
}
iVec2 Window::GetPosition()
{
	int X, Y;
	glfwGetWindowPos(glContext, &X, &Y);
	return iVec2(X, Y);
}
void Window::CLS()
{
	_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));// GL_COLOR_BUFFER_BIT
}
void Window::CLS(int r, int g, int b)
{
    _GL(glClearColor(GL_Color(r), GL_Color(g), GL_Color(b), 1.0));
    _GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Window::Sync()
{/*~~~~~~~~~~~~~~~~~ Flips the backbuffer and updates the Frame Timer ~~~~~~~~~~~~~~~~~~~*/
	FramesPerSecond.Update();
	glfwSwapBuffers(glContext);
}

//------------------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& stream, Window& win)
{
	stream << "Title: " << win.Title << "  FSP: " << win.FramesPerSecond.Get() << "OpenGL Context: " << win.RenderingContext();
	return stream;
}