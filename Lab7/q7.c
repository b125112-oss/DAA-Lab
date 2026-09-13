/* =====================================================================
   DAA Lab-07, Q7: Matrix Chain Multiplication (MCM)
   =====================================================================

   PROBLEM
   -------
   Given a chain of n matrices A1, A2, ..., An where Ai has dimensions
   p[i-1] x p[i], find the parenthesisation (order of multiplying
   adjacent pairs) that minimises the total number of scalar
   multiplications, and report that minimum together with the optimal
   parenthesisation.

   INPUT REPRESENTATION
   ---------------------
   An array p[0..n] of n+1 positive integers, where matrix Ai (1<=i<=n)
   has dimensions p[i-1] x p[i].

   ALGORITHM (DYNAMIC PROGRAMMING)
   ----------------------------------
   Let m[i][j] = minimum scalar multiplications needed to compute the
   product Ai * A(i+1) * ... * Aj (1<=i<=j<=n).
       m[i][i] = 0
       m[i][j] = min over i<=k<j of  m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]
   The split k that achieves the minimum is stored in s[i][j] so the
   optimal parenthesisation can be reconstructed afterwards.
   Sub-chains are processed in increasing order of chain length L =
   j-i+1 (the classic "diagonal-by-diagonal" fill order), since m[i][j]
   depends only on strictly shorter sub-chains.

   COMPLEXITY
   -----------
     Time  : O(n^3)   (O(n^2) sub-chains (i,j), each scanned over
                        O(n) split points k)
     Space : O(n^2)    (tables m and s)
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>

long long **m;
int **s;
int n;

void matrix_chain_order(int *p) {
    m = malloc((n + 1) * sizeof(long long *));
    s = malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++) {
        m[i] = calloc(n + 1, sizeof(long long));
        s[i] = calloc(n + 1, sizeof(int));
    }

    for (int i = 1; i <= n; i++) m[i][i] = 0;

    for (int L = 2; L <= n; L++) {              /* L = chain length */
        for (int i = 1; i <= n - L + 1; i++) {
            int j = i + L - 1;
            m[i][j] = -1;
            for (int k = i; k < j; k++) {
                long long cost = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
                if (m[i][j] == -1 || cost < m[i][j]) { m[i][j] = cost; s[i][j] = k; }
            }
        }
    }
}

void print_optimal_parens(int i, int j) {
    if (i == j) { printf("A%d", i); return; }
    printf("(");
    print_optimal_parens(i, s[i][j]);
    print_optimal_parens(s[i][j] + 1, j);
    printf(")");
}

/* -------- brute force (try every full parenthesisation) for
   independent validation on small n (exponential, Catalan-number many
   parenthesisations, so only used for n up to ~10) -------------------- */
long long brute_force(int i, int j, int *p) {
    if (i == j) return 0;
    long long best = -1;
    for (int k = i; k < j; k++) {
        long long cost = brute_force(i, k, p) + brute_force(k + 1, j, p)
                        + (long long)p[i - 1] * p[k] * p[j];
        if (best == -1 || cost < best) best = cost;
    }
    return best;
}

int main(void) {
    printf("=== Q7: Matrix Chain Multiplication ===\n\n");

    printf("Validation: DP vs. exhaustive brute force on random small instances\n");
    srand(42);
    int allOk = 1;
    for (int trial = 0; trial < 8; trial++) {
        int nn = 2 + rand() % 7;                 /* n = 2..8 matrices */
        int *pp = malloc((nn + 1) * sizeof(int));
        for (int i = 0; i <= nn; i++) pp[i] = 2 + rand() % 20;
        n = nn;
        matrix_chain_order(pp);
        long long dpAns = m[1][nn];
        long long bfAns = brute_force(1, nn, pp);
        int ok = (dpAns == bfAns);
        allOk &= ok;
        printf("  trial %d: n=%d  DP=%lld  brute=%lld  %s\n",
               trial + 1, nn, dpAns, bfAns, ok ? "OK" : "MISMATCH");
        for (int i = 0; i <= nn; i++) { free(m[i]); free(s[i]); }
        free(m); free(s); free(pp);
    }
    printf("All validation trials passed: %s\n\n", allOk ? "YES" : "NO");

    printf("Enter number of matrices n: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input.\n"); return 1; }
    int *p = malloc((n + 1) * sizeof(int));
    printf("Enter %d dimensions p[0..%d] (matrix Ai is p[i-1] x p[i]):\n", n + 1, n);
    for (int i = 0; i <= n; i++)
        if (scanf("%d", &p[i]) != 1) { printf("Invalid input.\n"); return 1; }

    matrix_chain_order(p);

    printf("\nMinimum number of scalar multiplications = %lld\n", m[1][n]);
    printf("Optimal parenthesisation: ");
    print_optimal_parens(1, n);
    printf("\n");

    for (int i = 0; i <= n; i++) { free(m[i]); free(s[i]); }
    free(m); free(s); free(p);
    return 0;
}