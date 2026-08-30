#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    int n;
    const char *inputFile = "input_data.txt";
    const char *outputFile = "sorted_data.txt";

    printf("===== Quick Sort of N elements (stored in a file) =====\n\n");
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

    /* Step 3: sort using Quick Sort */
    quickSort(arr, 0, n - 1);

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