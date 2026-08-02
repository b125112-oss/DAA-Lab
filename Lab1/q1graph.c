#include <stdio.h>
#include <stdlib.h>

// Struct representing the properties of a mathematical function for complexity comparison
typedef struct {
    const char *formula_str;
    double base_of_exp;  
    double power_of_log_exp;  
    double poly_degree;     
    double log_degree;   
    double leading_coeff;  
} FuncComplexity;

// Comparison function used by qsort to order the growth rates
int compare_growth_rates(const void *ptr1, const void *ptr2) {
    FuncComplexity *func1 = (FuncComplexity *)ptr1;
    FuncComplexity *func2 = (FuncComplexity *)ptr2;
    
    if (func1->base_of_exp != func2->base_of_exp) 
        return (func1->base_of_exp < func2->base_of_exp) ? -1 : 1;
        
    if (func1->power_of_log_exp != func2->power_of_log_exp) 
        return (func1->power_of_log_exp < func2->power_of_log_exp) ? -1 : 1;
        
    if (func1->poly_degree != func2->poly_degree)       
        return (func1->poly_degree < func2->poly_degree) ? -1 : 1;
        
    if (func1->log_degree != func2->log_degree)   
        return (func1->log_degree < func2->log_degree) ? -1 : 1;
        
    if (func1->leading_coeff != func2->leading_coeff) 
        return (func1->leading_coeff < func2->leading_coeff) ? -1 : 1;
    
    return 0;
}

int main() {
    // Define the list of mathematical functions to be compared
    FuncComplexity math_funcs[] = {
        {"1 / n",          1, 0, -1,   0, 1},
        {"log2(n)",        1, 0, 0,    1, 1},
        {"12 * sqrt(n)",   1, 0, 0.5,  0, 12},
        {"50 * n^0.5",     1, 0, 0.5,  0, 50},
        {"n^0.51",         1, 0, 0.51, 0, 1},
        {"2^32 * n",       1, 0, 1,    0, 4294967296.0},
        {"n * log2(n)",    1, 0, 1,    1, 1},            
        {"n^2 - 324",      1, 0, 2,    0, 1},
        {"100n^2 + 6n",    1, 0, 2,    0, 100},
        {"2n^3",           1, 0, 3,    0, 2},
        {"n^(log2 n)",     1, 1, 0,    0, 1},
        {"3^n",            3, 0, 0,    0, 1}
    };

    int func_count = sizeof(math_funcs) / sizeof(math_funcs[0]);

    // Sort the array based on asymptotic growth logic
    qsort(math_funcs, func_count, sizeof(FuncComplexity), compare_growth_rates);

    // Print the sorted output to the console
    printf("Functions ordered by increasing asymptotic growth rate:\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < func_count; i++) {
        printf("Rank %2d: %-20s\n", i + 1, math_funcs[i].formula_str);
    }

    // Open pipe to Gnuplot for rendering the chart
    FILE *gnuplot_pipe = popen("gnuplot", "w");
    if (gnuplot_pipe == NULL) {
        perror("Error: Gnuplot could not be opened. Please ensure it is installed and in your PATH.");
        return 1;
    }

    // Configure the Gnuplot terminal, output file, and canvas size
    fprintf(gnuplot_pipe, "set terminal pngcairo size 1000,800 enhanced font 'Arial,10'\n");
    fprintf(gnuplot_pipe, "set output 'asymptotic_growth_plot.png'\n");
    
    // Set scale and bounds to prevent mathematics overflows
    fprintf(gnuplot_pipe, "set logscale xy\n");             
    fprintf(gnuplot_pipe, "set xrange [1:1e9]\n");   
    fprintf(gnuplot_pipe, "set yrange [1e-13:1e150]\n");

    // Set graph labels and title
    fprintf(gnuplot_pipe, "set title 'Comparison of Asymptotic Growth Rates'\n");
    fprintf(gnuplot_pipe, "set xlabel 'Input Size (n)'\n");
    fprintf(gnuplot_pipe, "set ylabel 'Operations f(n)'\n");
    fprintf(gnuplot_pipe, "set key spacing 1.5\n");      
    fprintf(gnuplot_pipe, "set key outside right center box\n");

    // Pass the plot rendering commands
    fprintf(gnuplot_pipe, "plot "
            "x*(log(x)/log(2)) title 'n log2(n)' lw 2, "
            "12*x**(0.5) title '12 * n^{0.5}' lw 2, "
            "1.0/x title '1/n' lw 2, "
            "(x <= 1e6 ? x**(log(x)/log(2)) : 1/0) title 'n^{log2 n}' lw 2, "
            "100*x**2 + 6*x title '100n^2 + 6n' lw 2, "
            "x**(0.51) title 'n^{0.51}' lw 2, "
            "(x > 18 ? x**2 - 324 : 1/0) title 'n^2 - 324' lw 2, "
            "50*x**(0.50) title '50 * n^{0.5}' lw 2, "
            "2*x**3 title '2n^3' lw 2, "
            "(x <= 415 ? 3**x : 1/0) title '3^n' lw 2, "
            "x*4294967296.0 title '2^{32} * n' lw 2, "
            "(log(x)/log(2)) title 'log2(n)' lw 2 \n");

    fprintf(gnuplot_pipe, "exit\n");
    pclose(gnuplot_pipe);
    
    return 0;
}