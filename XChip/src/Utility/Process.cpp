#include <csignal>
#include <string>
#include <algorithm>

#include <XChip/Utility/Log.h>
#include <XChip/Utility/Process.h>
#include <XChip/Utility/ScopeExit.h>
#include <XChip/Utility/Timer.h>

namespace xchip { namespace utility { 
	
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
	return pid != 0;
}

	
bool Process::Run(const std::string &app) 
{

	
	if (pid != 0)
		Terminate();
	
	int fd[2];
	int read_fd, write_fd;
	pipe(fd);
	read_fd = fd[0];
	write_fd = fd[1];
	pid = vfork();
	
	if (pid == 0)
	{
		close(read_fd);
		dup2(write_fd,1);
		close(write_fd);
		if(execl("/bin/sh", "sh", "-c", app.c_str(), NULL))
			exit(EXIT_FAILURE);

		exit(EXIT_SUCCESS);
	}
	
	else
	{
		close(write_fd);
		LOG("In Parent...");
	}
	
	return true;
}


int Process::Join()
{
	int status;
	
	const auto clean = make_scope_exit([this]() noexcept { this->pid = 0; });

	waitpid(pid, &status, 0);
	
	if(WIFEXITED(status))
		return WEXITSTATUS(status);


	return -1;

}


int Process::Terminate()
{	

	if(pid != 0)
	{
		LOG("Sent signal.");

		if(kill(pid, SIGINT) == ESRCH )
			LOG("Process not found");

		return Join();
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
		LOGerr("Could not create a new process!");
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
		LOGerr("Could not get process exit code.");
		return -1;
	}

	return exitCode;
}





int Process::Terminate()
{
	
	if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, _pi.dwProcessId))
	{
		LOGerr("Sending CTRL_BREAK_EVENT failed...");
		LOGerr("Trying SendMessage WM_CLOSE...");
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
