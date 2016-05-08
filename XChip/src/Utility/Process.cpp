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
	pid = fork();
	
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

}



Process::~Process()
{
	if (IsRunning())
		Terminate();
}


bool Process::Run(const std::string &app)
{
	std::string appCpy = app;
	CreateProcess(nullptr, (LPSTR)appCpy.c_str(), nullptr,
		          nullptr, false, 0, nullptr,
		          nullptr, nullptr, nullptr);
	
	return false;
}


int Process::Join()
{
	return 0;
}


void Process::Terminate()
{
	
}



bool Process::IsRunning() const
{
	return false;
}



#endif // _WIN32





}}
