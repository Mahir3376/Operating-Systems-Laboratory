#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Helper function to print file permissions
void print_permissions(mode_t mode) {
    printf( (S_ISDIR(mode)) ? "d" : "-");
    printf( (mode & S_IRUSR) ? "r" : "-");
    printf( (mode & S_IWUSR) ? "w" : "-");
    printf( (mode & S_IXUSR) ? "x" : "-");
    printf( (mode & S_IRGRP) ? "r" : "-");
    printf( (mode & S_IWGRP) ? "w" : "-");
    printf( (mode & S_IXGRP) ? "x" : "-");
    printf( (mode & S_IROTH) ? "r" : "-");
    printf( (mode & S_IWOTH) ? "w" : "-");
    printf( (mode & S_IXOTH) ? "x" : "-");
}

// Function to print file details in `-l` format
void print_file_info(const char *dir_name, const char *file_name) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir_name, file_name);

    struct stat file_stat;
    if (stat(path, &file_stat)==0) {
        print_permissions(file_stat.st_mode);
        printf(" %ld", file_stat.st_nlink);
        printf(" %d", file_stat.st_uid);
        printf(" %d", file_stat.st_gid);
        printf(" %5ld", file_stat.st_size);

        char time_str[20];
        struct tm *time_info;
        time_info=localtime(&file_stat.st_mtime);
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", time_info);
        printf(" %s",time_str);

        printf(" %s\n",file_name);
    }
}

// Function to list directory contents
void list_dir(const char *dirname, int show_all, int long_format, int recursive) {
    DIR *dir=opendir(dirname);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry=readdir(dir))!=NULL) {
        // Skip hidden files unless `-a` is specified
        if (!show_all && entry->d_name[0]=='.') {
            continue;
        }

        if (long_format) {
            print_file_info(dirname, entry->d_name);
        } else {
            printf("%s  ", entry->d_name);
        }
    }
    printf("\n");
    closedir(dir);

    // If recursive (-R) option is specified, list subdirectories
    if (recursive) {
        dir=opendir(dirname);
        while ((entry=readdir(dir))!=NULL) {
            if (entry->d_type==DT_DIR && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0) {
                char path[1024];
                snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
                printf("\n%s:\n", path);
                list_dir(path, show_all, long_format, recursive);
            }
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    int show_all=0, long_format=0, recursive=0;
    char *dirname=".";

    // Parse command-line options
    for (int i=1;i<argc;i++) {
        if (argv[i][0]=='-') {
            for (int j=1; argv[i][j]!='\0'; j++) {
                switch (argv[i][j]) {
                    case 'a':
                        show_all=1;
                        break;
                    case 'l':
                        long_format=1;
                        break;
                    case 'R':
                        recursive=1;
                        break;
                    default:
                        fprintf(stderr, "Unknown option: %c\n", argv[i][j]);
                        exit(EXIT_FAILURE);
                }
            }
        } else {
            dirname=argv[i];
        }
    }

    // List the directory
    list_dir(dirname, show_all, long_format, recursive);

    return 0;
}
