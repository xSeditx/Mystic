#pragma once
#include<vector>
#include<string>


#include"ECSsystem.h"
#include"ECScomponent.h"

struct ImGuiContext;
struct Event;
struct ImGuiIO;

#define IMGUI_IMPL_OPENGL_LOADER_GLAD


COMPONENT(ControlPosition)
{

};
COMPONENT(OnClickComponent)
{

};


struct GUIsystem
    : 
    public BaseSystem
{
    GUIsystem()
        :
        BaseSystem()
    {
        AddComponentType(ControlPosition::ID);
     //   AddComponentType(MovementComponent::ID);
#ifdef _DEBUG
        SystemName = (typeid(this).name());
#endif
    }
};



struct GUILayer
{
	GUILayer(std::string _name);
	int LayerID;
	std::string Name;
};

struct ImGUILayer 
	:
	public GUILayer
{
	ImGUILayer();

	ImGuiContext* ImGUIcontext;
	bool ImGUI_Initialized = false;
	ImGuiIO *InputOutput;

	void OnAttach();
	void OnDetach();
	void OnUpdate();
	void OnEvent(Event& msg);

	float Time;
};


class GraphicalInterface
{// Eventially Move the whole system down here once I figure out how this all works
public:
	GraphicalInterface();
	std::vector<GUILayer> Layer;
};