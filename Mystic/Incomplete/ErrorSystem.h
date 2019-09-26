#pragma once
#include<unordered_map>

#include"Common.h"

class ErrorSystem
{
	ErrorSystem();
public:
	ErrorSystem & Get();
	ErrorSystem(ErrorSystem const&) = delete;
	void operator = (ErrorSystem const&) = delete;


	bool Init();
	~ErrorSystem();
	bool isInitialized() { return (Initialized == true); }
private:
	bool Initialized = false;
};

