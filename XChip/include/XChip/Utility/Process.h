#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <iostream>
#include <string>

#if defined(__APPLE__) || defined(__linux__)
#include <signal.h>
#include <unistd.h>
//#include <sys/types.h>
//#include <sys/wait.h>
#include <pthread.h>
#include <cstring>

#elif defined(_WIN32)
#include <Windows.h>
#include <process.h>

#endif


namespace xchip { namespace utility {
	
	
class Process
{
public:
	using ProcFunc = int(*)(void*);


	Process();
	~Process();
	Process(const Process&) = delete;
	const Process& operator=(const Process&) = delete;

	bool IsRunning() const;

	bool Run(const std::string &app);
	bool Run(ProcFunc pfunc, void* arg = nullptr);
	int Join();
	void Terminate();
private:

#if defined(__APPLE__) || defined(__linux__)
	//pid_t pid = 0;
	pthread_t _pthread = 0;

#elif defined(_WIN32)

	unsigned int _threadId;
	HANDLE _threadHandle;


#endif

};
	
	
	
	














}}









#endif
