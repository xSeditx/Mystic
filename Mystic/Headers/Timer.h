#pragma once

#include<chrono>
#include"Common.h"


/// TODO: The System Timer of Chrono will be abstracted out for various task needed by the Engine
/// from Framerate calculations, to physics Calculations to Parallel processing of game objects
/// The timer and various timer devices will be created out of this class
/// It is likely a singleton system timer class will also be created to keep Application time
/// Then again the System Timer might just be a Timer Object inside of my Application class
/// in order to keep track of a Master clock for the program.
// Refuse to use count() (except for I / O).
// Refuse to use time_since_epoch() (except for I / O).
// Refuse to use conversion factors(such as 1000).
// Argue with it until it compiles.

enum TimerResolution
{
	Nanoseconds,
	Milliseconds,
	Seconds,
	Minutes,
	Hours,
	Days,
	System,
};
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::time_point<std::chrono::system_clock> SystemClock;
typedef std::chrono::time_point<std::chrono::steady_clock> SteadyClock;
typedef std::chrono::duration<float> fDuration;

class Timer
{
public:
	Timer();
	Timer(TimerResolution res);

	SystemClock StartTime;
	SystemClock EndTime;
};
class StopWatch  
{
public:
	StopWatch();
	StopWatch(TimerResolution res);
	float GetTicks();

	void Sleep();
	void Start();
	void Stop();

	SteadyClock StartTime;
	SteadyClock EndTime;

	std::chrono::duration<float>  TimeNow;
};

/// Create a Stopwatch that waits for X amount of time and when the stopwatch completes it fires off the Function which was declared in the Stopwatch constructor for that specific kind 
/// of stopwatch

struct FrameRate
{

	void Update();
	void Set(float rate)
	{
		Coefficient = 1000.0 / rate;
	}
	float Get() { return Rate; }


	void SleepFor(float _time);
private:
	float Rate = 0.0f;
	float Accumulator = 0.0f;
	float PreviousTime = 0.0f;
	float Coefficient = 0.0f;
	float CurrentTime = 0.0f;
	float Tracker = 0.0f;

	unsigned int Counter = 0.0f;
	float LastFrame = 0.0;
};

class BenchMark
{
public:
	BenchMark();
	BenchMark(std::string name);

	~BenchMark();
	std::chrono::time_point<std::chrono::steady_clock>  PerformanceTimer;

	float NumberOfSamples = 100;
	int Accumulator;
	std::string Name;

	static int ObjectCounter;
	static long long AverageTimer;
	static long long AverageResult;

	void Start();
	void End();
};




//std::chrono::duration_cast <std::chrono::nanoseconds>
//	(std::chrono::high_resolution_clock::now() - PerformanceTimer).count()
//std::chrono::time_point<std::chrono::steady_clock>  PerformanceTimer;