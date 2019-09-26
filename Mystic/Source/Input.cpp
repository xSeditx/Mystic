#include"Input.h"
/// I HATE THE EXISTANCE OF THIS ENTIRE MODULE AND WISH FOR A BETTER WAY BUT GLFW LIMITS THAT
Key_Board Keyboard;
MouseProperties Mouse;

#pragma message("Compiling Window CPP InputManager Class")

InputManager::InputManager()
{
	DEBUG_TRACE("Creating Input Interface:");
	// _____________ SETS __________________
	GLFWwindow *window = Window::RenderingContext();
	glfwSetErrorCallback(Error_callback);                          // Callback when Error Encountered
	glfwSetKeyCallback(window, KeyBoard_Callback);	               // Callback for KeyPress
	glfwSetDropCallback(window, DropFile_callback);	               // Callback Allows Files to be dropped into the window and get the path read
	glfwSetMouseButtonCallback(window, Mouse_Button_Callback);     // Callback for Mouse Button Click
	glfwSetCursorPosCallback(window, MouseMove_Callback);          // Callback to return Mouse Position			 
	glfwSetWindowPosCallback(window, Window_Move_Callback);        // Callback to return Window Position
	glfwSetWindowSizeCallback(window, Resize_Callback);            // Callback when Window is Resized
	glfwSetWindowCloseCallback(window, Window_close_callback);     // Callback when Closed
}


void  InputManager::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Event msg(EVENT_WINDOWSCROLL);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void  InputManager::Error_callback(int, const char* description)
{
	Print("GLFW Error " << description);
	Event msg(EVENT_ERROR);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void  InputManager::Resize_Callback(GLFWwindow *window, int xpos, int ypos)
{
	Event msg(EVENT_WINDOW_RESIZE);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void  InputManager::Window_close_callback(GLFWwindow *window)
{
	Event msg(EVENT_WINDOW_CLOSE);
	EventSystem::Instance().PostMSG(msg);
}

 
MYSTIC void 
InputManager::KeyBoard_Callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	Keyboard.Key = key;
	Keyboard.Scancode = scancode;
	Keyboard.Modifications = mods;
	Keyboard.Action = action;
	
	Event msg;
	switch (action)
	{
	case MSG_KEY_PRESS:
		msg = Event(EVENT_KEYDOWN);
		msg.data.Keyboard.KeyCode = key;
		msg.data.Keyboard.ScanCode = scancode;
		msg.data.Keyboard.Action = action;
		msg.data.Keyboard.Mods = mods;
		EventSystem::Instance().PostMSG(msg);
		break;

	case MSG_KEY_RELEASE:
		msg = Event(EVENT_KEYUP);
		msg.data.Keyboard.KeyCode = key;
		msg.data.Keyboard.ScanCode = scancode;
		msg.data.Keyboard.Action = action;
		msg.data.Keyboard.Mods = mods;
		EventSystem::Instance().PostMSG(msg);
		break;

	case MSG_KEY_REPEAT:
		msg = Event(EVENT_KEYDOWN);
		///.msg = Event(EVENT_KEYREPEAT); THIS IS WHAT IS SUPPOSE TO BE HERE FIX LATER
		msg.data.Keyboard.KeyCode = key;
		msg.data.Keyboard.ScanCode = scancode;
		msg.data.Keyboard.Action = action;
		msg.data.Keyboard.Mods = mods;
		EventSystem::Instance().PostMSG(msg);
		break;

	case MSG_KEY_UNKNOWN:
		msg = Event(EVENT_KEYUNKNOWN);
		msg.data.Keyboard.KeyCode = key;
		msg.data.Keyboard.ScanCode = scancode;
		msg.data.Keyboard.Action = action;
		msg.data.Keyboard.Mods = mods;
		EventSystem::Instance().PostMSG(msg);
		break;
	}
}


MYSTIC void 
InputManager::Mouse_Button_Callback(GLFWwindow *window, int button, int action, int mods)
{	 
	Mouse.Action = action;
	Mouse.Modifications = mods;
 	Event msg;
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		if (action == MOUSE_BUTTONUP)
		{
			msg = Event(EVENT_MOUSEUP_LEFT);
			msg.data.MouseButton.Buttons.Left = false;
			msg.data.MouseButton.Action = action;
			msg.data.MouseButton.Modification = mods;
			Mouse.Button.Left = false;

			EventSystem::Instance().PostMSG(msg);
			break;

		}
		else
		{
			msg = Event(EVENT_MOUSEDOWN_LEFT);
			msg.data.MouseButton.Buttons.Left = true;
			msg.data.MouseButton.Action = action;
			msg.data.MouseButton.Modification = mods;
			Mouse.Button.Left = true;
			EventSystem::Instance().PostMSG(msg);
			break;

		}
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		if (action == MOUSE_BUTTONUP)
		{
			msg = Event(EVENT_MOUSEUP_RIGHT);
			msg.data.MouseButton.Buttons.Right = false;
			msg.data.MouseButton.Action = action;
			msg.data.MouseButton.Modification = mods;
			Mouse.Button.Right = false;
			EventSystem::Instance().PostMSG(msg);
			break;
		}
		else
		{
			msg = Event(EVENT_MOUSEDOWN_RIGHT);
			msg.data.MouseButton.Buttons.Right = true;
			msg.data.MouseButton.Action = action;
			msg.data.MouseButton.Modification = mods;
			Mouse.Button.Right = true;
			EventSystem::Instance().PostMSG(msg);
			break;

		}
	}


	case GLFW_MOUSE_BUTTON_MIDDLE:
	{
		if (action == MOUSE_BUTTONUP)
		{
			msg = Event(EVENT_MOUSEUP_MIDDLE);
			msg.data.MouseButton.Buttons.Middle = false;
			msg.data.MouseButton.Action = action;
			msg.data.MouseButton.Modification = mods;
			EventSystem::Instance().PostMSG(msg);
			Mouse.Button.Center = false;
			break;

		}
		else
		{
			msg = Event(EVENT_MOUSEDOWN_MIDDLE);
			msg.data.MouseButton.Buttons.Middle = true;
			msg.data.MouseButton.Action = action;
			msg.data.MouseButton.Modification = mods;
			Mouse.Button.Center = true;
			EventSystem::Instance().PostMSG(msg);
			break;

		}
	}
	}
}

MYSTIC void  InputManager::MouseMove_Callback(GLFWwindow *window, double xpos, double ypos)
{
	Event msg(EVENT_MOUSEMOVE);
	msg.data.Mouse.x = xpos;
	msg.data.Mouse.y = ypos;
	msg.data.Mouse.relX = xpos - Mouse.Relative.x; 
	msg.data.Mouse.relY = ypos - Mouse.Relative.y; 
	Mouse.Relative.x = xpos;
	Mouse.Relative.y = ypos;

	DEBUG_TRACE("We have no way to get the Mouse Buttons. Delete it or fix it or ELSE!")

	Mouse.Position = Vec2(xpos, ypos);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void  InputManager::DropFile_callback(GLFWwindow *window, int, const char**)
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_DRAGDROP);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void  InputManager::Window_Move_Callback(GLFWwindow *window, int, int)
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_WINDOW_MOVE);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void  InputManager::SetClipBoard(char*)
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_CLIPBOARD);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC const char * InputManager::GetClipBoard()
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_CLIPBOARD);
	EventSystem::Instance().PostMSG(msg);
	return "   ";
}
MYSTIC void InputManager::Charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods)
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_MODTEXTINPUT);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void InputManager::Character_callback(GLFWwindow* window, unsigned int codepoint)
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_TEXTINPUT);
	EventSystem::Instance().PostMSG(msg);
}
MYSTIC void InputManager::Minimize_Window_callback(GLFWwindow *, int)
{
	DEBUG_TRACE("NEEDS WORK");
	Event msg(EVENT_WINDOW_MINIMIZE);
	EventSystem::Instance().PostMSG(msg);
}