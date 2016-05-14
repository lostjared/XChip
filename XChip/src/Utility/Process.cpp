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
		execl("/bin/sh", "sh", "-c", app.c_str(), NULL);
		exit(1);
		return true;
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
	
	waitpid(pid, &status, 0);
	
	if(WIFEXITED(status))
		return WEXITSTATUS(status);


	pid = 0;

	return -1;

}


void Process::Terminate()
{	

	if(pid != 0)
	{
		LOG("Sent signal.");

		if(kill(pid, SIGINT) == ESRCH )
			LOG("Process not found");

		pid = 0;
	}

}





#endif // __APPLE__ || __linux__



#if defined(_WIN32)



Process::Process()
{
	ZeroInf();
}



Process::~Process()
{
	if (IsRunning())
		Terminate();
}


bool Process::Run(const std::string &app)
{
	std::string appCpy = app;
	
	if (IsRunning())
		Terminate();

	// Start the child process. 
	if (
		
		!CreateProcess(nullptr, // No module name (use command line)
		               (LPSTR)appCpy.c_str(),  // Command line
		               nullptr,                // Process handle not inheritable
		               nullptr,                // Thread handle not inheritable
		               true,                  // Set handle inheritance to FALSE
		               CREATE_NEW_PROCESS_GROUP,     // creation flags
		               nullptr,                // Use parent's environment block
		               nullptr,                // Use parent's starting directory 
		               &_si,                   // Pointer to STARTUPINFO structure
		               &_pi)                   // Pointer to PROCESS_INFORMATION structure
	  )

	{
		LOGerr("Could not create a new process!");
		ZeroInf();
		return false;
	}
	
	CloseHandle(_pi.hThread);
	
	return true;
}


int Process::Join()
{
	DWORD exitCode;
	WaitForSingleObject(_pi.hProcess, INFINITE);
	if (!GetExitCodeProcess(_pi.hProcess, &exitCode))
	{
		LOGerr("Could not get process exit code.");
		return -1;
	}

	ZeroInf();
	return exitCode;
}


void Process::Terminate()
{
	if (!GenerateConsoleCtrlEvent(CTRL_C_EVENT, _pi.dwProcessId))
	{
		LOGerr("Sending CTRL_C_EVENT failed. Try SendMessage WM_CLOSE");
		if (!SendMessage(FindWindow(0, "EmuApp"), WM_CLOSE, 0, 0))
		{
			LOGerr("Sending Message failed, terminating process by force");
			TerminateProcess(_pi.hProcess, -1);
		}
	}

	Join();
}




bool Process::IsRunning() const
{
	return _pi.dwProcessId != 0;
}



void Process::ZeroInf()
{
	CloseHandle(_pi.hProcess);
	CloseHandle(_pi.hThread);
	ZeroMemory(&_si, sizeof(_si));
	_si.cb = sizeof(_si);
	ZeroMemory(&_pi, sizeof(_pi));
}

#endif // _WIN32





}}
