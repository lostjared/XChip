/*

XChip - A chip8 lib and emulator.
Copyright (C) 2016  Authors:
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

#include <cerrno>
#include <csignal>
#include <string>
#include <algorithm>

#include <XChip/Utils/Log.h>
#include <XChip/Utils/Process.h>
#include <XChip/Utils/ScopeExit.h>
#include <XChip/Utils/Timer.h>

namespace xchip { namespace utils { 
	
#if defined(__APPLE__) || defined(__linux__)
	
Process::Process() 
{
	
}


Process::~Process()
{
	if(IsRunning())
		Terminate();
}


bool Process::IsRunning() const
{
	return _pid != 0;
}

	
bool Process::Run(const std::string &app) 
{
	using namespace literals;
	
	if (_pid != 0)
		Terminate();

	const auto clean = make_scope_exit([this]() noexcept 
	{ 
		if(_pid == -1) 
			_pid = 0;		
	});


	int fd[2];
	int read_fd, write_fd;

	if(pipe(fd) == -1)
	{
		LogError("Could not open pipe");
		return false;
	}

	read_fd = fd[0];
	write_fd = fd[1];

	_pid = vfork();

	if(_pid == -1)
	{
		LogError("Could not create child process");
		return false;
	}

	
	else if (_pid == 0)
	{
		close(read_fd);
		dup2(write_fd,1);
		close(write_fd);
		if(execl("/bin/sh", "sh", "-c", app.c_str(), NULL))
		{
			LogError("Could not execute command");
			exit(EXIT_FAILURE);
		}

		exit(EXIT_SUCCESS);
	}
	
	else
	{
		close(write_fd);
		Log("Created Child Process...");
	}
	
	return true;
}


int Process::Join()
{
	int status;
	
	const auto clean = make_scope_exit([this]() noexcept { this->_pid = 0; });

	waitpid(_pid, &status, 0);
	
	if(WIFEXITED(status))
		return WEXITSTATUS(status);


	return -1;

}


int Process::Terminate()
{	
	Log("Terminating Process...");
	if(_pid != 0)
	{
		Log("Sending SIGINT.");

		const auto killResult = kill(_pid, SIGINT);

		if(  killResult == ESRCH )
			LogError("Process not found");
		else if( killResult == 1 )
			LogError("Error");

		return Join();
	}

	else
	{
		Log("Process is not running...");
	}

	return 0;
}





#endif // __APPLE__ || __linux__



#if defined(_WIN32)



Process::Process()
{
	ZeroInfo();
}



Process::~Process()
{
	if (IsRunning())
		Terminate();
}


bool Process::Run(const std::string &app)
{	
	if (IsRunning())
		Terminate();

	// Start the child process. 
	if (
		
		!CreateProcess(nullptr, // No module name (use command line)
                       (LPSTR)app.c_str(),    // Command line
                       nullptr,               // Process handle not inheritable
                       nullptr,               // Thread handle not inheritable
                       true,                  // Set handle inheritance to FALSE
                       CREATE_NO_WINDOW,      // creation flags
                       nullptr,               // Use parent's environment block
                       nullptr,               // Use parent's starting directory 
                       &_si,                  // Pointer to STARTUPINFO structure
                       &_pi)                  // Pointer to PROCESS_INFORMATION structure
	  )

	{
		LogError("Could not create a new process!");
		CloseAndZero();
		return false;
	}
	
	return true;
}


int Process::Join()
{

	const auto clean = make_scope_exit([this]() noexcept { CloseAndZero(); });

	DWORD exitCode;

	WaitForSingleObject(_pi.hProcess, INFINITE);
	
	if (!GetExitCodeProcess(_pi.hProcess, &exitCode))
	{
		LogError("Could not get process exit code.");
		return -1;
	}

	return exitCode;
}





int Process::Terminate()
{
	
	if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, _pi.dwProcessId))
	{
		LogError("Sending CTRL_BREAK_EVENT failed. Trying SendMessage WM_CLOSE...");
		EnumWindows((WNDENUMPROC)enum_windows_callback, _pi.dwProcessId);
	}

	return Join();
}




bool Process::IsRunning() const
{
	return _pi.dwProcessId != 0;
}



void Process::ZeroInfo()
{
	ZeroMemory(&_si, sizeof(STARTUPINFO));
	ZeroMemory(&_pi, sizeof(PROCESS_INFORMATION));
	_si.cb = sizeof(STARTUPINFO);
}


void Process::CloseHandles()
{
	CloseHandle(_pi.hProcess);
	CloseHandle(_pi.hThread);
}


void Process::CloseAndZero()
{
	CloseHandles();
	ZeroInfo();
}


bool _stdcall Process::enum_windows_callback(HWND hwnd, LPARAM neededId)
{

	DWORD procId;
	GetWindowThreadProcessId(hwnd, &procId);
	
	if (procId == static_cast<DWORD>(neededId))
	{
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		return false;
	}

	return true;
}








#endif // _WIN32





}}
