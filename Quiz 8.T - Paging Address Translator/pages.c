#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 1024
#define NUM_FRAMES 16

int main(void) {
    const int number_pages = 4;
    int page_table[] = {5, 2, 8, 1};

    // Display paging configuration and page table
    printf("Paging Config:\n");
    printf("PAGE_SIZE = %d bytes\n", PAGE_SIZE);
    printf("NUM_FRAMES = %d (Physical memory = %d bytes)\n", NUM_FRAMES, NUM_FRAMES * PAGE_SIZE);

    printf("Page Table:\n");
    for (int p = 0; p < number_pages; p++) {
        printf("Page %d -> Frame %d\n", p, page_table[p]);
    }
    printf("\n");

    // Get number of logical addresses to process
    int n;
    printf("Enter number of logical addresses (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid N.\n");
        return EXIT_FAILURE;
    }

    // Process each logical address
    printf("Enter %d logical addresses, one per line: \n", n);
    for (int i = 0; i < n; i++) {
        long logical;
        if(scanf("%ld", &logical) != 1) {  
            fprintf(stderr, "Invalid logical address input.\n");
            return EXIT_FAILURE;
        }
        // Check for negative logical address
        if (logical < 0){
            printf(" [#%d] logical = %ld -> INVALID (negative address).\n", i, logical);
            continue;
        }
        // Calculate page number and offset
        long page = logical / PAGE_SIZE;
        long offset = logical % PAGE_SIZE;  
        printf(" [#%d] logical = %ld -> page = %ld, offset = %ld -> ", i, logical, page, offset);

        // Address out of range for this process
        if (page >= number_pages) { 
               printf("-> INVALID (page %ld out of range: valid pages..%d\n", page, number_pages-1);
            continue;
        }

        // Get the frame number from the page table
        int frame = page_table[page];

        // Check if the frame number is valid
        if(frame<0){
            printf("-> Page %ld INVALID \n", page);
            continue;
        }
        // Calculate physical address
        long physical = (long)frame * PAGE_SIZE + offset;
        printf("-> frame = %d, physical = %ld\n", frame, physical);

}

return EXIT_SUCCESS;
}