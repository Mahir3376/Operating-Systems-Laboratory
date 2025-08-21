// Name: P.Mahir
// Roll No: 22CSB0C27

// Implementation of wc command

# include<stdio.h>
# include<ctype.h>

int newline=0,wc=0,byteCount=0; // Global counters for total counts

void WC(char* path) 
{
    FILE *file = fopen(path, "r");
    
    if (file == NULL) 
    {
        printf("Error opening file\n");
        return;
    }

    int curr_newline = 0, curr_wc = 0, curr_byteCount = 0;
    int isWord = 0; // Flag to track if we are inside a word

    char ch;
    while((ch = fgetc(file)) != EOF) 
    {
        curr_byteCount ++;

        if (ch == '\n') 
        {
            curr_newline ++;
            isWord = 0;
        } 
        else if (isspace(ch)) 
        {
            isWord = 0;
        } 
        else if (isWord == 0) 
        {
            curr_wc ++; // Starting a new word
            isWord = 1;
        }
    }

    printf("%d %d %d %s\n", curr_newline, curr_wc, curr_byteCount, path);
    
    // Update global counters
    newline += curr_newline;
    wc += curr_wc;
    byteCount += curr_byteCount;
}

int main(int argc, char* argv[]) 
{
   // We will process each file provided as an argument    
    for (int i = 1; i < argc; i ++) 
        WC(argv[i]);
  // If multiple files were processed, we will print the total counts
    if (argc > 2) 
        printf("%d %d %d total\n", newline, wc, byteCount);

    return 0;
}
