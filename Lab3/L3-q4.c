/*
 * Q4: Matrix Multiplication using Strassen's Method (D&C)
 * ----------------------------------------------------------
 * Naive multiplication of two n x n matrices: Theta(n^3).
 *
 * Strassen's trick: split each n x n matrix into four (n/2) x (n/2)
 * blocks, and combine them using only 7 recursive multiplications
 * (instead of the natural 8) plus O(n^2) additions/subtractions:
 *
 *   M1 = (A11+A22)(B11+B22)      M5 = (A11+A12) B22
 *   M2 = (A21+A22) B11           M6 = (A21-A11)(B11+B12)
 *   M3 = A11 (B12-B22)           M7 = (A12-A22)(B21+B22)
 *   M4 = A22 (B21-B11)
 *
 *   C11 = M1+M4-M5+M7    C12 = M3+M5
 *   C21 = M2+M4          C22 = M1-M2+M3+M6
 *
 * Recurrence: T(n) = 7*T(n/2) + O(n^2)  =>  T(n) = Theta(n^log2 7) ~= Theta(n^2.807)
 * which beats the naive Theta(n^3) for large n.
 *
 * This program works for any n (not just powers of two) by padding the
 * matrices with zero rows/columns up to the next power of two, running
 * Strassen's algorithm, then trimming the result back down.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef long long ll;

static ll **allocMat(int n) {
    ll **m = malloc((size_t)n * sizeof(ll *));
    for (int i = 0; i < n; i++) m[i] = calloc((size_t)n, sizeof(ll));
    return m;
}
static void freeMat(ll **m, int n) {
    for (int i = 0; i < n; i++) free(m[i]);
    free(m);
}
static void addMat(ll **A, ll **B, ll **C, int n) {
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) C[i][j] = A[i][j] + B[i][j];
}
static void subMat(ll **A, ll **B, ll **C, int n) {
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) C[i][j] = A[i][j] - B[i][j];
}

static void strassen(ll **A, ll **B, ll **C, int n) {
    if (n == 1) { C[0][0] = A[0][0] * B[0][0]; return; }
    if (n <= 32) {
        /* small-n cutoff: naive multiply is faster in practice and avoids
         * needless recursion overhead; does not change asymptotic result */
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                ll s = 0;
                for (int k = 0; k < n; k++) s += A[i][k] * B[k][j];
                C[i][j] = s;
            }
        return;
    }

    int h = n / 2;
    ll **A11 = allocMat(h), **A12 = allocMat(h), **A21 = allocMat(h), **A22 = allocMat(h);
    ll **B11 = allocMat(h), **B12 = allocMat(h), **B21 = allocMat(h), **B22 = allocMat(h);
    for (int i = 0; i < h; i++) for (int j = 0; j < h; j++) {
        A11[i][j] = A[i][j];     A12[i][j] = A[i][j + h];
        A21[i][j] = A[i + h][j]; A22[i][j] = A[i + h][j + h];
        B11[i][j] = B[i][j];     B12[i][j] = B[i][j + h];
        B21[i][j] = B[i + h][j]; B22[i][j] = B[i + h][j + h];
    }

    ll **M1 = allocMat(h), **M2 = allocMat(h), **M3 = allocMat(h), **M4 = allocMat(h);
    ll **M5 = allocMat(h), **M6 = allocMat(h), **M7 = allocMat(h);
    ll **T1 = allocMat(h), **T2 = allocMat(h);

    addMat(A11, A22, T1, h); addMat(B11, B22, T2, h); strassen(T1, T2, M1, h);
    addMat(A21, A22, T1, h);                          strassen(T1, B11, M2, h);
    subMat(B12, B22, T2, h);                          strassen(A11, T2, M3, h);
    subMat(B21, B11, T2, h);                          strassen(A22, T2, M4, h);
    addMat(A11, A12, T1, h);                          strassen(T1, B22, M5, h);
    subMat(A21, A11, T1, h); addMat(B11, B12, T2, h); strassen(T1, T2, M6, h);
    subMat(A12, A22, T1, h); addMat(B21, B22, T2, h); strassen(T1, T2, M7, h);

    ll **C11 = allocMat(h), **C12 = allocMat(h), **C21 = allocMat(h), **C22 = allocMat(h);
    addMat(M1, M4, T1, h); subMat(T1, M5, T2, h); addMat(T2, M7, C11, h);
    addMat(M3, M5, C12, h);
    addMat(M2, M4, C21, h);
    subMat(M1, M2, T1, h); addMat(T1, M3, T2, h); addMat(T2, M6, C22, h);

    for (int i = 0; i < h; i++) for (int j = 0; j < h; j++) {
        C[i][j] = C11[i][j];         C[i][j + h] = C12[i][j];
        C[i + h][j] = C21[i][j];     C[i + h][j + h] = C22[i][j];
    }

    freeMat(A11, h); freeMat(A12, h); freeMat(A21, h); freeMat(A22, h);
    freeMat(B11, h); freeMat(B12, h); freeMat(B21, h); freeMat(B22, h);
    freeMat(M1, h); freeMat(M2, h); freeMat(M3, h); freeMat(M4, h);
    freeMat(M5, h); freeMat(M6, h); freeMat(M7, h);
    freeMat(T1, h); freeMat(T2, h);
    freeMat(C11, h); freeMat(C12, h); freeMat(C21, h); freeMat(C22, h);
}

static int nextPow2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

/* Public entry point: multiplies two n x n matrices for ANY n by padding
 * to the next power of two internally.                                  */
static void strassenMultiply(ll **A, ll **B, ll **C, int n) {
    int m = nextPow2(n);
    if (m == n) { strassen(A, B, C, n); return; }

    ll **Ap = allocMat(m), **Bp = allocMat(m), **Cp = allocMat(m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) { Ap[i][j] = A[i][j]; Bp[i][j] = B[i][j]; }

    strassen(Ap, Bp, Cp, m);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) C[i][j] = Cp[i][j];

    freeMat(Ap, m); freeMat(Bp, m); freeMat(Cp, m);
}

static void naiveMultiply(ll **A, ll **B, ll **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            ll s = 0;
            for (int k = 0; k < n; k++) s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
}

static void printMat(ll **M, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) printf("%6lld ", M[i][j]);
        printf("\n");
    }
}

int main(void) {
    srand(7);

    /* ---- 1. Small worked example, printed in full ---- */
    int n = 4;
    ll **A = allocMat(n), **B = allocMat(n), **C = allocMat(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) { A[i][j] = rand() % 10; B[i][j] = rand() % 10; }

    printf("Matrix A:\n"); printMat(A, n);
    printf("\nMatrix B:\n"); printMat(B, n);

    strassenMultiply(A, B, C, n);
    printf("\nC = A x B  (Strassen):\n"); printMat(C, n);

    ll **Cn = allocMat(n);
    naiveMultiply(A, B, Cn, n);
    int ok = 1;
    for (int i = 0; i < n && ok; i++)
        for (int j = 0; j < n && ok; j++)
            if (C[i][j] != Cn[i][j]) ok = 0;
    printf("\nMatches naive O(n^3) multiplication: %s\n\n", ok ? "YES" : "NO");
    freeMat(A, n); freeMat(B, n); freeMat(C, n); freeMat(Cn, n);

    /* ---- 2. Correctness stress test across various sizes (incl. non-power-of-2) ---- */
    int sizes[] = {1, 2, 3, 5, 7, 8, 15, 16, 33, 50, 64};
    int ns = sizeof(sizes) / sizeof(sizes[0]);
    printf("Correctness check across sizes:\n");
    for (int s = 0; s < ns; s++) {
        int m = sizes[s];
        ll **X = allocMat(m), **Y = allocMat(m), **Z1 = allocMat(m), **Z2 = allocMat(m);
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) { X[i][j] = rand() % 20 - 10; Y[i][j] = rand() % 20 - 10; }
        strassenMultiply(X, Y, Z1, m);
        naiveMultiply(X, Y, Z2, m);
        int match = 1;
        for (int i = 0; i < m && match; i++)
            for (int j = 0; j < m && match; j++)
                if (Z1[i][j] != Z2[i][j]) match = 0;
        printf("  n=%3d : %s\n", m, match ? "OK" : "MISMATCH");
        freeMat(X, m); freeMat(Y, m); freeMat(Z1, m); freeMat(Z2, m);
    }

    /* ---- 3. Timing comparison to illustrate the asymptotic advantage ---- */
    printf("\nTiming Strassen vs naive (larger matrices):\n");
    int tsizes[] = {128, 256, 512};
    for (int s = 0; s < 3; s++) {
        int m = tsizes[s];
        ll **X = allocMat(m), **Y = allocMat(m), **Z1 = allocMat(m), **Z2 = allocMat(m);
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) { X[i][j] = rand() % 10; Y[i][j] = rand() % 10; }

        clock_t t0 = clock();
        strassenMultiply(X, Y, Z1, m);
        clock_t t1 = clock();
        naiveMultiply(X, Y, Z2, m);
        clock_t t2 = clock();

        double strassen_s = (double)(t1 - t0) / CLOCKS_PER_SEC;
        double naive_s    = (double)(t2 - t1) / CLOCKS_PER_SEC;

        int match = 1;
        for (int i = 0; i < m && match; i++)
            for (int j = 0; j < m && match; j++)
                if (Z1[i][j] != Z2[i][j]) match = 0;

        printf("  n=%4d : Strassen=%.4fs  Naive=%.4fs  match=%s\n",
               m, strassen_s, naive_s, match ? "OK" : "MISMATCH");

        freeMat(X, m); freeMat(Y, m); freeMat(Z1, m); freeMat(Z2, m);
    }

    return 0;
}