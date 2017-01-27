/*
 * Author: Benjamin Wireman
 * Date: 26 Jan 2017
 * Description: Basic shell.. provides some functionality but not all.  
 */

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/stat.h> 

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

int executeMe(char *input[])
{
	execvp(input[0], input);
	std::cout << "[-] Error running command";
	return -1;
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

void redirectMe(char input[], int opt)
{
	int fd;
	char *splitRd[10], *args[10]; 
	parseMe(input, splitRd, ">");
	parseMe(splitRd[1], args, " "); // To get rid of an extra space....... wasted so much time on this.
	if(opt==0)
		fd = open(args[0], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
		fd = open(args[0], O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	parseMe(splitRd[0], args, " ");
	if(fork()==0)
	{
		close(1);
		dup2(fd, 1);
		close(fd);
		executeMe(args);
	} else wait(0);
}

void redirectAPipe(char input[])
{
	int fd, fdup=dup(1);
	char *splitRd[10], *args[10]; 
	parseMe(input, splitRd, ">");
	parseMe(splitRd[1], args, " "); // To get rid of an extra space....... wasted so much time on this.
	fd = open(args[0], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	close(1);
	dup2(fd, 1);
	close(fd);
	pipeMe(splitRd[0]);
	dup2(fdup, 1);
	close(fdup);
}

int main(int argc, char* argv[])
{
	bool quit = false;
	char userInput[1024];
	while(!quit)
	{
		memset(&userInput[0], 0, sizeof(userInput));
		std::cout << "Lab3 >> " << std::flush;
		std::cin.getline(userInput, sizeof(userInput));
		if(std::string(userInput).find("quit") != std::string::npos || std::string(userInput).find("exit") != std::string::npos) { quit = true; }
		else if(std::string(userInput).find("|") != std::string::npos && std::string(userInput).find(">") == std::string::npos) { pipeMe(userInput); }
		else if(std::string(userInput).find(">") != std::string::npos && std::string(userInput).find("|") == std::string::npos) { redirectMe(userInput, 0); }
		else if(std::string(userInput).find("|") != std::string::npos && std::string(userInput).find(">") != std::string::npos) { redirectAPipe(userInput); }
		else { runMe(userInput); }
	}	
	return 0;
}
