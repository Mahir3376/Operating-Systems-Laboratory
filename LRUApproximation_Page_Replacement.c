// LRU-Approximation Page Replacement Algorithm (Second Chance Algorithm)

#include <stdio.h>
#include <stdbool.h>

void lruApproxPageReplacement(int pages[], int num_pages, int frames[], int num_frames) {
    int page_faults = 0;
    int reference_bits[num_frames];
    int pointer = 0;

    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        reference_bits[i] = 0;
    }

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        bool found = false;
        // Check if page is already in frame
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                reference_bits[j] = 1;
                found = true;
                printf("Page %d hit.\n", page);
                break;
            }
        }

        if (!found) {  // Page fault
            // Find next page to replace (second-chance)
            while (reference_bits[pointer] == 1) {
                reference_bits[pointer] = 0;
                pointer = (pointer + 1) % num_frames;
            }
            frames[pointer] = page;
            reference_bits[pointer] = 1;
            pointer = (pointer + 1) % num_frames;
            page_faults++;
            printf("Page %d loaded, causing a page fault.\n", page);
        }
    }
    printf("Total page faults using LRU-Approximation: %d\n", page_faults);
}

int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int num_pages = sizeof(pages) / sizeof(pages[0]);
    int num_frames = 3;
    int frames[num_frames];

    lruApproxPageReplacement(pages, num_pages, frames, num_frames);
    return 0;
}
