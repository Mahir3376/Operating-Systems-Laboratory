#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int agrc,char *argv[]) {
	bool verf=false;
	for (int i=1;i<agrc;i++) {
		if (i==1 && strcmp(argv[1],"-i")==0) verf=true;
		
		else {
			if (verf) {
				printf("Do you want to remove %s y/n :\n",argv[i]);
				char x;
				scanf(" %c",&x);
				if (x=='n') continue;
			}
			 
			if (remove(argv[i])>=0) {
				printf("%s removed succesfully\n",argv[i]);
			} else {
				printf("%s file not present\n",argv[i]);
			}
		}
	}
		
	return 0;
}
