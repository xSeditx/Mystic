#include"EventSystem.h"
#include"Input.h"

Event::Event()
	:
	ID(0),
	WinID(0),
	Time(0)
{
	memset(&data, 0, 16);
}
Event::Event(MsgType msg)
	:
	ID(msg),
	WinID(0),
	Time(0)
{ 
	memset(&data, 0, 16);
}

EventSystem::EventSystem()
{
}
EventSystem & EventSystem::Instance()
{
	static EventSystem instance;
	return instance;
}
void EventSystem::PostMSG(Event msg)
{
	Messages.push(msg);
}
void EventSystem::RegisterListener(MsgType msg, Listener & handler)
{
	ListenerMap[msg].push_back(&handler);
}
void EventSystem::RemoveListener(MsgType msg, Listener &handler)
{
	ListenerMap[msg].erase(std::find(ListenerMap[msg].begin(), ListenerMap[msg].end(), &handler)); 
}


void EventSystem::Dispatch(Event msg)
{
	for(auto& Callback : ListenerMap[msg.ID])
	{
		Callback->Handler(msg);
	}
} 
bool EventSystem::PeekMSG(Event & msg, unsigned int rangemin, unsigned int rangemax, int handlingflags)
{
	if (Messages.size())
	{
		msg = Event(Messages.front());
		if (msg.ID > rangemin && msg.ID < rangemax)
		{
			switch (handlingflags)
			{
			case REMOVE_MESSAGE:
			{// ONLY USE THIS FOR NOW
				Messages.pop();
				return true;
			}break;
			case KEEP_MESSAGE:
			{
				__debugbreak();
			}break;
			case REPOST_MESSAGE:
			{/// THIS WILL CURRENTLY BE A PROBLEM DO NOT USE IT
				Messages.pop();
				PostMSG(msg);
				__debugbreak();
			}break;

			}
		}
	}
	return false;//Messages.size() > 0;
}
void EventSystem::PollEvents()
{
	InputManager::PollNativeEvents();
}