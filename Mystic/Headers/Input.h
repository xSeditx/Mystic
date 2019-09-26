#pragma once
#include"Common.h"
#include"EventSystem.h"
#include"Window.h"
// REALLY REALLY HATE THIS SETUP AND DO NOT LIKE GLFW FOR IT.
// MUCH RATHER JUST MANAGE MY INPUT AND EVENTS FROM A TYPICAL EVENTLOOP INSTEAD OF HAVING GLFW
// CREATE THREADS FOR ME WHICH WILL INTERFEAR WITH MY PERFOMANCE LATER ON.
struct MouseProperties
{
	Vec2 Position = Vec2(0);
	Vec2 Relative = Vec2(0);
	Vec2 RealPosition = Vec2(0);
	Vec2 Velocity = Vec2(0);
	Vec2 Offset = Vec2(0); // NEEDED TO FIX SCALING ISSUES FOR RELATIVE MOUSE POSITION OF WINDOW SIZE IS CHANGES
	int
		Action,
		Modifications;

	struct
	{
		bool Left, Right, Center, X1, X2;
		short Clicks;
	}Button;

	inline void Hide() { Window::ActiveWindow()->HideMouseCursor(); }
	inline void Show() { Window::ActiveWindow()->ShowMouseCursor(); }
};


struct Key_Board
{
	int Key,
		Scancode,
		Action,
		Modifications;
	int KeyStates[350];
	inline bool IsKeyPressed(int keycode)
	{
		return (KeyStates[keycode] != 0);
	}
};

class InputManager
{
	// Why did I change this from Singleton. The main Managers and Systems should be singletons. There is absolutely no reason for them not to be.
	// I have a Single Instance of them and Do not wish to have any more kicking around. Not only that but unlike the other singletons I have created 
	// Every god damn function in this mess of a class has to be static due to GLFW poor way of handling messages which highly complicates creating an 
	// Observer pattern.
	// Yeah its not 6/2/2019 and I still agree with the above statement...
	/// Singleton design pattern. 
public: 
	InputManager();

	static void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void Error_callback(int, const char*);
	static void Resize_Callback(GLFWwindow *window, int, int);
	static void Window_close_callback(GLFWwindow *window);
	static void KeyBoard_Callback(GLFWwindow *window, int, int, int, int);
	static void Mouse_Button_Callback(GLFWwindow *window, int, int, int);
	static void MouseMove_Callback(GLFWwindow *window, double, double);
	static void DropFile_callback(GLFWwindow *window, int, const char**);
	static void Window_Move_Callback(GLFWwindow *window, int, int);
	static void SetClipBoard(char*);
	static void Charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods);
	static void Character_callback(GLFWwindow* window, unsigned int codepoint);
	static void WindowClose_callback(GLFWwindow *);
	static void Minimize_Window_callback(GLFWwindow *, int);
	static const char *GetClipBoard();


	static void PollNativeEvents() 
	{
		glfwPollEvents();
	}
};

