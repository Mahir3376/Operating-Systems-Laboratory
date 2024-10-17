#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>

int main(int agrc,char *argv[]) {
	bool lineflag=false,wordflag=false,charflag=false;
	int ttline=0,ttword=0,ttchar=0,fdc=0;
	for (int i=1;i<agrc;i++) {
		if (i==1 && strcmp(argv[1],"-l")==0) {
			lineflag=true;
			continue;
		} 
		
		if (i==1 && strcmp(argv[1],"-w")==0) {
			wordflag=true;
			continue;
		}
		
		if (i==1 && strcmp(argv[1],"-c")==0) {
			charflag=true;
			continue;
		}
		
		int fd=open(argv[i],O_RDONLY);
		char X; int ltrc=0,wdc=0,lnc=0;
		while (read(fd,&X,1)>0) {
			if (X=='\n') {
				lnc++;
				wdc++; 
			} else if (X==' ') {
				wdc++; 
			} /*else { //check for multispace
				ltrc++;
			}*/
			ltrc++;
		} fdc++;
		
		if (!lineflag && !wordflag && !charflag) printf("%d %d %d %s \n",lnc,wdc,ltrc,argv[i]);
		else {
			if (lineflag) printf("%d %s \n",lnc,argv[i]);
			else if (wordflag) printf("%d %s \n",wdc,argv[i]);
			else if (charflag) printf("%d %s \n",ltrc,argv[i]);
		}
		ttline+=lnc;
		ttword+=wdc;
		ttchar+=ltrc;
	}
	if (fdc>1) {
		if (!lineflag && !wordflag && !charflag) printf("%d %d %d Total\n",ttline,ttword,ttchar);
		else {
			if (lineflag) printf("%d Total\n",ttline);
			else if (wordflag) printf("%d Total\n",ttword);
			else if (charflag) printf("%d Total\n",ttchar);
		}
	}
	
	return 0;
}
