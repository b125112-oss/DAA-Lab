#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Sift the element at index i down so the subtree rooted at i
   satisfies the max-heap property. 'n' is the current heap size. */
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    /* Build max heap: start from the last internal node and go up */
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    /* Repeatedly move the current max (root) to the end */
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

int main() {
    int n;
    const char *inputFile = "heap_input.txt";
    const char *outputFile = "heap_sorted.txt";

    printf("===== Heap Sort of N elements (stored in a file) =====\n\n");
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

    /* Step 1: store the entered elements in a file */
    FILE *fp = fopen(inputFile, "w");
    if (fp == NULL) {
        printf("Error: could not open '%s' for writing.\n", inputFile);
        free(arr);
        return 1;
    }
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", arr[i]);
    fclose(fp);
    printf("\n%d element(s) written to '%s'.\n", n, inputFile);

    /* Step 2: read the elements back from the file */
    fp = fopen(inputFile, "r");
    if (fp == NULL) {
        printf("Error: could not open '%s' for reading.\n", inputFile);
        free(arr);
        return 1;
    }
    int count = 0;
    while (count < n && fscanf(fp, "%d", &arr[count]) == 1)
        count++;
    fclose(fp);

    if (count != n) {
        printf("Error: file data mismatch (expected %d, read %d).\n", n, count);
        free(arr);
        return 1;
    }

    /* Step 3: sort using Heap Sort */
    heapSort(arr, n);

    /* Step 4: write the sorted data to a new file */
    fp = fopen(outputFile, "w");
    if (fp == NULL) {
        printf("Error: could not open '%s' for writing.\n", outputFile);
        free(arr);
        return 1;
    }
    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", arr[i]);
    fclose(fp);

    printf("Sorted elements (also written to '%s'):\n", outputFile);
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    free(arr);
    return 0;
}