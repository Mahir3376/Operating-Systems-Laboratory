#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

int main(int agrc,char *argv[]) {
	bool verf=false; int signal=15;
	for (int i=1;i<agrc;i++) {
		if (i==1 && argv[1][0]=='-') {
			verf=true;
			signal=atoi((*(argv+1))+1);
			//printf("%d signal\n",signal);
		}
		
		else { 
			if (kill(atoi(argv[i]),signal)>=0) {
				printf("signaled succesful for PID: %s\n",argv[i]);
			} else {
				printf("signal failed for PID: %s\n",argv[i]);
			}
		}
	}
		
	return 0;
}
