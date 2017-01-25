#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void sub(int x, int y) { std::cout << x << " - " << y << " = " << (x-y) << std::endl; }
void mul(int x, int y) { std::cout << x << " * " << y << " = " << (x*y) << std::endl; }
void add(int x, int y) { std::cout << x << " + " << y << " = " << (x+y) << std::endl; }

int main(int argc, char* argv[])
{
	pid_t c_pid;
	std::cout << "Initial Process: " << getpid() << std::endl;
	c_pid = fork();
	if(c_pid==0)
	{
		std::cout << "Parent: " << getppid() << " -> Child: " << getpid() << std::endl;
		sub(2,1);
		pid_t c2_pid = fork();
		if(c2_pid==0)
		{
			std::cout << "Parent: " << getppid() << " -> Child: " << getpid() << std::endl;
			add(2,1);
			exit(0);
		}
		else { wait(0); }

		std::cout << "Parent: " << getppid() << " -> Child: " << getpid() << std::endl;
		mul(2,1);
		exit(0);
	}
	else { wait(0); }
	std::cout << "Initial process: " << getpid() << std::endl;
	exit(0);
}
