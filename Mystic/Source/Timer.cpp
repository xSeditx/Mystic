#include"Timer.h"
 int  BenchMark::ObjectCounter;
 long long BenchMark::AverageTimer;
 long long BenchMark::AverageResult;
#include"OpenGL.h"
Timer::Timer(){}
Timer::Timer(TimerResolution res){}

void StopWatch::Sleep(){}
void StopWatch::Start()
{
    StartTime = std::chrono::high_resolution_clock::now();
}
float StopWatch::GetTicks()
{
	std::chrono::duration<float> TimeNow = std::chrono::high_resolution_clock::now() - StartTime;
	return  TimeNow.count();
}
void StopWatch::Stop(){}


#include <time.h>
//void SleepFor(float secs)
//{
//	//float end = clock() / CLOCKS_PER_SEC + secs;
//	//while ((clock() / CLOCKS_PER_SEC) < end);
//}
OPTIMIZATION_OFF();
void FrameRate::SleepFor(float _time)
{
	float EndTime = (glfwGetTime() * 1000.0) + _time;
	float delta = 0;
	while (delta < EndTime)
	{
		float T = (glfwGetTime() * 1000);
		delta =  T;
	}
}
OPTIMIZATION_ON();
void FrameRate::Update()
{
	CurrentTime = glfwGetTime() * 1000.0;
	float DeltaTime = CurrentTime - PreviousTime;
	PreviousTime = CurrentTime;
	float P = (CurrentTime - LastFrame);
	if (CurrentTime - Tracker >= 1000)
	{// If One second has passed
		Rate = Counter;
		Tracker = CurrentTime;
		Counter = 0;
	}
	if (P >= Coefficient)
	{// If the DeltaTime is over the Frame Limiter Coefficient
		LastFrame = CurrentTime;
		Counter++;
	}
	else
	{
		SleepFor(P);
	}
}



#include"window.h"
#pragma optimize( "BenchMark::Start()", off )
#pragma optimize( "BenchMark::End()", off )

BenchMark::BenchMark()
	:
	Accumulator(0)
{
	PerformanceTimer = std::chrono::high_resolution_clock::now();
	ObjectCounter++;
}
BenchMark::BenchMark(std::string _name)
{
	Name = _name;
	PerformanceTimer = std::chrono::high_resolution_clock::now();
	ObjectCounter++;
	Accumulator = 0;
}

BenchMark::~BenchMark()
{
	AverageTimer += std::chrono::duration_cast  <std::chrono::nanoseconds>  (std::chrono::high_resolution_clock::now() - PerformanceTimer).count();
	if (ObjectCounter > NumberOfSamples)
	{
		Print("LastFrame: "
			<<
			std::chrono::duration_cast  <std::chrono::nanoseconds>  (std::chrono::high_resolution_clock::now() - PerformanceTimer).count()
			<< "n/s    "
			<< "AvgSpeed: "
			<< float((AverageResult) / 1000000.0f) << " m/s  "
			<< "FPS: " << Window::ActiveWindow()->FramesPerSecond.Get()
		);

		ObjectCounter = 0;
		AverageResult = AverageTimer / NumberOfSamples;
		AverageTimer = 0;
	}
}

void BenchMark::Start()
{
	Accumulator++;
	PerformanceTimer = std::chrono::high_resolution_clock::now();
	NumberOfSamples = 100;
}
void BenchMark::End()
{
	AverageTimer += std::chrono::duration_cast <std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - PerformanceTimer).count();
	if (Accumulator > NumberOfSamples)
	{
		Print("Timer: " << Name);
		Print("LastFrame: "
			<<
			std::chrono::duration_cast <std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - PerformanceTimer).count()
			<< "n/s    "
			<< "AvgSpeed: "
			<< (AverageResult)  / 1000000 << "m/s  "
			<< "FPS: " << Window::ActiveWindow()->FramesPerSecond.Get()
		);

		Accumulator = 0;
		AverageResult = AverageTimer / NumberOfSamples;
		AverageTimer = 0;
	}

}



//  Print("LastFrame: "
//  	<< std::chrono::duration_cast <std::chrono::nanoseconds>
//  	(std::chrono::high_resolution_clock::now() - PerformanceTimer).count()
//  	<< "n/s    "
//  	<< "AvgSpeed: "
//  	<< (AverageResult) << "n/s  "
//  );
//  
//  Print(Window::ActiveWindow()->FramesPerSecond.Get());
	//	<< "FPS: "
	//	<< Window::SCREEN->Framerate.Get());




