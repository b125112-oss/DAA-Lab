#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Standard Bubble Sort: Always completes the (n-1)th pass
void bubbleSortStandard(int arr[], int n, long long *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Optimized Bubble Sort: Terminates if array is sorted before (n-1)th pass
void bubbleSortOptimized(int arr[], int n, long long *comparisons) {
    *comparisons = 0;
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            (*comparisons)++;
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

int main() {
    int min_size = 100;
    int max_size = 2000;
    int step = 100;
    int trials = 5; 

    int num_points = (max_size - min_size) / step + 1;
    
    int *sizes = (int *)malloc(num_points * sizeof(int));
    long long *std_data = (long long *)malloc(num_points * sizeof(long long));
    long long *opt_data = (long long *)malloc(num_points * sizeof(long long));

    srand((unsigned int)time(NULL));
    printf("Running simulations and buffering data...\n");

    // 1. Run simulations and store results in arrays
    int data_index = 0;
    for (int n = min_size; n <= max_size; n += step) {
        long long totalCompStd = 0;
        long long totalCompOpt = 0;

        for (int t = 0; t < trials; t++) {
            int *arrStd = (int *)malloc(n * sizeof(int));
            int *arrOpt = (int *)malloc(n * sizeof(int));

            // Populate the first half with randomized data
            int split_index = n / 2;
            for (int i = 0; i < split_index; i++) {
                int val = rand() % 10000;
                arrStd[i] = val;
                arrOpt[i] = val;
            }
            
            // Populate the second half with strictly increasing, sorted data
            int current_sorted_val = 10000;
            for (int i = split_index; i < n; i++) {
                current_sorted_val += (rand() % 10) + 1; 
                arrStd[i] = current_sorted_val;
                arrOpt[i] = current_sorted_val;
            }

            long long compStd = 0, compOpt = 0;
            bubbleSortStandard(arrStd, n, &compStd);
            bubbleSortOptimized(arrOpt, n, &compOpt);

            totalCompStd += compStd;
            totalCompOpt += compOpt;

            free(arrStd);
            free(arrOpt);
        }

        sizes[data_index] = n;
        std_data[data_index] = totalCompStd / trials;
        opt_data[data_index] = totalCompOpt / trials;
        data_index++;
    }
    
    printf("Simulation complete. Piping data directly to gnuplot...\n");

    FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe) {
        fprintf(gnuplotPipe, "set terminal pngcairo size 800,600 enhanced font 'Arial,12'\n");
        fprintf(gnuplotPipe, "set output 'bubble_sort_performance.png'\n");
        fprintf(gnuplotPipe, "set title 'Performance Analysis: Bubble Sort Comparisons'\n");
        fprintf(gnuplotPipe, "set xlabel 'Array Size (n)'\n");
        fprintf(gnuplotPipe, "set ylabel 'Number of Comparisons'\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "set key left top\n");
        
        fprintf(gnuplotPipe, "plot '-' using 1:2 with linespoints lw 2 pt 7 title 'Standard (Unoptimized)', \\\n");
        fprintf(gnuplotPipe, "     '-' using 1:2 with linespoints lw 2 pt 7 title 'Optimized (Early Termination)'\n");
        
        // Send Standard Bubble Sort data
        for (int i = 0; i < num_points; i++) {
            fprintf(gnuplotPipe, "%d %lld\n", sizes[i], std_data[i]);
        }
        fprintf(gnuplotPipe, "e\n");

        // Send Optimized Bubble Sort data
        for (int i = 0; i < num_points; i++) {
            fprintf(gnuplotPipe, "%d %lld\n", sizes[i], opt_data[i]);
        }
        fprintf(gnuplotPipe, "e\n");
        
        pclose(gnuplotPipe);
        printf("Success! Graph saved as 'bubble_sort_performance.png'.\n");
    } else {
        printf("Error: Could not open gnuplot. Is it installed?\n");
    }

    free(sizes);
    free(std_data);
    free(opt_data);

    return 0;
}