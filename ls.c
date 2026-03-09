// Implementation of ls command

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

void ls_l(char *path, const char *name) {
    struct stat statbuf;
    
    if (stat(path, &statbuf) == -1) {
        perror("stat");
        return;
    }
    
    printf("%c%c%c%c %ld %s %s %ld %s\n",
           (S_ISDIR(statbuf.st_mode)) ? 'd' : '-',   
           (statbuf.st_mode & S_IRUSR) ? 'r' : '-',  
           (statbuf.st_mode & S_IWUSR) ? 'w' : '-',  
           (statbuf.st_mode & S_IXUSR) ? 'x' : '-',  
           statbuf.st_nlink,                        
           getpwuid(statbuf.st_uid)->pw_name,        
           getgrgid(statbuf.st_gid)->gr_name,        
           statbuf.st_size,                          
           name);                                    
}

void ls(char *path) 
{
    int count = 0;
    DIR *dir = opendir(path);  
    struct dirent *d;
    
    if (!dir) 
    {
        perror("opendir");
        return;
    }

    while ((d = readdir(dir)) != NULL) 
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;
        count++;
        printf("%s ", d->d_name); 
    }

    closedir(dir);  
    if (count != 0) 
    {
        printf("\n");  
    }
}

void ls_a(char *path) 
{
    DIR *dir = opendir(path);
    struct dirent *d;
    
    if (!dir) 
    {
        perror("opendir");
        return;
    }

    while ((d = readdir(dir)) != NULL) 
    {
        printf("%s ", d->d_name);
    }
    
    closedir(dir);
    printf("\n");  
}

void ls_R(char *path) {
    DIR *dir = opendir(path);
    printf("%s:\n", path);  
    ls(path);  
    printf("\n");
    
    struct dirent *d;
    
    if (!dir) {
        perror("opendir");
        return;
    }

    while ((d = readdir(dir)) != NULL) 
    {
        if (strcmp(".", d->d_name) == 0 || strcmp("..", d->d_name) == 0) continue;
        if (d->d_type == DT_DIR) 
        {
            char new_path[1024];
            if (snprintf(new_path, sizeof(new_path), "%s/%s", path, d->d_name) >= sizeof(new_path)) {
                fprintf(stderr, "Path too long: %s/%s\n", path, d->d_name);
                continue;
            }
            ls_R(new_path);  
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) 
{
    char path[1024];

    if (argc == 1) 
    {
        snprintf(path, sizeof(path), ".");
        ls(path);
    } 
    else 
    {
        // Handling command-line options
        if (strcmp(argv[1], "-a") == 0) {
            snprintf(path, sizeof(path), ".");
            ls_a(path); 
        } 
        else if (strcmp(argv[1], "-l") == 0) {
            snprintf(path, sizeof(path), ".");
            
            DIR *dir = opendir(path);
            struct dirent *d;
            
            if (!dir) {
                perror("opendir");
                return 1;
            }
            
            while ((d = readdir(dir)) != NULL) {
                if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;
                
                char full_path[1024];
                snprintf(full_path, sizeof(full_path), "%s/%s", path, d->d_name);
                ls_l(full_path, d->d_name);
            }
            
            closedir(dir);
        } 
        else if (strcmp(argv[1], "-R") == 0) 
        {
            snprintf(path, sizeof(path), ".");
            ls_R(path);
        } 
        else 
        {
            // If an argument is provided, we will take it as a path
            snprintf(path, sizeof(path), "%s", argv[1]);
            ls(path);  // List files in the specified directory
        }
    }
    return 0;
}

