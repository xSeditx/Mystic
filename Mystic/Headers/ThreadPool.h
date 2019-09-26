#pragma once
#include <thread>
#include <functional>
#include <future>
#include <deque>

/// Look up Slim reader writer lock
#include"Common.h"
#include<queue>

//https://chromium.googlesource.com/chromium/src.git/+/master/docs/callback.md
// TODO("Look into Intels APIC controller which allows for Multithreading without C++ standard library");
//Thread Pool Implementation on Github: https://github.com/mtrebi/thread-pool/blob/master/README.md#queue
//Threadpool with documentation: https://www.reddit.com/r/cpp/comments/9lvji0/c_threadpool_with_documentation/
//Original White paper on Work stealing Queues: http://supertech.csail.mit.edu/papers/steal.pdf

/*
Threadpool and Job system. Much work to do in this area. Work on it had slowed because I turned my focus on rendering something to the screen
for a change
*/

/*
TODO: 

Create a WhenOne and WhenAll if C++ 20 does not have it yet

WhenAll(std::vector<Functions_or_Lambdas>).than([]{Lambda Calls});
WhenAll(functions).tpause()... Then.Pause
*/

/// WORK STEALING CPPCON Pabk
//https://www.youtube.com/watch?v=iLHNF7SgVN4

#define MyBinder(_func,...) std::bind(_func,  ##__VA_ARGS__)


struct Event;

template<typename F>
void ASYNC(...)
{
	va_list va;
	MyBinder(F, va);
}

struct Task
{
public:
};

typedef void(*Worker_Function)();

struct JobQueue
{
public:
	JobQueue()
	{

	}
	std::deque< Worker_Function> TaskQueue;
	std::condition_variable is_Ready;
	std::mutex QueueMutex;
	bool is_Done{ false };

	bool Try_Pop(Worker_Function &func)
	{// Try to aquire a function off the Queue to run
		std::unique_lock<std::mutex> Lock{ QueueMutex, std::try_to_lock };
		if(!Lock || TaskQueue.empty())
		{
			return false;
		}
		func = std::move(TaskQueue.front());
		TaskQueue.pop_front();
		return true;
	}

	void Done() 
	{
		{
			std::unique_lock<std::mutex> Lock{ QueueMutex }; is_Done = true;
		}
		is_Ready.notify_all();
	}
	bool pop(Worker_Function &func) {
		std::unique_lock<std::mutex> Lock{QueueMutex};
		while(TaskQueue.empty() && !is_Done)
		{
			is_Ready.wait(Lock);
		}
		if (TaskQueue.empty())
		{
			return false;
		}
		func = std::move(TaskQueue.front());
		TaskQueue.pop_front();
		return true;
	}
	template<typename F> bool try_push(F&& func)
	{
		{
			std::unique_lock<std::mutex> Lock{ QueueMutex ,std::try_to_lock };
			if (!Lock)
			{
				return false;
			}
			TaskQueue.emplace_back(std::forward<F>(func));
		}
		is_Ready.notify_one();
		return true;
	}
	template<typename F> void push(F&& func)
 	{
 		{
 			std::unique_lock<std::mutex> Lock{ QueueMutex };
			TaskQueue.emplace_back(std::forward<F>(func));
 		}
 	}
};

class ThreadPool
{
	const size_t ThreadCount{ std::thread::hardware_concurrency() };
	std::vector<std::thread> Worker_Threads;
	std::vector<JobQueue> ThreadQueue{ ThreadCount };
	std::atomic<unsigned int> Index{ 0 };

	ThreadPool()
	{

		for_loop(N, ThreadCount)
		{
			Worker_Threads.emplace_back([&, N] {Run(N); });
		}
	}
	~ThreadPool()
	{
		for (auto& Q : ThreadQueue)
		{
			Q.Done();
		}
		for (auto& WT : Worker_Threads)
		{
			WT.join();
		}
	}

 	void Run(unsigned int _i)
	{
		while (true) 
		{
			Worker_Function Func;
			for(unsigned int N = 0; N != ThreadCount;++N)
			{
				if (ThreadQueue[(size_t)(_i + N) % ThreadCount].Try_Pop(Func))
				{
					break;
				}
			}
			if (!ThreadQueue[_i].pop(Func))break;
			Func();
		}
//		while (true)
//		{
//			std::function<void(Event&)>_Func;
//			for (unsigned int n = 0; n != ThreadCount; ++n)
//			{
//				if (ThreadQueue[(i + n) % ThreadCount].try_pop(_Func))
//				{
//					break;
//				}
//			}
//			if (!ThreadQueue[i].pop(_Func))break;
//			Event *msg;
//			_Func(*msg);
//		}
	}
public:

	static ThreadPool &Instance()
	{
		static ThreadPool instance;
		return instance;
	}

	template<typename F>
	void Async(F&& _func)
	{
		auto i = Index++;
		int K = 8;
		for (unsigned int n = 0; n != ThreadCount * K; ++n) // K is Tunable 
		{
			if (ThreadQueue[(i + n) % ThreadCount].try_push(Worker_Function(_func)))
			{
				return;
			}
		}
		ThreadQueue[i % ThreadCount].push(std::forward<F>(_func));
	}
};












/// ================================================================================================================
class ASyncTask
{// Dpes a bunch of a specified task _count number of times.
	// Need to create a method of passing params to this class
public:
	ASyncTask() {}
	~ASyncTask()
	{
		for (auto& T : WorkList)
		{
			T.detach();
		}
	}
	template<typename ...Args>
	ASyncTask(void(*function)(Args...args), unsigned int _count , Args...args)
	{
		for (int i = 0; i < _count; ++i)
		{
			WorkList.push_back(std::thread(function, &args...));
		}
	}
	//ASyncTask(void(*function)(JobArguments *_args), unsigned int _count)
	//{
	//	for (int i = 0; i < _count; ++i)
	//	{
	//		WorkList.push_back(std::thread(function));
	//	}
	//}
	void Start(int index)
	{
		WorkList.push_back(std::thread(Functions[index]));
	}
private:
	std::vector<void(*)()> Functions;
	std::vector<std::thread> WorkList;
};





