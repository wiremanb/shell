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
	std::cout << "[-] Error running command" << std::flush;
}

int main(int argc, char* argv[])
{
	bool quit = false;
	char userInput[1024];
	while(!quit)
	{
		std::cout << std::endl << "Lab3 >> ";
		std::cin.getline(userInput, sizeof(userInput));
		if(std::string(userInput).find("quit") != std::string::npos || std::string(userInput).find("exit") != std::string::npos) quit = true;
		else if(std::string(userInput).find("|") != std::string::npos) 
		{
			char *splitPipe[256];
			int pipefd[2], i=0;
			parseMe(userInput, splitPipe, "|");
			pipe(pipefd);
			// while(splitPipe[i] != NULL && i<1)
			// {
				// printf("main: %s\n", splitPipe[1]);
				if(fork()==0)
				{
					char *args[256];
					parseMe(splitPipe[0], args, " ");
					// std::cout << std::endl << "First set: " << std::flush;
					// int j=0;
					// while(args[j]!=NULL)
					// 	printf("%s, ", args[j++]);
					dup2(pipefd[1], STDOUT_FILENO);
        			close(pipefd[1]);
					executeMe(args);
					exit(0);
					//*args=NULL;
				} 
				else 
				{
					wait(0);
					char *args[256];
					parseMe(splitPipe[1], args, " ");
					// std::cout << std::endl << "Second set: " << std::flush;
					// int j=0;
					// while(args[j]!=NULL)
					// 	printf("%s, ", args[j++]);
					dup2(pipefd[0], STDIN_FILENO);
					close(pipefd[0]);
					executeMe(args);
					close(pipefd[1]);
					//*args=NULL;
				}
				// while(args[j] != NULL)
				// 	printf("%s\n", args[j++]);
				// i++;
			//}
		}
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
		else 
		{		
			if(fork() == 0)
			{
				char *args[256];
				parseMe(userInput, args, " ");
				executeMe(args);
				exit(0);
			} else {wait(0);}
		}
	}	
	return 0;
}
