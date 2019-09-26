#include"Application.h"
#include"Common.h"
#include"Window.h"

/*=====================================================================================================
_________________________________ APPLICATION CLASS ___________________________________________________
=======================================================================================================

  Application class Abstracts away functionality of the application itself. It handles some important
  Messages such as Keyboard or Mouse Events. The User can define a handler for all Keyboard or Mouse 
  Events however if they chose not to they can instead define specific Key or Mouse events such as 
  a Handler for only the Left Mouse click or one for Only the Key Repeat. 
  If the User does not define a specific Generic handler one has been supplied which calls the 
  appropritate sub functionality for the input device

=========================================================================================================
USAGE:

=========================================================================================================
*/


#include<string>
Application::Application()
	:
	Position(0,0)
{
	DEBUG_TRACE("Created:");
}
Application::~Application()
{
	DEBUG_TRACE("Deleted:");
}

//=======================================================================================================
//_______________________________ STATE and FLOW HANDLING _______________________________________________
//=======================================================================================================

void Application::Start()
{
	CreateMysticWindow();
	Input = new InputManager(); 

	//--------Gathering information about OpenGL state and Display it -----------------------------------------------
	//  int NumberOfExtensions = 0;
	//  _GL(glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions));
	//  
	//  for (int i = 0; i < NumberOfExtensions; i++) {
	//  	const GLubyte *ccc = glGetStringi(GL_EXTENSIONS, i);
	//  	Print(ccc);
	//  }

	//const GLubyte *extensions = glGetString(GL_EXTENSIONS);
	//Print(extensions);
	Print("");
	Print("OpenGL Version: " << glGetString(GL_VERSION));
	Print("Renderer: " << glGetString(GL_RENDERER));
 	//Print("Current Context: "; Print(glGetCurrentContext()));

	//-------------------------------------------------------------------------------------------------------------
 	OnCreate();
}

void Application::Pause() {}
void Application::End()
{
	OnEnd();
}
void Application::Run() 
{
	Event msg;
	while (ApplicationWindow->Alive())
	{
	BenchMark Performance;
		{
			EventSystem::Instance().PollEvents();
			while (EventSystem::Instance().PeekMSG(msg, 0, 0xFFFFFFFF, REMOVE_MESSAGE))
			{
				EventSystem::Instance().Dispatch(msg); // Line 37 Eventsystem.cpp
			}
			Update();
			Render();
	}
	}
	End();
}

void Application::Update()
{ // Calls User defined Application Update function
	OnUpdate();
}
void Application::Render()
{ // Calls User define Application Render function
	OnRender();
	ApplicationWindow->Sync();
	//ApplicationWindow->CLS(100, 150, 255); // Do not like the duel Dereference every single frame Feels unneeded at best
}

void Application::OnStart()
{
	DEBUG_TRACE(" Default ");
	CreateMysticWindow();
}
/// IDK ABOUT ANY OF THESE
void Application::OnCreate(){ DEBUG_TRACE(" Default ");}
void Application::OnPause() { DEBUG_TRACE(" Default "); }
void Application::OnSleep() { DEBUG_TRACE(" Default "); }
void Application::OnEnd() { DEBUG_TRACE(" Default "); }
void Application::OnRun() { DEBUG_TRACE(" Default "); }
void Application::OnUpdate(){DEBUG_TRACE(" Default ");}
void Application::OnRender(){DEBUG_TRACE(" Default ");}



//=================================================================================================
//________________________________ WINDOW CREATION PROPERTIES _____________________________________
//=================================================================================================
void Application::SetWindowProperties()
{
	DEBUG_TRACE("DEFAULT: ");
	Position = iVec2(0);
	Size = iVec2(640, 480);
	Title = "Default Mystic Application Window";
}
void Application::CreateMysticWindow()
{
	DEBUG_TRACE("DEFAULT: ");
	SetWindowProperties();
	if (Size.x == 0 || Size.y == 0)
	{
		Print(" ERROR: Window Settings have not been set or have been set to NULL.");
		Print(" Define the virtual function with the specifications for your applications Window before calling CreateWindow() ");
		__debugbreak();
	}
	ApplicationWindow = new Window(Position.x, Position.y, Size.x, Size.y, Title);
}
void Application::SetHints()
{// Sets the Properties defined in the Hint Structure for the Application
	DEBUG_TRACE("I Do not Like this, Not one single bit, I want this Hint structure to be in the Window Class however I want to be able to assign properties using the SetWindowProperties. There is a problem because I have to Initialize GLFW before and that is in the Window Struct. I will likely Initialize GLFW in the System Manager or something.")
	glfwWindowHint(GLFW_RESIZABLE, Hints.WindowStyle.RESIZABLE);
	glfwWindowHint(GLFW_VISIBLE, Hints.WindowStyle.VISIBLE);
	glfwWindowHint(GLFW_DECORATED, Hints.WindowStyle.DECORATED);
	// For VR type stuff
	glfwWindowHint(GLFW_STEREO, Hints.Random.STEREO);
	glfwWindowHint(GLFW_SRGB_CAPABLE, Hints.Random.SRGB_CAPABLE);
	// --------- Following Create the Default Framebuffer for the Window being created --------
	// Color Bits
	glfwWindowHint(GLFW_RED_BITS, Hints.FrameBuffer.RED_BITS);
	glfwWindowHint(GLFW_GREEN_BITS, Hints.FrameBuffer.GREEN_BITS);
	glfwWindowHint(GLFW_BLUE_BITS, Hints.FrameBuffer.BLUE_BITS);
	glfwWindowHint(GLFW_ALPHA_BITS, Hints.FrameBuffer.ALPHA_BITS);
	// Depth and Stencil Buffer bits
	glfwWindowHint(GLFW_DEPTH_BITS, Hints.FrameBuffer.DEPTH_BITS);
	glfwWindowHint(GLFW_STENCIL_BITS, Hints.FrameBuffer.STENCIL_BITS);
	// Accumulation Bits
	glfwWindowHint(GLFW_ACCUM_RED_BITS, Hints.FrameBuffer.ACCUM_RED_BITS);
	glfwWindowHint(GLFW_ACCUM_GREEN_BITS, Hints.FrameBuffer.ACCUM_GREEN_BITS);
	glfwWindowHint(GLFW_ACCUM_BLUE_BITS, Hints.FrameBuffer.ACCUM_BLUE_BITS);
	glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, Hints.FrameBuffer.ACCUM_ALPHA_BITS);
	//-------------------------------------------------------------------------------------------
	glfwWindowHint(GLFW_AUX_BUFFERS, Hints.FrameBuffer.AUX_BUFFERS);
	// Anti Aliasing Sampling rates
	glfwWindowHint(GLFW_SAMPLES, Hints.FrameBuffer.SAMPLES);
	glfwWindowHint(GLFW_REFRESH_RATE, Hints.FrameBuffer.REFRESH_RATE);
	// OpenGL Context Creation
	glfwWindowHint(GLFW_CLIENT_API, Hints.Context.CLIENT_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Hints.Context.CONTEXT_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Hints.Context.CONTEXT_VERSION_MINOR);
	glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, Hints.Context.CONTEXT_ROBUSTNESS);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, Hints.Context.FORWARD_COMPAT);
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, Hints.Context.OPENGL_DEBUG_CONTEXT);
	glfwWindowHint(GLFW_OPENGL_PROFILE, Hints.Context.OPENGL_PROFILE);
}
void Application::SetStereo(bool param)
{
	Hints.Random.STEREO = param;
}
void Application::sRGBCapable(bool param)
{
	Hints.Random.SRGB_CAPABLE = param;
}
void Application::ForwardCompatible(bool param)
{
	Hints.Random.OPENGL_FORWARD_COMPAT = param;
}
void Application::DebugContext(bool param)
{
	Hints.Random.OPENGL_DEBUG_CONTEXT = param;
}
void Application::ResizableWindow(bool param)
{
	Hints.WindowStyle.RESIZABLE = param;
}
void Application::VisibleWindow(bool param)
{
	Hints.WindowStyle.VISIBLE = param;
}
void Application::DecoratedWindow(bool param)
{
	Hints.WindowStyle.DECORATED = param;
}
void Application::SetRedBits(unsigned int param)
{
	Hints.FrameBuffer.RED_BITS = param;
}
void Application::SetGreenBits(unsigned int param)
{
	Hints.FrameBuffer.GREEN_BITS = param;
}
void Application::SetBlueBits(unsigned int param)
{
	Hints.FrameBuffer.BLUE_BITS = param;
}
void Application::SetAlphaBits(unsigned int param)
{
	Hints.FrameBuffer.ALPHA_BITS = param;
}
void Application::SetDepthBits(unsigned int param)
{
	Hints.FrameBuffer.DEPTH_BITS = param;
}
void Application::SetStencilBits(unsigned int param)
{
	Hints.FrameBuffer.STENCIL_BITS = param;
}
void Application::SetAccumulatorRedBits(unsigned int param)
{
	Hints.FrameBuffer.ACCUM_RED_BITS = param;
}
void Application::SetAccumulatorGreenBits(unsigned int param)
{
	Hints.FrameBuffer.ACCUM_GREEN_BITS = param;
}
void Application::SetAccumulatorBlueBits(unsigned int param)
{
	Hints.FrameBuffer.ACCUM_BLUE_BITS = param;
}
void Application::SetAccumulatorAlphaBits(unsigned int param)
{
	Hints.FrameBuffer.ACCUM_ALPHA_BITS = param;
}
void Application::SetAuxiliaryBits(unsigned int param)
{
	Hints.FrameBuffer.AUX_BUFFERS = param;
}
void Application::SetNumberOfSamples(unsigned int param)
{
	Hints.FrameBuffer.SAMPLES = param;
}
void Application::SetRefreshRate(unsigned int param)
{
	Hints.FrameBuffer.REFRESH_RATE = param;
}
void Application::UseOpenGLClient(unsigned int param)
{
	Hints.Context.CLIENT_API = param;
}
void Application::UseOpenGLESClient(unsigned int param)
{
	Hints.Context.CONTEXT_VERSION_MAJOR = param;
}
void Application::SetMajorVersion(unsigned int param)
{
	Hints.Context.CONTEXT_VERSION_MINOR = param;
}
void Application::SetMinorVersion(unsigned int param)
{
	Hints.Context.CONTEXT_ROBUSTNESS = param;
}
void Application::UseOpenGLProfile(unsigned int param)
{
	Hints.Context.OPENGL_PROFILE = param;
}
