#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <iostream>
#include <string>


namespace xchip {
	
	
	class Process {
	public:
		Process();
		void Run(const std::string &app);
		void Stop();
	private:
		int pid;
	};
	
	
	
	
}









#endif
