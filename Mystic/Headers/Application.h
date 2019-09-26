#pragma once
/* =============================================================================================================================================================
 APPLICATION
-------------
~ Managers ( singletons )
-------------
    TASK  // Handles the thread pool and asigns processes to each thread
    STATE
    SERVICE
    ENVIRONMENT
    PLATFORM
NOTES: Making these Singletons prevents the duplication of resources to ensure smooth performance

~ Framework
-------------
    GAMELOOP
    ---- GetNativeMessages;
    ---- Schedule execution
    ---- Distribute Changes
    ---- Check Execution State
    SCHEDULER
    ---- Determine Systems to Execute
    ---- Send to Task Manager
    ---- Wait for Completion
================================================================================================================================================================ */


#include"Common.h"
#include"Timer.h"
#include"EventSystem.h"
#include"Input.h"

class Window;

class  MYSTIC Application
{
public:
	Application();
   ~Application();
	iVec2 Position;
	iVec2 Size;
	std::string Title;

	void Start();
	void Run();
	void Pause();
	void End();

	void Update();
	void Render();

	InputManager *Input;
	EventSystem  &MessageHandler = EventSystem::Instance();

	Window *ApplicationWindow;
	bool Running = true;
protected:

	virtual void OnCreate();
	virtual void OnStart();
	virtual void OnRun(); /// Differs from start in that it can resume execution; Possibly change to Resume later or something
	virtual void OnPause();
	virtual void OnSleep();
	virtual void OnEnd();

	virtual void OnUpdate();
	virtual void OnRender();

	void CreateMysticWindow();
	virtual void SetWindowProperties();

	Timer ApplicationTimer;

protected:

	void SetHints();
	void SetStereo(bool);
	void sRGBCapable(bool);
	void ForwardCompatible(bool);
	void DebugContext(bool);
	void ResizableWindow(bool);
	void VisibleWindow(bool);
	void DecoratedWindow(bool);
	void SetRedBits(unsigned int);
	void SetGreenBits(unsigned int);
	void SetBlueBits(unsigned int);
	void SetAlphaBits(unsigned int);
	void SetDepthBits(unsigned int);
	void SetStencilBits(unsigned int);
	void SetAccumulatorRedBits(unsigned int);
	void SetAccumulatorGreenBits(unsigned int);
	void SetAccumulatorBlueBits(unsigned int);
	void SetAccumulatorAlphaBits(unsigned int);
	void SetAuxiliaryBits(unsigned int);
	void SetNumberOfSamples(unsigned int);
	void SetRefreshRate(unsigned int);
	void UseOpenGLClient(unsigned int);
	void UseOpenGLESClient(unsigned int);
	void SetMajorVersion(unsigned int);
	void SetMinorVersion(unsigned int);
	void UseOpenGLProfile(unsigned int);


private: // Window Information. Default Properties will be set however user can Override these settings

	void SetHints(HintsStruct hint);
	HintsStruct Hints;
}; 

///                                                                                                   |||
///       Universal scenes and universal objects are responsible for registering all their extensions |||
///   with the state manager so that the extensions will get notified of changes made by other        |||
///   extensions(ie systems). Example would be to receive notifications of position and orientation   |||
///   changes made by the physics extension.                                                          |||
///                                                                                                   |||
