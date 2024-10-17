#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>

// Function to get the username from the UID
char* get_username(uid_t uid) {
    struct passwd *pw=getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return NULL;
}

// Function to print process info
void print_process_info(const char* pid, int show_all, int extended, const char* username_filter) {
    char stat_file[256];
    snprintf(stat_file, sizeof(stat_file), "/proc/%s/stat", pid);
    FILE* fp=fopen(stat_file, "r");

    if (!fp) {
        return;  // Skip if we can't open the stat file (e.g., process no longer exists)
    }

    // Reading process information from /proc/[pid]/stat
    int pid_num;
    char comm[256], state;
    int ppid, pgrp, session, tty_nr, tpgid;
    unsigned flags;
    uid_t uid;
    fscanf(fp, "%d %s %c %d %d %d %d %d", &pid_num, comm, &state, &ppid, &pgrp, &session, &tty_nr, &tpgid);
    fclose(fp);

    // Get the UID and username of the process
    char status_file[256];
    snprintf(status_file, sizeof(status_file), "/proc/%s/status", pid);
    fp=fopen(status_file, "r");
    if (fp) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "Uid:", 4)==0) {
                sscanf(buffer, "Uid:\t%d", &uid);
                break;
            }
        }
        fclose(fp);
    } else {
        return;  // Skip if we can't open the status file
    }

    char* username=get_username(uid);

    // Skip processes that don't match the username filter if specified
    if (username_filter && strcmp(username_filter, username)!=0) {
        return;
    }

    // Skip non-terminal processes unless `-a` or `-e` is specified
    if (!extended && !show_all && tty_nr==0) {
        return;
    }

    // Print process details
    if (extended) {
        printf("%-5d %-10s %-8s %-5c\n", pid_num, username, comm, state);
    } else {
        printf("%-5d %-8s %-5c\n", pid_num, comm, state);
    }
}

// Function to list processes based on flags
void list_processes(int show_all, int extended, const char* username_filter) {
    DIR* proc=opendir("/proc");
    if (!proc) {
        perror("opendir");
        return;
    }

    struct dirent* entry;
    printf("%-5s %-10s %-8s %-5s\n", "PID", extended ? "USER" : "", "COMMAND", "STAT");

    while ((entry=readdir(proc))!=NULL) {
        // Only consider directories that are PID numbers
        if (isdigit(entry->d_name[0])) {
            print_process_info(entry->d_name, show_all, extended, username_filter);
        }
    }
    closedir(proc);
}

int main(int argc, char* argv[]) {
    int show_all=0, extended=0;
    char* username_filter=NULL;

    // Parse command-line options
    for (int i=1; i<argc && i!=2; i++) {
        if (argv[i][0]=='-') {
            for (int j=1; argv[i][j]!='\0'; j++) {
                switch (argv[i][j]) {
                    case 'a':
                        show_all=1;
                        break;
                    case 'e':
                        extended=1;
                        break;
                    case 'u':
                        if (i+1<argc) {
                            username_filter=argv[i+1];
                        } else {
                            fprintf(stderr, "Option -u requires an argument.\n");
                            exit(EXIT_FAILURE);
                        }
                        break;

                    default:
                        fprintf(stderr, "Unknown option: %c\n", argv[i][j]);
                        exit(EXIT_FAILURE);
                }
            }
        }
    }

    // List the processes based on the options
    list_processes(show_all, extended, username_filter);

    return 0;
}
