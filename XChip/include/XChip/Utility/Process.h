#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <iostream>
#include <string>

#if defined(__APPLE__) || defined(__linux__)
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<cstring>
#endif


namespace xchip {
	
	
	class Process
	{
	public:
		Process();
		void Run(const std::string &app);
		void Stop();
	private:
#if defined(__APPLE__) || defined(__linux__)
		pid_t pid;
#endif
	};
	
	
	
	
}









#endif