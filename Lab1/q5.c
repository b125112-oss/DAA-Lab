#include <stdio.h>
#include <stdlib.h>

// Approach 1: Naive Linear Search O(n)
int linearSearchPartition(int arr[], int n, int *steps) {
    for (int i = 0; i < n; i++) {
        (*steps)++; // Track number of loop iterations
        if (arr[i] == 1) {
            return i; // Found the first '1'
        }
    }
    return -1;
}

// Approach 2: Optimized Binary Search O(log n)
int binarySearchPartition(int arr[], int n, int *steps) {
    int low = 0, high = n - 1;
    int first_one_index = -1;
    
    while (low <= high) {
        (*steps)++; // Track number of loop iterations
        int mid = low + (high - low) / 2;
        
        if (arr[mid] == 1) {
            first_one_index = mid; // Record the '1', but keep checking left
            high = mid - 1;
        } else {
            low = mid + 1; // It's a '0', so the transition is to the right
        }
    }
    return first_one_index;
}

void runPartitionAnalysis() {
    // Test drastically increasing array sizes (up to 10 million elements)
    int sizes[] = {10, 100, 1000, 10000, 100000, 1000000, 10000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("Partition Search Complexity Analysis\n");
    printf("------------------------------------------------------------------\n");
    printf("%-12s | %-16s | %-14s | %-14s\n", "Array (n)", "Transition Idx", "Linear Steps", "Binary Steps");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < numSizes; i++) {
        int n = sizes[i];
        int *arr = (int *)malloc(n * sizeof(int));

        // Intentionally place the partition near the end (80% mark) 
        // to force the linear search to scan through most of the array.
        int transitionIdx = (int)(n * 0.8);
        
        for (int j = 0; j < n; j++) {
            if (j < transitionIdx) arr[j] = 0;
            else arr[j] = 1;
        }

        int linearSteps = 0, binarySteps = 0;
        
        linearSearchPartition(arr, n, &linearSteps);
        binarySearchPartition(arr, n, &binarySteps);

        printf("%-12d | %-16d | %-14d | %-14d\n", n, transitionIdx, linearSteps, binarySteps);

        free(arr);
    }
    printf("------------------------------------------------------------------\n");
}

int main() {
    runPartitionAnalysis();
    return 0;
}