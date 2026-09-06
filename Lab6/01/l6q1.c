#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int findMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

void findFirstAndSecondLargest(int arr[], int n, int *first, int *second) {
    *first = arr[0];
    *second = arr[0];

    for (int i = 1; i < n; i++) {
        if (arr[i] > *first) {
            *second = *first;
            *first = arr[i];
        } else if (arr[i] > *second && arr[i] != *first) {
            *second = arr[i];
        }
    }
}

double findMean(int arr[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return (double) sum / n;
}

void sortArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[i]) {
                swap(&arr[i], &arr[j]);
            }
        }
    }
}

double findMedian(int arr[], int n) {
    int *temp = malloc(n * sizeof(int));
    if (!temp) return 0.0;
    for (int i = 0; i < n; i++) temp[i] = arr[i];
    sortArray(temp, n);

    double med;
    if (n % 2 == 0) {
        med = (temp[n / 2 - 1] + temp[n / 2]) / 2.0;
    } else {
        med = temp[n / 2];
    }
    free(temp);
    return med;
}

double findStandardDeviation(int arr[], int n) {
    double mean = findMean(arr, n);
    double variance = 0.0;

    for (int i = 0; i < n; i++) {
        variance += (arr[i] - mean) * (arr[i] - mean);
    }

    variance /= n;
    return sqrt(variance);
}

int findMode(int arr[], int n) {
    int *temp = malloc(n * sizeof(int));
    int *freq = malloc(n * sizeof(int));
    if (!temp || !freq) { free(temp); free(freq); return 0; }
    int uniqueCount = 0;

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < uniqueCount; j++) {
            if (temp[j] == arr[i]) { found = 1; break; }
        }
        if (!found) temp[uniqueCount++] = arr[i];
    }

    for (int i = 0; i < uniqueCount; i++) freq[i] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < uniqueCount; j++) {
            if (temp[j] == arr[i]) { freq[j]++; break; }
        }
    }

    int mode = temp[0];
    int maxFreq = freq[0];
    for (int i = 1; i < uniqueCount; i++) {
        if (freq[i] > maxFreq) { maxFreq = freq[i]; mode = temp[i]; }
    }

    free(temp); free(freq);
    return mode;
}

int removeDuplicates(int arr[], int n, int result[]) {
    int uniqueCount = 0;
    for (int i = 0; i < n; i++) {
        int exists = 0;
        for (int j = 0; j < uniqueCount; j++) {
            if (result[j] == arr[i]) {
                exists = 1;
                break;
            }
        }
        if (!exists) {
            result[uniqueCount++] = arr[i];
        }
    }
    return uniqueCount;
}

void reverseArray(int arr[], int n) {
    for (int i = 0; i < n / 2; i++) {
        swap(&arr[i], &arr[n - 1 - i]);
    }
}

int partitionAroundPivot(int arr[], int low, int high, int pivot) {
    /* Ensure the pivot value is placed at arr[high] before doing Lomuto partition.
       If pivot value is not found in arr[low..high], proceed with current high as pivot. */
    int pivotIndex = -1;
    for (int k = low; k <= high; k++) {
        if (arr[k] == pivot) { pivotIndex = k; break; }
    }
    if (pivotIndex != -1) {
        swap(&arr[pivotIndex], &arr[high]);
    }

    int i = low;
    for (int j = low; j < high; j++) {
        if (arr[j] < arr[high]) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[high]);
    return i;
}

int main() {
    srand((unsigned)time(NULL));
    int mode = 0;
    printf("Choose input mode:\n1) Manual input\n2) Random data\nSelect (1 or 2): ");
    if (scanf("%d", &mode) != 1) return 0;

    int n;
    int *arr;

    if (mode == 1) {
        printf("Enter number of elements: ");
        if (scanf("%d", &n) != 1) return 0;
        arr = malloc(n * sizeof(int));
        if (!arr) return 0;
        printf("Enter %d integers:\n", n);
        for (int i = 0; i < n; i++) scanf("%d", &arr[i]);
    } else {
        printf("Enter number of elements to generate: ");
        if (scanf("%d", &n) != 1) return 0;
        int range = 100;
        printf("Enter max random value (default 100): ");
        if (scanf("%d", &range) != 1) range = 100;
        arr = malloc(n * sizeof(int));
        if (!arr) return 0;
        for (int i = 0; i < n; i++) arr[i] = rand() % range + 1;
    }

    int *copy = malloc(n * sizeof(int));
    if (!copy) { free(arr); return 0; }

    printf("Original array: ");
    printArray(arr, n);

    printf("Maximum: %d\n", findMax(arr, n));

    int first, second;
    findFirstAndSecondLargest(arr, n, &first, &second);
    printf("First largest: %d\n", first);
    printf("Second largest: %d\n", second);

    printf("Mean: %.2f\n", findMean(arr, n));
    printf("Median: %.2f\n", findMedian(arr, n));
    printf("Standard deviation: %.2f\n", findStandardDeviation(arr, n));
    printf("Mode: %d\n", findMode(arr, n));

    int uniqueCount = removeDuplicates(arr, n, copy);
    printf("Array after removing duplicates: ");
    printArray(copy, uniqueCount);

    int *rev = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) rev[i] = arr[i];
    reverseArray(rev, n);
    printf("Reversed array: ");
    printArray(rev, n);

    int pivot;
    printf("Enter pivot value to partition around: ");
    if (scanf("%d", &pivot) != 1) pivot = arr[0];
    int *partitioned = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) partitioned[i] = arr[i];
    int pIndex = partitionAroundPivot(partitioned, 0, n - 1, pivot);
    printf("Partitioned array around pivot %d: ", pivot);
    printArray(partitioned, n);
    printf("Pivot position: %d\n", pIndex);

    free(arr); free(copy); free(rev); free(partitioned);
    return 0;
}