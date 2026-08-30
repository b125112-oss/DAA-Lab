#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

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

/* Returns the k-th smallest element (0-indexed) of arr[left..right]. */
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

const char *ordinalSuffix(int k) {
    if (k % 100 >= 11 && k % 100 <= 13) return "th";
    switch (k % 10) {
        case 1: return "st";
        case 2: return "nd";
        case 3: return "rd";
        default: return "th";
    }
}

int main() {
    int n, k;
    srand((unsigned int) time(NULL));

    printf("===== K-th smallest element WITHOUT sorting (QuickSelect) =====\n\n");
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

    printf("Enter the value of K (1 to %d): ", n);
    if (scanf("%d", &k) != 1 || k < 1 || k > n) {
        printf("Error: K must be an integer between 1 and %d.\n", n);
        free(arr);
        return 1;
    }

    int result = quickSelect(arr, 0, n - 1, k - 1); /* k-1: 0-indexed */

    printf("\nThe %d%s smallest element is: %d\n", k, ordinalSuffix(k), result);

    free(arr);
    return 0;
}