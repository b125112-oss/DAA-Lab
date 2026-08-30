

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Lomuto partition scheme with a RANDOM pivot to avoid worst-case
   behaviour on already-sorted or adversarial input. */
int partition(int arr[], int left, int right) {
    int randIndex = left + rand() % (right - left + 1);
    swap(&arr[randIndex], &arr[right]);
    int pivot = arr[right];
    int i = left - 1;

    for (int j = left; j < right; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[right]);
    return i + 1;
}

/* Returns the k-th smallest element (0-indexed) of arr[left..right].
   The array is partially rearranged in the process, but it is never
   fully sorted. */
int quickSelect(int arr[], int left, int right, int k) {
    if (left == right)
        return arr[left];

    int pivotIndex = partition(arr, left, right);

    if (k == pivotIndex)
        return arr[k];
    else if (k < pivotIndex)
        return quickSelect(arr, left, pivotIndex - 1, k);
    else
        return quickSelect(arr, pivotIndex + 1, right, k);
}

int *copyArray(int arr[], int n) {
    int *copy = (int *) malloc(n * sizeof(int));
    if (copy != NULL) {
        for (int i = 0; i < n; i++)
            copy[i] = arr[i];
    }
    return copy;
}

int main() {
    int n;
    srand((unsigned int) time(NULL));

    printf("===== Median of N numbers WITHOUT sorting (QuickSelect) =====\n\n");
    printf("Enter the number of elements (N): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Error: N must be a positive integer.\n");
        return 1;
    }

    int *arr = (int *) malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Error: memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d integer(s):\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Error: invalid input.\n");
            free(arr);
            return 1;
        }
    }

    double median;

    if (n % 2 == 1) {
        /* Odd N: single middle element */
        int *copy1 = copyArray(arr, n);
        if (copy1 == NULL) { printf("Memory allocation failed.\n"); free(arr); return 1; }
        int mid = quickSelect(copy1, 0, n - 1, n / 2);
        median = (double) mid;
        free(copy1);
    } else {
        /* Even N: average of the two middle elements.
           We use fresh copies for each QuickSelect call since the
           array gets partially rearranged during selection. */
        int *copy1 = copyArray(arr, n);
        int *copy2 = copyArray(arr, n);
        if (copy1 == NULL || copy2 == NULL) {
            printf("Memory allocation failed.\n");
            free(arr); free(copy1); free(copy2);
            return 1;
        }
        int mid1 = quickSelect(copy1, 0, n - 1, n / 2 - 1);
        int mid2 = quickSelect(copy2, 0, n - 1, n / 2);
        median = (mid1 + mid2) / 2.0;
        free(copy1);
        free(copy2);
    }

    printf("\nMedian of the given %d elements = %.2lf\n", n, median);

    free(arr);
    return 0;
}