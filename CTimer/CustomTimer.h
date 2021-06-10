#include "thread.h"
#include <iostream>

typedef void(CALLBACK* Timerfunc)(LPVOID pParam);
typedef Timerfunc TimerHandler;
class CustomTimer :
	public Thread
{
public:
	CustomTimer(void);
	~CustomTimer(void);
	void registerHandler(TimerHandler handler, LPVOID pParam);
	void setInterval(int nMillisecond) { m_nInterval = nMillisecond; };
	void Run();
	void Cancel();
private:
	TimerHandler m_handler;
	int m_nInterval;
	LPVOID m_Parameter;
};

