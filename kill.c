// Implementation of kill command

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int main(int argc,char *argv[]) 
{
	bool flag=false; 
	int signal=15; // SIGTERM
	for (int i=1;i<argc;i++) 
	{
	// If the first argument starts with '-',it's a signal option
		if (i==1 && argv[1][0]=='-') 
		{
			flag=true;
			signal=atoi((*(argv+1))+1);
		}
		else 
		{ 
		// Attempt to kill the process with the specified signal
			if (kill(atoi(argv[i]),signal)>=0) 
			{
				printf("signaled succesful for PID: %s\n",argv[i]);
			} 
			else 
			{
				printf("signal failed for PID: %s\n",argv[i]);
			}
		}
	}
	return 0;
}
