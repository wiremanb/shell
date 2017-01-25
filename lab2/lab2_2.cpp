#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	pid_t c_pid;
	if(argc > 1)
	{
		int x = atoi(argv[1]);
		if((x%2)==0)
		{
			std::cout << "The PID of the parent is: " << getpid() << std::endl;
			for(size_t i=0; i<x; i++)
			{
				c_pid = fork();
				if(c_pid==0)
				{
					std::cout << "I am a child; My PID is: " << getpid() << std::endl;
					exit(0);
				}
			}
		}
		else
			std::cout << "[-] Invalid input -> Expected even number input, received odd number input. Suggested use: ./test <int>" << std::endl;
	}
	else
		std::cout << "[-] Invalid input -> Missing number of child processes. Suggested use: ./test <int>" << std::endl;
		exit(-1);
	exit(0);
}
