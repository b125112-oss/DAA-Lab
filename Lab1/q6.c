#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to count the total number of duplicate elements
int countDuplicates(int arr[], int n) {
    if (n <= 1) return 0;

    // Step 1: Find the maximum value in the array to appropriately size the frequency array
    int max_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    // Step 2: Allocate a frequency array initialized to 0
    int *freq = (int *)calloc(max_val + 1, sizeof(int));
    if (freq == NULL) {
        return -1;
    }

    int duplicate_count = 0;
    
    // Step 3: Linearly scan the original array to count duplicates
    for (int i = 0; i < n; i++) {
        if (freq[arr[i]] > 0) {
            // If the frequency is already greater than 0, we have seen this number before
            duplicate_count++;
        }
        freq[arr[i]]++; // Mark this number as seen
    }
    
    free(freq);
    
    return duplicate_count;
}

int main() {
    int size_arr, x;

    size_arr = 100;
    x = 1000; // x > size_arr so that we may have duplicates

    // Dynamically allocate memory based on size_arr
    int *arr = (int *)malloc(size_arr * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    srand(time(NULL));

    // Generate random values between 0 and x (inclusive)
    for (int i = 0; i < size_arr; i++) {
        arr[i] = rand() % (x + 1); 
    }

    int total_duplicates = countDuplicates(arr, size_arr);
    
    printf("Result: Found %d duplicate(s) in the array.\n", total_duplicates);

    free(arr);

    return 0;
}