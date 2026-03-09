// Optimal Page Replacement Algorithm

#include <stdio.h>
#include <stdbool.h>

int findOptimal(int pages[], int num_pages, int frames[], int num_frames, int current) {
    int farthest = current;
    int frame_index = -1;

    for (int i = 0; i < num_frames; i++) {
        int j;
        for (j = current + 1; j < num_pages; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    frame_index = i;
                }
                break;
            }
        }
        if (j == num_pages)
            return i;
    }
    return (frame_index == -1) ? 0 : frame_index;
}

void optimalPageReplacement(int pages[], int num_pages, int frames[], int num_frames) {
    int page_faults = 0;

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
        // If page not found, replace using Optimal
        if (!found) {
            int replace_index = (i < num_frames) ? i : findOptimal(pages, num_pages, frames, num_frames, i);
            frames[replace_index] = page;
            page_faults++;
            printf("Page %d loaded, causing a page fault.\n", page);
        } else {
            printf("Page %d hit.\n", page);
        }
    }

    printf("Total page faults using Optimal: %d\n", page_faults);
}

int main() {
    int pages[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int num_pages = sizeof(pages) / sizeof(pages[0]);
    int num_frames = 3;
    int frames[num_frames];

    for (int i = 0; i < num_frames; i++)
        frames[i] = -1;

    optimalPageReplacement(pages, num_pages, frames, num_frames);
    return 0;
}

