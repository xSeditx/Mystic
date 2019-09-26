#pragma once
class TaskManager
{
	TaskManager();
public:
	TaskManager &Instance(); // Returns out EventSystem object and prevents Copies from being made
	TaskManager(TaskManager const&) = delete;
	void operator = (TaskManager const&) = delete;

private:
	bool Initialized = false;
};

