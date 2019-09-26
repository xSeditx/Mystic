#include "ErrorSystem.h"

#include"Window.h" // To check Initialization state of GLFW or other library
//http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505?pgno=3

ErrorSystem::ErrorSystem()
{
}


ErrorSystem::~ErrorSystem()
{
}

bool ErrorSystem::Init()
{
	if (Window::isInitialized() == true)
	{
		Initialized = true;
	}
	else
	{
		Initialized = false;
	}
	return Initialized;
}




