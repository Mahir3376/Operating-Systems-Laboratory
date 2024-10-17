#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

int main(int agrc,char *argv[]) {
	if (agrc==1) {
		char X;
		while (read(0,&X,1)>0) {
			printf("%c",X);
		}
	}
	
	for (int i=1;i<agrc;i++) {
		int fd=open(argv[i],O_RDONLY);
		char X;
		while (read(fd,&X,1)>0) {
			printf("%c",X);
		}
		
		printf("\n");
	}
	
	return 0;
}
