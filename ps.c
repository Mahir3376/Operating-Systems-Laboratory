// Implementation of ps command

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc, char *argv[]) {
    int c = fork();

    if (c<0) 
    {
        perror("fork failed");
        return 1;
    } 
    else if (c == 0) 
    {
        char *args[4]; // Enough space for arguments

	 // Handling different command-line arguments for ps
        if (argc == 1) {
            args[0] = "ps";  // Command
            args[1] = NULL;  // Terminate argument list
        } else if (argc == 2 && strcmp(argv[1], "-a") == 0) {
            args[0] = "ps";
            args[1] = "-a";  // List all the processes
            args[2] = NULL;
        } else if (argc == 3 && strcmp(argv[1], "-u") == 0) {
            args[0] = "ps";
            args[1] = "-u";
            args[2] = argv[2]; // Username from command line
            args[3] = NULL;
        } else if (argc == 2 && strcmp(argv[1], "-ae") == 0) {
            args[0] = "ps";
            args[1] = "-ae"; // List all processes with details
            args[2] = NULL;
        } else {
            fprintf(stderr, "Usage: %s [options]\n", argv[0]);
            }
            
        execv("/bin/ps", args);
        perror("execv failed"); // If execv fails
        exit(1);
    } 
    else 
    {
        wait(NULL);
    }
    return 0;
}
