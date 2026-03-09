// LRU Page Replacement Algorithm

#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

void lruPageReplacement(int pages[], int num_pages, int frames[], int num_frames) {
    int page_faults = 0;
    int timestamps[num_frames];  // Store the last used time of each frame

    for (int i = 0; i < num_frames; i++)
        frames[i] = -1;

    for (int i = 0; i < num_pages; i++) {
        int page = pages[i];
        int least_used_index = -1, min_time = INT_MAX;
        bool found = false;
        // Check if page is in frame
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) {
                found = true;
                timestamps[j] = i;  // Update timestamp
                break;
            }
        }

        if (!found) {  // Page fault
            // Find least recently used page
            for (int j = 0; j < num_frames; j++) {
                if (timestamps[j] < min_time) {
                    min_time = timestamps[j];
                    least_used_index = j;
                }
            }
            frames[least_used_index] = page;
            timestamps[least_used_index] = i;
            page_faults++;
            printf("Page %d loaded, causing a page fault.\n", page);
        } else {
            printf("Page %d hit.\n", page);
        }
    }
    printf("Total page faults using LRU: %d\n", page_faults);
}

int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int num_pages = sizeof(pages) / sizeof(pages[0]);
    int num_frames = 3;
    int frames[num_frames];

    lruPageReplacement(pages, num_pages, frames, num_frames);
    return 0;
}

