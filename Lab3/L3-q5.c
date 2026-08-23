/*
 * Q5: Multiply special-pattern square matrices using D&C  -  O(n^2)
 * ---------------------------------------------------------------------
 * Each n x n matrix (n = 2^k) has the recursive block structure
 *
 *        M = [ M1  M2 ]
 *            [ M2  M1 ]
 *
 * all the way down to single elements. Equivalently, indexing rows and
 * columns from 0, M[i][j] depends ONLY on (i XOR j): if you write i and j
 * in binary, whichever bit position first differs between i and j decides
 * whether you are in the "M1-type" (matching bit -> stay on diagonal) or
 * "M2-type" (differing bit -> go off diagonal) block at that level, and
 * this composes exactly into M[i][j] = a[i XOR j] for some vector a of
 * length n.  (We use this fact only to *generate* valid test matrices;
 * the multiplication algorithm itself works purely on the block form.)
 *
 * DERIVING THE RECURSION FOR THE PRODUCT
 * ---------------------------------------
 * Let A = [[A1,A2],[A2,A1]], B = [[B1,B2],[B2,B1]] (each block n/2 x n/2,
 * and each block itself has the same recursive pattern). Block-multiply:
 *
 *   A*B = [ A1B1+A2B2   A1B2+A2B1 ]
 *         [ A2B1+A1B2   A2B2+A1B1 ]
 *       = [ C1  C2 ]      where  C1 = A1B1 + A2B2
 *         [ C2  C1 ]             C2 = A1B2 + A2B1
 *
 * So the PRODUCT also has the same special block-symmetric pattern - we
 * only ever need to compute the two distinct blocks C1 and C2.
 *
 * Computing C1, C2 naively needs 4 half-size multiplications (A1B1, A2B2,
 * A1B2, A2B1), giving T(n) = 4T(n/2) + O(n^2) = O(n^2 log n)  -  NOT O(n^2).
 *
 * KARATSUBA-STYLE TRICK (reduces 4 multiplications to 3):
 *   P1 = A1 * B1
 *   P2 = A2 * B2
 *   P3 = (A1+A2) * (B1+B2)        = A1B1 + A1B2 + A2B1 + A2B2
 *   =>  C1 = P1 + P2
 *       C2 = P3 - P1 - P2         (= A1B2 + A2B1)
 *
 * Note (A1+A2) and (B1+B2) are themselves special-pattern matrices (sum
 * of two special matrices is special), so P3 is a valid recursive call.
 *
 * Recurrence:  T(n) = 3*T(n/2) + O(n^2)
 * By the Master Theorem (case 3, since n^2 dominates n^(log2 3) ~ n^1.585):
 *   T(n) = Theta(n^2)
 *
 * (The additions/subtractions, which cost O(n^2) total across the whole
 * recursion because sum_i 3^i*(n/2^i)^2 is a decreasing geometric series
 * dominated by its first term, are what set the final Theta(n^2) - the
 * number of *scalar multiplications* alone only grows as n^log2 3.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef long long ll;

static long long mult_ops = 0;   /* scalar multiplications performed   */
static long long add_ops  = 0;   /* scalar additions/subtractions      */

static ll **allocMat(int n) {
    ll **m = malloc((size_t)n * sizeof(ll *));
    for (int i = 0; i < n; i++) m[i] = calloc((size_t)n, sizeof(ll));
    return m;
}
static void freeMat(ll **m, int n) { for (int i = 0; i < n; i++) free(m[i]); free(m); }

static void addMat(ll **A, ll **B, ll **C, int n) {
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) { C[i][j] = A[i][j] + B[i][j]; add_ops++; }
}
static void subMat(ll **A, ll **B, ll **C, int n) {
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) { C[i][j] = A[i][j] - B[i][j]; add_ops++; }
}
static void copyBlock(ll **src, int rowOff, int colOff, ll **dst, int n) {
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) dst[i][j] = src[rowOff + i][colOff + j];
}

/* A, B, C are full n x n matrices; A and B are assumed to already have
 * the special [[X1,X2],[X2,X1]] block pattern.  C is produced with that
 * same pattern automatically.                                          */
static void specialMultiply(ll **A, ll **B, ll **C, int n) {
    if (n == 1) { C[0][0] = A[0][0] * B[0][0]; mult_ops++; return; }

    int h = n / 2;
    ll **A1 = allocMat(h), **A2 = allocMat(h), **B1 = allocMat(h), **B2 = allocMat(h);
    copyBlock(A, 0, 0, A1, h);
    copyBlock(A, 0, h, A2, h);
    copyBlock(B, 0, 0, B1, h);
    copyBlock(B, 0, h, B2, h);

    ll **Asum = allocMat(h), **Bsum = allocMat(h);
    addMat(A1, A2, Asum, h);
    addMat(B1, B2, Bsum, h);

    ll **P1 = allocMat(h), **P2 = allocMat(h), **P3 = allocMat(h);
    specialMultiply(A1, B1, P1, h);
    specialMultiply(A2, B2, P2, h);
    specialMultiply(Asum, Bsum, P3, h);

    ll **C1 = allocMat(h), **C2 = allocMat(h), **tmp = allocMat(h);
    addMat(P1, P2, C1, h);              /* C1 = P1 + P2                */
    addMat(P1, P2, tmp, h);
    subMat(P3, tmp, C2, h);             /* C2 = P3 - P1 - P2           */

    for (int i = 0; i < h; i++) for (int j = 0; j < h; j++) {
        C[i][j] = C1[i][j];
        C[i][j + h] = C2[i][j];
        C[i + h][j] = C2[i][j];
        C[i + h][j + h] = C1[i][j];
    }

    freeMat(A1, h); freeMat(A2, h); freeMat(B1, h); freeMat(B2, h);
    freeMat(Asum, h); freeMat(Bsum, h);
    freeMat(P1, h); freeMat(P2, h); freeMat(P3, h);
    freeMat(C1, h); freeMat(C2, h); freeMat(tmp, h);
}

/* Build a valid special-pattern n x n matrix from n independent values:
 * M[i][j] = vals[i XOR j].  This is exactly the family described above. */
static void fillSpecial(ll **M, int n, ll *vals) {
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) M[i][j] = vals[i ^ j];
}

static void naiveMultiply(ll **A, ll **B, ll **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            ll s = 0;
            for (int k = 0; k < n; k++) s += A[i][k] * B[k][j];
            C[i][j] = s;
        }
}

int main(void) {
    srand(3);

    /* ---- correctness check against naive O(n^3) multiplication ---- */
    printf("Correctness check (special D&C vs naive O(n^3)):\n");
    int sizes[] = {1, 2, 4, 8, 16, 32, 64, 128};
    int ns = sizeof(sizes) / sizeof(sizes[0]);
    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        ll *avals = malloc((size_t)n * sizeof(ll));
        ll *bvals = malloc((size_t)n * sizeof(ll));
        for (int i = 0; i < n; i++) { avals[i] = rand() % 20 - 10; bvals[i] = rand() % 20 - 10; }

        ll **A = allocMat(n), **B = allocMat(n), **C1 = allocMat(n), **C2 = allocMat(n);
        fillSpecial(A, n, avals);
        fillSpecial(B, n, bvals);

        mult_ops = add_ops = 0;
        specialMultiply(A, B, C1, n);
        long long this_mult = mult_ops, this_add = add_ops;

        naiveMultiply(A, B, C2, n);

        int match = 1;
        for (int i = 0; i < n && match; i++)
            for (int j = 0; j < n && match; j++)
                if (C1[i][j] != C2[i][j]) match = 0;

        /* also confirm the OUTPUT itself has the special pattern */
        int patternOK = 1;
        for (int i = 0; i < n && patternOK; i++)
            for (int j = 0; j < n && patternOK; j++)
                if (C1[i][j] != C1[0][i ^ j]) patternOK = 0;

        printf("  n=%4d : match=%-3s  pattern_preserved=%-3s  mults=%-8lld  adds=%-8lld  total_ops=%-8lld  n^2=%-8d\n",
               n, match ? "OK" : "NO", patternOK ? "OK" : "NO",
               this_mult, this_add, this_mult + this_add, n * n);

        free(avals); free(bvals);
        freeMat(A, n); freeMat(B, n); freeMat(C1, n); freeMat(C2, n);
    }

    /* ---- empirical complexity validation: total_ops / n^2 should stay bounded ---- */
    printf("\nComplexity validation (total scalar ops vs n^2, and vs naive n^3):\n");
    printf("%8s %14s %14s %12s %14s\n", "n", "total_ops", "ops/n^2", "n^3(naive)", "speed-up x");
    int csizes[] = {8, 16, 32, 64, 128, 256, 512};
    for (int s = 0; s < 7; s++) {
        int n = csizes[s];
        ll *avals = malloc((size_t)n * sizeof(ll));
        ll *bvals = malloc((size_t)n * sizeof(ll));
        for (int i = 0; i < n; i++) { avals[i] = rand() % 20 - 10; bvals[i] = rand() % 20 - 10; }
        ll **A = allocMat(n), **B = allocMat(n), **C = allocMat(n);
        fillSpecial(A, n, avals);
        fillSpecial(B, n, bvals);

        mult_ops = add_ops = 0;
        specialMultiply(A, B, C, n);
        long long total = mult_ops + add_ops;
        double n3 = (double)n * n * n;

        printf("%8d %14lld %14.3f %12.0f %14.1f\n",
               n, total, (double)total / (n * n), n3, n3 / total);

        free(avals); free(bvals);
        freeMat(A, n); freeMat(B, n); freeMat(C, n);
    }
    printf("\n'ops/n^2' staying roughly bounded (not growing) as n grows confirms Theta(n^2).\n");

    return 0;
}