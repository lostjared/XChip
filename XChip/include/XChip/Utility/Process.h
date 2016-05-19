/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016 Authors:
Jared Bruni.
Rafael Moura.


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/

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
