// Name: P.Mahir
// Roll No: 22CSB0C27

// Implementation of cat command

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // If no arguments are given, we will read from stdin
    if (argc==1) 
    {
        char X;
        while(read(STDIN_FILENO,&X,1)>0) 
        {
            write(STDOUT_FILENO,&X,1);
        }
    } 
    else 
    {
        for (int i=1;i<argc;i++) 
        {
            int fd = open(argv[i],O_RDONLY);
            if (fd < 0) 
            {
                perror(argv[i]);
                continue;
            }
            // Read from the file and write to stdout
            char X;
            while (read(fd,&X,1)>0) 
            {
                write(STDOUT_FILENO,&X,1);
            }
            close(fd);
            write(STDOUT_FILENO,"\n",1);
        }
    }

    return 0;
}
