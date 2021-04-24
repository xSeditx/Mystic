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

#include<functional>
#include<algorithm>

typedef void(*Worker_Function)();


class B
{
public:

	template<typename FUNC, typename ...ARGS>
	auto operator()(FUNC func, ARGS...args)->decltype(func(args...))
	{
		func(args...);
	}
};

template<typename _RETURN, typename ...ARGS>
class D : public B
{
public:
	D(_RETURN ret, ARGS...arg)
	{
	//	func = ret(arg...);
	}
	/* maybe a tuple unpacked would allow storing params than executing function via operator()*/
	void operator()(_RETURN ret, ARGS...arg)
	{
		ret(arg...);
	}
	_RETURN func;
};

template <typename _Ty>
class Task;


template<typename _RETURN, typename ...ARGS>
struct Task<_RETURN(ARGS...)>
{
private:


	struct Callable_Base
	{
		virtual ~Callable_Base() = default;
		virtual _RETURN Invoke(ARGS...) = 0;
	};

	template<typename _FUNC>
	struct Callable : public Callable_Base
	{

		Callable(const _FUNC& _func)
			: Func(_func)
		{}

		~Callable() override = default;


		_RETURN Invoke(ARGS... args) override
		{
			return Func(args...);
		}

	private:
		_FUNC Func;
	};


public:
	//Task() = default;
	template<typename _F, ARGS...>
	Task(_F func, ARGS...arg)
	{
		TaskFunction = std::make_unique<Callable<_F>>(func);
	}

	template <typename T>
	Task& operator=(T t) {
		TaskFunction = std::make_unique<Callable<T>>(t);
		return *this;
	}

	_RETURN operator()(ARGS... args) const
	{
		return TaskFunction->Invoke(args...);
	}


	std::unique_ptr<Callable_Base> TaskFunction;

};



template<size_t _SZ>
class lfRingBuffer
{

	bool isDone{ false };
	std::condition_variable isReady;

	std::array<Worker_Function, _SZ> ringBuffer; /// std::atomic<Worker_function, _SZ> is what should be in the Array

	std::atomic<uint32_t> writePos{ 0 }, readPos{ 0 };

	static constexpr size_t getNextPosition(uint32_t _current)
	{
		return ++_current == ringBufferSize ? 0 : _current;
	}
	static constexpr size_t ringBufferSize = _SZ + 1;

public:

	template<typename _FUNC, typename ...ARGS>
	auto Async(_FUNC&& _func, ARGS&&... args)->std::future<decltype(_func(args...))>
	{
		std::promise<decltype(_func(args...))> result;
		auto FuncPush = std::bind(std::forward<_FUNC>(_func), std::forward<ARGS>( args)...);

		auto wrapper = []() {
			//_func(args...);
			std::cout << "Hello";
		 	//FuncPush();
		};
		//void* Erased =;
		Push((Worker_Function)wrapper);
		return result.get_future();
	}



	bool Push(Worker_Function&& _function)
	{
		auto oldWritePos = writePos.load();
		auto newWritePos = getNextPosition(oldWritePos);

		if(newWritePos == readPos.load())
		{
			return false;
		}
		ringBuffer[oldWritePos]= _function;///.store(_function); Is what we should have if we were using Atomics at the moment

		writePos.store(newWritePos);
		return true;
	}
	bool Pop(Worker_Function& _returnFunc)
	{
		while (true)
		{
			auto oldWritePos = writePos.load();
			auto oldReadPos = readPos.load();

			if (oldWritePos == oldReadPos)
			{
				return false;
			}
			_returnFunc = std::move(*ringBuffer[oldReadPos]);/// .load()); If were using Atomics in the Array
			if (readPos.compare_exchange_strong(oldReadPos, getNextPosition(oldReadPos)))
			{
				return true;
			}
	}
	}



	Worker_Function operator()(void(*input)())
	{
		return static_cast<Worker_Function>(input);
	}
};

/*

I am getting stuck attempting to forward a function pointer to an array of Atomic function pointers.I am also a bit confused about what exactly is a function pointer come to think of it
I know pointers are ints but are function pointers as well or is there additional data stored about the return type and param ? I ask because this is my current situation...

```cpp
typedef void(*Worker_Function)();
std: : array<std::atomic<Worker_Function>, SIZE> ringBuffer;
```


My push function that is suppose to push a function into the Queue(array here) looks like this..
```cpp
bool Push(Worker_Function&& _function)
{
	auto oldWritePos = writePos.load();
	auto newWritePos = getNextPosition(oldWritePos);

	if (newWritePos == readPos.load())
	{
		return false;
	}
	ringBuffer[oldWritePos].store(_function);
	writePos.store(newWritePos);
	return true;
}
```
On the atomic.store()

*/



///#define MyBinder(_func,...) std::bind(_func,  ##__VA_ARGS__)
///
///
///struct Event;
///
///template<typename F>
///void ASYNC(...)
///{
///	va_list va;
///	MyBinder(F, va);
///}
///
///struct Task
///{
///public:
///};












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
	bool pop(Worker_Function &func) 
	{
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


/* NOTE: Build a Task list and Atomically swap the Head with your Task List in a single atomic write*/
// lfQueue
//http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.53.8674&rep=rep1&type=pdf

#include<atomic>
template<typename _Ty>
struct lfNode
{
	lfNode(_Ty& _value)
		:
		Value(_value),
		Next(nullptr)
	{}
	_Ty Value;
	lfNode<_Ty> *Next;
};

//AlignAs
///  
///  template<typename _Ty>
///  class lfQueue
///  {
///  	using Object = lfNode<_Ty>;
///  public:
///  
///  
///  	std::atomic<Object*>
///  		Head{ nullptr },
///  		Tail{ nullptr };
///  
///  
///  	lfQueue() = default;
///  	~lfQueue()
///  	{// Traverse and Destroy
///  		auto First = Head.load();
///  		while (First)
///  		{
///  			auto Dead = First;
///  			First = First->Next;
///  			delete(Dead);
///  		}
///  	}
///  //https://github.com/tempesta-tech/blog/blob/master/lockfree_rb_q.cc
///  
///  
///  	bool pop_back(_Ty &_value)
///  	{
///  		auto OldTail = Tail.load();
///  	
///  		while ((OldTail->Next != nullptr) && !Tail.compare_exchange_weak(OldHead, OldHead->Next))
///  		{
///  		}
///  		_value = OldTail.Value;
///  		return true;
///  	}
///  
///  	bool pop_front(_Ty &_value)
///  	{
///  		auto OldHead = Head.load();
///  		while ( (OldHead != nullptr) && !Head.compare_exchange_weak(OldHead, OldHead->Next))
///  		{
///  		}
///  		_value = result.Value;
///  		return true;
///  	}
///  
///  	void push_front(_Ty _value)
///  	{
///  		auto NewNode = new Object(_value); //Allocate New Node
///  		NewNode->Next = Head.load();// Swing Head pointer from Current First node to New Node->Next
///  		while (!Head.compare_exchange_weak(NewNode->Next, NewNode))//Link New node as Next Node atomically
///  		{// If Head has not changed since I looked at it CAS NewNode->Next = Head.load() again
///  		}
///  	}
///  
///  
///  	_Ty* find(_Ty _value) const
///  	{
///  		auto P = Head.load();
///  		while (P && P->Next != _value)
///  		{
///  			P = P->Next;
///  		}
///  		return P ? &P->Value : nullptr;
///  
///  	}
///  
///  
///  
///  };

//do
//{
//	result = Head;
//	if (result->Next == NULL)
//	{// Queue Empty
//		return false;
//	}
//}
////std::atomic<node<T>*> head;
//
//
//		// now make new_node the new head, but if the head
//		// is no longer what's stored in new_node->next
//		// (some other thread must have inserted a node just now)
//		// then put that new head into new_node->next and try again
//		while (!head.compare_exchange_weak(new_node->next, new_node,
//			std::memory_order_release,
//			std::memory_order_relaxed))
//			; // the body of the loop is empty
//	}
//};
//lfNode<_Ty> *NewNode= new lfNode<_Ty>(_value);
//NewNode->Next = Head.load();
//bool Success = false;
//do
//{
//	//Success = std::atomic_compare_exchange_weak<_Ty>(OldTail, nullptr, NewTail);
//	Success = std::atomic<lfNode<_Ty>>::compare_exchange_weak(NewNode, NewNode->Next); //WRONG!!!!!!!!!!!!!!!!!!!!!!
//	if (!Success)
//	{////		//Tail.compare_exchange_weak(OldTail, Tail.Next);
//	}
//} while (!Success);
//Tail.atomic_compare_exchange_weak(OldTail, NewTail);
////Tail.compare_exchange_weak(OldTail, NewTail);

/*
Slot is atomic

Owner checks his own slot, is something there , Use it else
Producer Thread: Which(Slot[(++CurrentSlot %= K] != Null){
Slot[CurrentSlot] = Task;
CV[CurrentSlot].Signal;
}
when empty producer owns it when not empty consumer ownes it



Consume:

While(Task != done)
{
}

*/
//bool atomic_compare_exchange_weak(std::atomic<T>* obj,
//	typename std::atomic<T>::value_type* expected,
//	typename std::atomic<T>::value_type desired)

///https://github.com/facebook/folly/tree/master/folly












	//Worker_Function Job;
//Task& operator=(const Task& _Right) {
//	Task(_Right).swap(*this); 
//	return *this;
//}
//
//Task(Task&& _Right) noexcept {
////	this->_Reset_move(_STD move(_Right));
//}
//
//void swap(Task& _Right) noexcept
//{ // swap with _Right
//	std::swap<decltype(this), decltype( _Right)>(*this, _Right);//this->_Swap(_Right);
//}