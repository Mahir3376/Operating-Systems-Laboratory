// Name: P.Mahir
// Roll No: 22CSB0C27

// Question 2 Command Interpreter

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100

// Function to parse the user input into arguments and checking for background
void parse_input(char *input, char **args, int *background) 
{
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) 
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (i > 0 && strcmp(args[i - 1], "&") == 0) 
    {
        *background = 1;
        args[i - 1] = NULL;
    } else {
        *background = 0;
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    int background;

    while (1) {
        printf("$ ");
        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) 
        {
            perror("fgets");
            continue;
        }

        input[strcspn(input, "\n")] = 0;
        parse_input(input, args, &background);
        if (args[0] != NULL && strcmp(args[0], "exit") == 0) 
        {
            break;
        }
        int input_redirect = -1, output_redirect = -1;
        
        // check for i/o redirection
        for (int i = 0; args[i] != NULL; i++) 
        {
            if (strcmp(args[i], "<") == 0) 
            {
                input_redirect = open(args[i + 1], O_RDONLY);
                if (input_redirect < 0) 
                {
                    perror("open");
                }
                args[i] = NULL;
            } 
            else if (strcmp(args[i], ">") == 0) 
            {
                output_redirect = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_redirect < 0) 
                {
                    perror("open");
                }
                args[i] = NULL;
            }
        }

	// Child process creation to execute commands
        pid_t pid = fork();
        if (pid < 0) 
        {
            perror("fork failed");
            exit(1);
        } else if (pid == 0) 
        { 
            if (input_redirect != -1) 
            {
                dup2(input_redirect, STDIN_FILENO);
                close(input_redirect);
            }
            if (output_redirect != -1) 
            {
                dup2(output_redirect, STDOUT_FILENO);
                close(output_redirect);
            }
            execvp(args[0], args);
            perror("execvp");
            exit(1);
        } 
        else 
        { 
        // parent process
            if (background) 
            {
                continue;
            } 
            else 
            {
                wait(NULL);
            }
        }
    }

    return 0;
}
