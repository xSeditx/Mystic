#pragma once
// CPP Standard Library Includes
#include<string>


// Project Includes
#include"Common.h"		
#include"Timer.h"
#include"OpenGL.h"
//----------------------------------------------------------------------------------------------------

#define INIT_ASSERT(x, str)  	if (!(x))\
{\
DEBUG_TRACE(#x);\
Print(#str);\
glfwTerminate();\
exit(EXIT_FAILURE);\
}\
 
/*
Hint are assigned in the derived class prior to the creation of the Applications Window. 
Defaults are set in the case that the user does not specify their own Hint for a given element
*/
struct HintsStruct
{
	struct HardConstraints
	{
		/// MOVED TO CONTEXT, Double check this MOVED BACK.... IDK WHAT I WANNA DO
		//bool OPENGL_FORWARD_COMPAT = 0;
		//bool OPENGL_DEBUG_CONTEXT = 0;//	GL_TRUE or GL_FALSE
		bool OPENGL_FORWARD_COMPAT = 0;//	GL_TRUE or GL_FALSE
		bool OPENGL_DEBUG_CONTEXT = 0;//	GL_TRUE or GL_FALSE


		bool STEREO = 0;//	GL_TRUE or GL_FALSE
		bool SRGB_CAPABLE = 0;//	GL_TRUE or GL_FALSE
	}Random;// Dude WTF do I want to call this, Not that thats for damn sure....

	struct WindowHints
	{
		bool RESIZABLE = 1;//	GL_TRUE or GL_FALSE
		bool VISIBLE = 1;//	GL_TRUE or GL_FALSE
		bool DECORATED = 1;//	GL_TRUE or GL_FALSE
	}WindowStyle;

	struct FrameBufferHints
	{
		unsigned int RED_BITS = 8;//	0 to INT_MAX
		unsigned int GREEN_BITS = 8;//	0 to INT_MAX
		unsigned int BLUE_BITS = 8;//	0 to INT_MAX
		unsigned int ALPHA_BITS = 8;//	0 to INT_MAX
		unsigned int DEPTH_BITS = 24;//	0 to INT_MAX
		unsigned int STENCIL_BITS = 8;//	0 to INT_MAX
		unsigned int ACCUM_RED_BITS = 0;//	0 to INT_MAX
		unsigned int ACCUM_GREEN_BITS = 0;//	0 to INT_MAX
		unsigned int ACCUM_BLUE_BITS = 0;//	0 to INT_MAX
		unsigned int ACCUM_ALPHA_BITS = 0;//	0 to INT_MAX
		unsigned int AUX_BUFFERS = 0;//	0 to INT_MAX
		unsigned int SAMPLES = 4;//	0 to INT_MAX
		unsigned int REFRESH_RATE = 0;//	0 to INT_MAX
	}FrameBuffer;

	struct ContextHints
	{
		unsigned int CLIENT_API = 0x00030001;//	GLFW_OPENGL_API or GLFW_OPENGL_ES_API
		unsigned int CONTEXT_VERSION_MAJOR = 1;//	Any valid major version number of the chosen client API
		unsigned int CONTEXT_VERSION_MINOR = 0;//	Any valid minor version number of the chosen client API
		unsigned int CONTEXT_ROBUSTNESS = 0;//	GLFW_NO_ROBUSTNESS, GLFW_NO_RESET_NOTIFICATION or GLFW_LOSE_CONTEXT_ON_RESET
		unsigned int OPENGL_PROFILE = 0;//	GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE or GLFW_OPENGL_CORE_PROFILE
	}Context;
};




class MYSTIC Window
{
public:
	Window(int x, int y, int w, int h, std::string title);

	iVec2
		Position,
		Size;
	std::string Title;

	// Helper Functions
	void CLS();
	void CLS(int r, int g, int b);
	void Sync();
	void SetPosition(int x, int y);
	void SetWindowTitle(std::string text);
	void SetasActiveWindow();
	void const HideMouseCursor() const;
	void const ShowMouseCursor() const;

	iVec2 GetPosition();

	bool const Alive() const;
 
	//---------------------------------------------------------------

	// STATIC
	static Window *ActiveWindow();
	static GLFWwindow* RenderingContext();
	static bool isInitialized() { return (ActiveInstance->glContext == 0); }

	FrameRate FramesPerSecond;

private:
	GLFWwindow *glContext;
	HintsStruct Hints;

	static Window *ActiveInstance;
};

std::ostream& operator<<(std::ostream& stream, Window& win);

  Window const *GetWindowHandle(GLFWwindow*);


/// TODO: Place this Framerate Manager into its own system
#include<chrono>
struct FrameLimiter
{
	float Rate;
	void Set(float rate)
	{
		Coefficient = 1000.0 / rate;
	}
	float Get() 
	{
		return Rate;
	}

	float PreviousTime = 0.0f;
	float Coefficient = 0.0f;
	float CurrentTime = 0.0f;
	float Tracker = 0.0f;
	float LastFrame = 0.0;

	unsigned int Accumulator = 0.0f;

	void SetWindowHints(const HintsStruct& hints);
};





/// NOTES: GLFW SOURCE for Win32 Window
//https://github.com/glfw/glfw/blob/master/src/win32_window.c


/*
Listener
{
Handler();
}
*/
// 
// 
// "private: __cdecl EventSystem::EventSystem(void)"
// (? ? 0EventSystem@@AEAA@XZ) referenced in function
// "public: class EventSystem & __cdecl EventSystem::Instance(void)" 
// (? Instance@EventSystem@@QEAAAEAV1@XZ)
// 
// 		"public: static class EventSystem * InputManager::EventManager" 
// (? EventManager@InputManager@@2PEAVEventSystem@@EA)
// "public: static struct InputManager::MouseProperties InputManager::Mouse"
// (? Mouse@InputManager@@2UMouseProperties@1@A)
// "public: static struct InputManager::Key_Board InputManager::Keyboard"
// (? Keyboard@InputManager@@2UKey_Board@1@A)
