#pragma once
// Standard Includes
#include<queue>
#include<unordered_map>
#include<vector>

// Project Includes
#include"Common.h"
//============================================================

typedef uint32_t MsgType;
typedef uint32_t WindowID;// This will likely change to a pointer, or some other form later on.
//============================================================
class Window;


struct MYSTIC Event
{// Event Union Defining an Event the Engine can produce
	Event();
	Event(MsgType msg);

	MsgType ID;
	WindowID WinID;    
	unsigned long Time;  

#pragma pack(push, 1)
	union msgUnion
	{// All messages are 128 bits
        msgUnion() = default;

		msgUnion &operator =(const msgUnion&other)
		{
			memcpy(this, &other, sizeof(other)); // Should be 16 bytes, check this if any changes get made
			return *this;
		}

		struct msgstruct
		{
			uint32_t wordParam;
			uint64_t longParam;
			uint32_t Padding;
		}MSG; // 128 bits
		struct kbstruct
		{
			uint32_t KeyCode;
			uint32_t ScanCode;
			uint32_t Action;
			uint32_t Mods;
		}Keyboard; // 128
		struct mousestruct
		{
			uint32_t Button; 
			uint32_t x;  
			uint32_t y;    
			int16_t relX;
			int16_t relY;
		}Mouse; // 128
		struct buttonstruct
		{
			struct buttons
			{
				uint8_t
					Left,
					Right,
					Middle,
					Wheel;
			}Buttons; // 32
			uint32_t Action;        
			uint32_t Modification;  
			uint32_t Padding;       
		}MouseButton; // 128
		struct textstruct {}TextInput;
	}data;
#pragma pack(pop)

	bool Handled() { return (ID == 0); }
};

struct Listener
{
public:
	Listener (void(*func)(Event))
		:
		_handler(func)
	{}
	void SetHandler(void(*func)(Event))
	{
		_handler = func;
	}
	inline void Handler(Event msg)
	{
		_handler(msg); 
	};
private:
	void(*_handler)(Event msg);
};

class MYSTIC EventSystem
{// Singleton design pattern
	EventSystem();
public:
	static EventSystem &Instance();           // Initializes EventSystem when called first time
	EventSystem(EventSystem const&) = delete; // and prevents Copies from being made
	void operator = (EventSystem const&) = delete;

public: 
	void PollEvents(); 
	void PostMSG(Event msg);
	bool PeekMSG(Event&msg, unsigned int rangemin, unsigned int rangemax, int handlingflags);
	void Dispatch(Event msg);

	void RegisterListener(MsgType msg, Listener &handler);
	void RemoveListener(MsgType msg, Listener &handler);
private:
	std::queue<Event> Messages;
	std::unordered_map<MsgType, std::vector<Listener*>> ListenerMap;
};
