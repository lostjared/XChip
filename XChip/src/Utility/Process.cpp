#include <XChip/Utility/Process.h>
#include <string>
#include <sstream>

namespace xchip { namespace utility { 
	
	
Process::Process()
{

#if defined(__APPLE__) || defined(__linux__)
	pid = 0;
#endif
	
}

void Process::Run(const std::string &app)
{
	
#if defined(__APPLE__) || defined(__linux__)
	
	if(pid == 0)
	{
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
	else
	{
		Stop();
		Run(app);
	}
	
#endif
}

void Process::Stop()
{
	if(pid != 0)
	{
		std::cout << "Sent kill signal.\n";
		kill(pid, SIGINT);
		pid = 0;
	}


	
}











}}
