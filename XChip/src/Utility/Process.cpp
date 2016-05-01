#include <XChip/Utility/Process.h>
#include <string>
#include <sstream>

namespace xchip { namespace utility { 

	
#if defined(__APPLE__) || defined(__linux__)
	
Process::Process() 
{
	
}


Process::~Process()
{
	if(pid != 0)
		Stop();
}

void Process::Run(const std::string &app)
{
	if(pid != 0)
		Stop();
	
	
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
		std::cout << "In Parent..\n";
	}

}






void Process::Run(ProcFunc pfunc, void* arg)
{
	if(pid != 0)
		Stop();	
	
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
		std::cout << "In Parent..\n";
	}

	
}




void Process::Stop()
{	
	if(pid != 0)
	{
		std::cout << "Sent signal.\n";

		int rt_val = kill(pid, SIGINT);
		
		if(rt_val == ESRCH)
			std::cout << "Process not fuond.\n";

		pid = 0;
	}
}





#endif // __APPLE__ || __linux__





}}
