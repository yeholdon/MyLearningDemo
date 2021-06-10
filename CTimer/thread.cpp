#include "Thread.h"
Thread::Thread()
	:m_bStopFlag(FALSE)
	, m_hThread(INVALID_HANDLE_VALUE)
{
}

Thread::~Thread(void)
{
	Stop();
}

void Thread::Start()
{
	unsigned long* p = NULL;
	m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, p);
}

DWORD WINAPI Thread::ThreadProc(LPVOID pParam)
{
	Thread* thread = (Thread*)pParam;
	thread->Run();

	CloseHandle(thread->m_hThread);
	thread->m_hThread = INVALID_HANDLE_VALUE;
	return 0;
}

void Thread::Stop()
{
	m_bStopFlag = TRUE;
	if (m_hThread != INVALID_HANDLE_VALUE)
	{
		if (WaitForSingleObject(m_hThread, INFINITE) != WAIT_ABANDONED)
		{
			CloseHandle(m_hThread);
		}
		m_hThread = INVALID_HANDLE_VALUE;
	}
}

BOOL Thread::IsStop()
{
	return m_bStopFlag;
}