#include "CustomTimer.h"
#include "timer.h"

CustomTimer::CustomTimer()
	:m_handler(0)
	, m_nInterval(-1)
{
}


CustomTimer::~CustomTimer(void)
{
}

void CustomTimer::registerHandler(TimerHandler handler, LPVOID pParam)
{
	m_handler = handler;
	m_Parameter = pParam;
}

void CustomTimer::Run()
{
	unsigned long tickNow = GetTickCount();
	unsigned long tickLastTime = tickNow;
	while (!IsStop())
	{
		tickNow = GetTickCount();
		if (tickNow - tickLastTime > m_nInterval)
		{
			if (m_handler)
			{
				(*m_handler)(m_Parameter);
			}
			tickLastTime = GetTickCount();
		}
		Sleep(1);
	}
}

void CustomTimer::Cancel()
{
	Stop();
}

void CALLBACK RegularFeedback(LPVOID pParam)
{
	std::cout << "callback" << std::endl;
}


void TestTimerQueue() {
	wzq::TimerQueue q;
	q.Run();
	auto time_point_now = std::chrono::high_resolution_clock::now();
	
	//q.AddFuncAfterDuration(std::chrono::microseconds(5000000), []() { std::cout << "this is " << std::endl; });
	q.AddRepeatedFunc(100, std::chrono::microseconds(5000000), []() { std::cout << "repeated " << std::endl; });
	//for (int i = 5; i < 15; ++i) {
		//q.AddFuncAfterDuration(std::chrono::seconds(i + 1), [i]() { std::cout << "this is " << i << std::endl; });
		//q.AddFuncAfterDuration(std::chrono::microseconds(5000000), [i]() { std::cout << "this is " << i << std::endl; });

		//q.AddFuncAtTimePoint(std::chrono::high_resolution_clock::now() + std::chrono::seconds(1),
		//	[i]() { std::cout << "this is " << i << " at " << std::endl; });
	//}

	//int id = q.AddRepeatedFunc(10, std::chrono::seconds(1), []() { std::cout << "func " << std::endl; });
	//std::this_thread::sleep_for(std::chrono::seconds(4));
	//q.CancelRepeatedFuncId(id);

	std::this_thread::sleep_for(std::chrono::seconds(30));
	q.Stop();
}

int main()
{
//	CustomTimer timer;
//	LPVOID p = nullptr;
//	timer.registerHandler(RegularFeedback, p);
//	timer.setInterval(1000);
//	timer.Start();
	//timer.Cancel();
	TestTimerQueue();
	return 0;
}