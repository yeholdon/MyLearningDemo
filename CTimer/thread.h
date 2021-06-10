#include <Windows.h>
class Thread
{
public:
	Thread(void);
	virtual ~Thread(void);

	virtual void  Run() = 0;
	void Start();
	void Stop();
	BOOL IsStop();
protected:
	static DWORD WINAPI ThreadProc(LPVOID pParam);
private:
	BOOL m_bStopFlag;
	HANDLE m_hThread;
};