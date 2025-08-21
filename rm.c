// Name: P.Mahir
// Roll No: 22CSB0C27

// Implementation of rm command

# include<stdio.h>
# include<string.h>

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
        return 0; // No files provided for removal
    if (argc == 2)
        remove(argv[1]); // Directly removing the file given as argument
    else if (strcmp("-i", argv[1]) == 0) 
    {
        for (int i = 2; i < argc; i ++) 
        {
            printf("rm: remove file '%s'? ", argv[i]); // prompt before removal
            char ch;
            scanf(" %c", &ch);
            if (ch == 'Y' || ch == 'y') 
                remove(argv[i]);
            else if (ch == 'N' || ch == 'n') 
                continue;
        }
    } 
    else 
    {
        for (int i = 1; i < argc; i ++) 
            remove(argv[i]);
    }

    return 0;
}
