#include <csignal>
#include <string>
#include <algorithm>

#include <XChip/Utility/Log.h>
#include <XChip/Utility/Process.h>


namespace xchip { namespace utility { 
	
#if defined(__APPLE__) || defined(__linux__)
	
Process::Process() 
{
	
}


Process::~Process()
{
	if (pid != 0)
		Terminate();
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
	pid = fork();

	if (pid == 0)
	{

		close(read_fd);
		dup2(write_fd,1);
		close(write_fd);
		execl("/bin/sh", "sh", "-c", app.c_str(), NULL);
		exit(1);
		return;
	}
	
	else
	{
		close(write_fd);
		LOG("In Parent...");
	}

}






bool Process::Run(ProcFunc pfunc, void* arg)
{
	if (pid != 0)
		Terminate();
	
	int fd[2];
	int read_fd, write_fd;
	pipe(fd);
	read_fd = fd[0];
	write_fd = fd[1];
	pid = fork();

	if (pid == 0)
	{
		close(read_fd);
		dup2(write_fd,1);
		close(write_fd);
		exit(pfunc(arg));
		return;
	}

	else
	{
		close(write_fd);
		LOG("In Parent...");
	}

	
}




void Process::Terminate()
{	
	if(pid != 0)
	{
		LOG("Sent signal.")

		int rt_val = kill(pid, SIGINT);
		
		if(rt_val == ESRCH)
			LOG("Process not fuond");

		pid = 0;
	}
}





#endif // __APPLE__ || __linux__



#if defined(_WIN32)



Process::Process()
{

}



Process::~Process()
{
	if (_threadHandle)
		Terminate();
}




bool Run(const std::string &app)
{

}



bool Process::Run(ProcFunc pfunc, void* arg)
{
	if (_threadHandle) {
		LOGerr("Previous process isn't finished yet!");
		return false;
	}

	_threadHandle = (HANDLE) _beginthreadex(nullptr, 0, (_beginthreadex_proc_type)pfunc, arg, 0, &_threadId);
	
	if (_threadHandle == nullptr)
	{
		LOGerr("Could not create Process!");
		return false;
	}

	return true;
}


int Process::Join()
{
	DWORD pfuncReturnCode = 0;

	WaitForSingleObject((HANDLE)_threadHandle, INFINITE);
	GetExitCodeThread(_threadHandle, &pfuncReturnCode);
	CloseHandle(_threadHandle);
	_threadHandle = nullptr;

	return pfuncReturnCode;
}


void Process::Terminate()
{
	if (TerminateThread(_threadHandle, 0) == 0)
		LOGerr("Could not terminate Emulator process...");

	CloseHandle(_threadHandle);
	_threadHandle = nullptr;
}



bool Process::IsRunning() const
{
	if (_threadHandle)
		return GetExitCodeThread(_threadHandle, nullptr) == STILL_ACTIVE;

	return false;
}



#endif // _WIN32





}}
