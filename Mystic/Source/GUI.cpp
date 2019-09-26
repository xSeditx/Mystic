#include"Gui.h"

#include"EventSystem.h"
#include"imgui.h"
#include"OpenGL.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#include"imgui_impl_opengl3.h"
#include"imgui_impl_glfw.h"

#include"Window.h"
 
GUILayer::GUILayer(std::string _name)
	:
	Name(_name)
{}



ImGUILayer::ImGUILayer()
	:
	GUILayer("ImGUILayer")
{}

void ImGUILayer::OnAttach()
{// Should be the GUI itself perhaps
	// Either way this entire system was just hacked in until I knew more about What ImGUI is and what I am going to need
	// To properly abstract it for my engine.
	// I read through the various GLFW implementations and from what I can gather this is the bare minimum one should need inorder to get it up and running.
	// As such it runs find and creates an ImGUI window but I can not render my OpenGL at the same time the ImGUI and evidence seems to suggest it has something
	// to so with state being improperly set.
	if (!ImGUI_Initialized)
	{
		IMGUI_CHECKVERSION();
		ImGUIcontext = ImGui::CreateContext();
		ImGui::SetCurrentContext(ImGUIcontext);
		InputOutput = &ImGui::GetIO();// (void)InputOutput;

		InputOutput->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		InputOutput->BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		InputOutput->BackendPlatformName = "imgui_impl_glfw";
		InputOutput->DeltaTime = 1.0f / 60.0f;
		InputOutput->DisplaySize.x = Window::ActiveWindow()->Size.x;
		InputOutput->DisplaySize.y = Window::ActiveWindow()->Size.y;
		///	InputOutput.Fonts->AddFontDefault();
		///	InputOutput.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixels);

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		InputOutput->KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		InputOutput->KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		InputOutput->KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		InputOutput->KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		InputOutput->KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		InputOutput->KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		InputOutput->KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		InputOutput->KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		InputOutput->KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		InputOutput->KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		InputOutput->KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		InputOutput->KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		InputOutput->KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		InputOutput->KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		InputOutput->KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		InputOutput->KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		InputOutput->KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		InputOutput->KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		InputOutput->KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		InputOutput->KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		InputOutput->KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

 	//	InputOutput->ImeWindowHandle = glfwGetWin32Window(Window::ActiveWindow()->RenderingContext());
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(Window::ActiveWindow()->RenderingContext(), true);
		ImGui_ImplOpenGL3_Init("#version 400");


		ImGUI_Initialized = true;
		std::cout << "ImGui Initialized..." << std::endl;
	}
}
void ImGUILayer::OnDetach() {}
void ImGUILayer::OnUpdate()
{
//	InputOutput.DisplaySize = ImVec2(Window::ActiveWindow()->Size.x, Window::ActiveWindow()->Size.y);
	InputOutput = &ImGui::GetIO();// (void)InputOutput;
	InputOutput->DisplaySize.x = Window::ActiveWindow()->Size.x;
	InputOutput->DisplaySize.y = Window::ActiveWindow()->Size.y;
	ImGui::SetCurrentContext(ImGUIcontext);

	ImGui_ImplOpenGL3_NewFrame();

	ImGui::NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::SetNextWindowSize(ImVec2(110, 110), ImGuiSetCond_FirstUseEver);

	float time = (float)glfwGetTime();
	InputOutput->DeltaTime = Time > 0.0 ? (time - Time) : (1.0f / 60.0f);
	Time = time;
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");              // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");   // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show);
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	//	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(Window::ActiveWindow()->RenderingContext());


//int display_w, display_h;
//glfwMakeContextCurrent(Window::ActiveWindow()->RenderingContext());
//glfwGetFramebufferSize(Window::ActiveWindow()->RenderingContext(), &display_w, &display_h);
//glViewport(0, 0, display_w, display_h);
//Vec4 clear_color = Vec4(.5, .5, .5, .5);
//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//glClear(GL_COLOR_BUFFER_BIT);

//
//
//// Rendering
//ImGui::Render();
//int display_w, display_h;
//glfwMakeContextCurrent(window);
//glfwGetFramebufferSize(window, &display_w, &display_h);
//glViewport(0, 0, display_w, display_h);
//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
//glClear(GL_COLOR_BUFFER_BIT);
//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//glfwMakeContextCurrent(window);
//glfwSwapBuffers(window);

}
void ImGUILayer::OnEvent(Event& msg)
{
}

GraphicalInterface::GraphicalInterface()
{
	ImGui_ImplGlfw_InitForOpenGL(Window::ActiveWindow()->RenderingContext(), true);
	ImGui_ImplOpenGL3_Init("#version 410");
}
