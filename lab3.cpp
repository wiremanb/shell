#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

void parseMe(char input[], char *output[], const char *delim)
{
	char **next = output;
	char *temp = strtok(input, delim);
	while (temp != NULL)
	{
		*next++ = temp;
		temp = strtok(NULL, delim);
	}
	*next = NULL;
}

void executeMe(char *input[])
{
	execvp(input[0], input);
	std::cout << "[-] Error running command";
	exit(1);
}

void runMe(char input[])
{
	if(fork() == 0)
	{
		char *args[10];
		parseMe(input, args, " ");
		executeMe(args);
	} else wait(0);
}

void pipeMe(char input[])
{
	char *splitPipe[10];
	int pipefd[2], i=0;
	parseMe(input, splitPipe, "|");
	pipe(pipefd);
	if(fork()==0)
	{
		char *args[10];
		parseMe(splitPipe[0], args, " ");
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		executeMe(args);
	} 
	if(fork()==0)
	{
		char *args[10];
		parseMe(splitPipe[1], args, " ");
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
		executeMe(args);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	wait(0);
	wait(0);
}

int main(int argc, char* argv[])
{
	bool quit = false;
	char userInput[1024];
	while(!quit)
	{
		std::cout << "Lab3 >> " << std::flush;
		std::cin.getline(userInput, sizeof(userInput));
		if(std::string(userInput).find("quit") != std::string::npos || std::string(userInput).find("exit") != std::string::npos) { quit = true; }
		else if(std::string(userInput).find("|") != std::string::npos) { pipeMe(userInput); }
		else if(std::string(userInput).find(">>") != std::string::npos) 
		{
			std::cout << ">>" << std::endl;
		}
		else if(std::string(userInput).find(">") != std::string::npos) 
		{
			std::cout << ">" << std::endl;
		}
		else if(std::string(userInput).find("history") != std::string::npos) 
		{
			std::cout << "history" << std::endl;
		}
		else { runMe(userInput); }
	}	
	return 0;
}
