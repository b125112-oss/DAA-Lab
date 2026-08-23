/* ============================================================================
 * complexity_gen.c
 *
 * Purpose:
 *   Generate the THEORETICAL worst-case growth curves for the seven
 *   Dictionary ADT operations (Search, Insert, Delete, Max, Min,
 *   Predecessor, Successor) on six implementations:
 *
 *        UA  - Unsorted array
 *        SA  - Sorted array
 *        SLU - Singly linked, unsorted
 *        SLS - Singly linked, sorted
 *        DLU - Doubly linked, unsorted
 *        DLS - Doubly linked, sorted
 *
 *   This program does NOT build any data structure and does NOT measure
 *   actual runtime. It simply evaluates the closed-form worst-case cost
 *   functions f(n) that were derived analytically (O(1), O(log2 n), O(n))
 *   for n = 1..N, and writes them to .dat files that gnuplot can plot
 *   directly. This is exactly what the assignment asks for: "plot the
 *   order of growth of these functions" rather than empirical timing.
 *
 * Output files (space-separated, gnuplot-friendly):
 *   growth_functions.dat   -> the three base functions: 1, log2(n), n
 *   op_search.dat
 *   op_insert.dat
 *   op_delete.dat
 *   op_max.dat
 *   op_min.dat
 *   op_predecessor.dat
 *   op_successor.dat
 *      each has columns: n  UA  SA  SLU  SLS  DLU  DLS
 *
 * Compile:  gcc -O2 -o complexity_gen complexity_gen.c -lm
 * Run:      ./complexity_gen 100        (N = upper bound on n, default 100)
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---- the only three asymptotic classes that occur in the whole table ---- */
static double f_const(int n)  { (void)n; return 1.0; }
static double f_log(int n)    { return log2((double)n); }
static double f_linear(int n) { return (double)n; }

/* Complexity code: 0 = O(1), 1 = O(log n), 2 = O(n) */
static double eval_cost(int code, int n) {
    switch (code) {
        case 0: return f_const(n);
        case 1: return f_log(n);
        case 2: return f_linear(n);
    }
    return 0.0;
}

/* ----------------------------------------------------------------------
 * The worst-case complexity table derived analytically.
 * Rows = operations, columns = {UA, SA, SLU, SLS, DLU, DLS}
 * Assumptions used (stated explicitly, see accompanying report):
 *   - Only a head pointer is kept for singly linked lists (no tail ptr).
 *   - A tail pointer IS kept for the doubly linked sorted list, so its
 *     Max is O(1) (mirrors Min via the head pointer).
 *   - Insert/Delete/Search are given a key or pointer as stated in the
 *     problem; array Delete assumes an index/pointer is given (swap with
 *     last element -> O(1) for the unsorted array).
 *   - Singly linked Delete needs the predecessor (no back-link), so it
 *     costs O(n) even when a pointer to the node itself is given.
 * ---------------------------------------------------------------------- */
typedef struct {
    const char *op_name;
    const char *filename;
    int cost[6];   /* UA, SA, SLU, SLS, DLU, DLS */
} OpRow;

static OpRow table[] = {
    { "Search",      "op_search.dat",      {2, 1, 2, 2, 2, 2} },
    { "Insert",      "op_insert.dat",      {0, 2, 0, 2, 0, 2} },
    { "Delete",      "op_delete.dat",      {0, 2, 2, 2, 0, 0} },
    { "Max",         "op_max.dat",         {2, 0, 2, 2, 2, 0} },
    { "Min",         "op_min.dat",         {2, 0, 2, 0, 2, 0} },
    { "Predecessor", "op_predecessor.dat", {2, 0, 2, 2, 2, 0} },
    { "Successor",   "op_successor.dat",   {2, 0, 2, 0, 2, 0} },
};
static const int NUM_OPS = sizeof(table) / sizeof(table[0]);
static const char *STRUCT_NAMES[6] =
    { "UA", "SA", "SLU", "SLS", "DLU", "DLS" };

static void write_growth_functions(int N) {
    FILE *fp = fopen("growth_functions.dat", "w");
    if (!fp) { perror("growth_functions.dat"); exit(1); }
    fprintf(fp, "# n  O(1)  O(log2n)  O(n)\n");
    for (int n = 1; n <= N; n++) {
        fprintf(fp, "%d %.6f %.6f %.6f\n",
                n, f_const(n), f_log(n), f_linear(n));
    }
    fclose(fp);
}

static void write_operation_file(const OpRow *row, int N) {
    FILE *fp = fopen(row->filename, "w");
    if (!fp) { perror(row->filename); exit(1); }
    fprintf(fp, "# n  UA  SA  SLU  SLS  DLU  DLS   (operation: %s)\n",
            row->op_name);
    for (int n = 1; n <= N; n++) {
        fprintf(fp, "%d", n);
        for (int s = 0; s < 6; s++) {
            fprintf(fp, " %.6f", eval_cost(row->cost[s], n));
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

static void print_summary_table(void) {
    printf("\nWorst-case cost table used to generate the data "
           "(theoretical, not measured):\n\n");
    printf("%-12s", "Operation");
    for (int s = 0; s < 6; s++) printf("%-8s", STRUCT_NAMES[s]);
    printf("\n");
    for (int i = 0; i < NUM_OPS; i++) {
        printf("%-12s", table[i].op_name);
        for (int s = 0; s < 6; s++) {
            const char *lbl = table[i].cost[s] == 0 ? "O(1)"
                             : table[i].cost[s] == 1 ? "O(logn)"
                                                      : "O(n)";
            printf("%-8s", lbl);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int N = 100;
    if (argc > 1) N = atoi(argv[1]);
    if (N < 2) N = 2;

    write_growth_functions(N);
    for (int i = 0; i < NUM_OPS; i++) {
        write_operation_file(&table[i], N);
    }

    print_summary_table();
    printf("Generated growth_functions.dat and %d op_*.dat files for n = 1..%d\n",
           NUM_OPS, N);
    return 0;
}
