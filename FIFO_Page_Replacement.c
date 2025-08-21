// Name: P.Mahir
// Roll No.: 22CSB0C27

// FIFO Page Replacement Algorithm

#include <stdio.h>
#include <stdbool.h>

void fifoPageReplacement(int pages[], int num_pages, int frames[], int num_frames) {
    int page_faults = 0;
    int front = 0;  // Pointer to the oldest page in frames

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        bool found = false;
        // Check if page is already in frame
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }
        // If page is not found, replace the oldest page (FIFO)
        if (!found) {
            frames[front] = page;  // Replace oldest page
            front = (front + 1) % num_frames;  // Move pointer to next frame
            page_faults++;
            printf("Page %d loaded, causing a page fault.\n", page);
        } else {
            printf("Page %d hit.\n", page);
        }
    }
    printf("Total page faults using FIFO: %d\n", page_faults);
}

int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int num_pages = sizeof(pages) / sizeof(pages[0]);
    int num_frames = 3;
    int frames[num_frames];

    for (int i = 0; i < num_frames; i++)
        frames[i] = -1;

    fifoPageReplacement(pages, num_pages, frames, num_frames);
    return 0;
}
