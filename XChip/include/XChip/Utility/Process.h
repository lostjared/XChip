#ifndef __PROCESS_H__
#define __PROCESS_H__

#if defined(__APPLE__) || defined(__linux__)
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



#elif defined(_WIN32)
#include <Windows.h>
#include <process.h>

#endif


#include <cstring>
#include <iostream>
#include <string>




namespace xchip { namespace utility {
	
	
class Process
{
public:
	Process();
	~Process();
	Process(const Process&) = delete;
	const Process& operator=(const Process&) = delete;
	bool IsRunning() const;
	bool Run(const std::string &app);
	int Join();
	int Terminate();
private:

#if defined(__APPLE__) || defined(__linux__)
	pid_t pid = 0;

#elif defined(_WIN32)
	static bool _stdcall enum_windows_callback(HWND hwnd, LPARAM neededId);
	void CloseHandles();
	void ZeroInfo();
	void CloseAndZero();
	STARTUPINFO _si;
	PROCESS_INFORMATION _pi;


#endif

};
	
	
	
	














}}









#endif
