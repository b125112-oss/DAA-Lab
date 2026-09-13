/* =====================================================================
   DAA Lab-07, Q3: Reve's puzzle  (Tower of Hanoi with FOUR pegs)
   =====================================================================

   PROBLEM
   -------
   n disks (n = 8 in the given instance), 4 pegs A,B,C,D. All disks
   start on peg A in decreasing-size order; move them all to another
   peg, one at a time, never placing a larger disk on a smaller one.
   Minimise the number of moves. Show this can be done in 33 moves for
   n = 8, and generalise to n disks / 4 pegs.

   INPUT REPRESENTATION
   ---------------------
   n (number of disks) and the 4 peg labels; the recursive algorithm
   itself only needs n and (source, auxiliary1, auxiliary2, target).

   ALGORITHM: FRAME-STEWART
   --------------------------
   With only 3 pegs the classical Tower of Hanoi needs 2^n - 1 moves
   (move n-1 disks out of the way using the other 2 pegs, move the
   largest disk, move the n-1 disks back).  With a 4th peg available
   we can be smarter: move the TOP k disks (some 1<=k<n, to be chosen
   optimally) to a spare peg using all 4 pegs recursively, then move
   the remaining (n-k) disks using ONLY 3 pegs (classical Hanoi, since
   the k smaller disks are safely out of the way and one peg is
   effectively "borrowed" by them), then move the k disks back on top
   using all 4 pegs again.  This gives the recurrence (Frame, 1941 /
   Stewart, 1941 -- proved optimal by Bousch, 2014):

       FS(0)   = 0
       FS(n)   = min over 1<=k<n of  [ 2*FS(k) + (2^(n-k) - 1) ]

   The optimal k is not a simple closed form, so it is computed by an
   O(n^2) DP that tries every k for every sub-size 1..n (this also
   RECOVERS the optimal k for every size, needed to actually generate
   the move sequence, not just the count).

   For n = 8: the DP below gives FS(8) = 33, matching the puzzle
   statement, achieved for k = 4 (move top 4 disks to a spare peg
   using all 4 pegs, do a plain 3-peg Hanoi transfer of the bottom 4
   disks, then move the top 4 back).

   The program also (a) prints the FS(n) table for n = 1..12 --
   reproducing the well-known sequence 1,3,5,9,13,17,25,33,41,49,65,81
   -- and (b) GENERATES the actual move list recursively and
   VALIDATES it by simulation (checks every move is legal -- i.e.
   never places a larger disk on a smaller one -- and that the move
   count matches FS(n) exactly and all disks end on the target peg).

   COMPLEXITY
   -----------
     Computing FS(1..n) and best-k table  : O(n^2) time, O(n) space
     Generating the move list             : O(FS(n)) = O(2^(n/2))-ish
                                              time (the move list itself
                                              has FS(n) moves, which is
                                              the true lower bound -- you
                                              cannot do better than
                                              writing down every move)
     Simulation / validation of the moves : O(FS(n)) time, O(n) space
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>

long long *FS;      /* FS[i] = min moves for i disks, 4 pegs */
int *bestK;          /* bestK[i] = optimal split point for i disks   */
int totalDisks;

typedef struct { int from, to; } Move;
Move *moves;
long long moveCount;

/* ---- classical 3-peg Hanoi (used for the "middle" phase) ------------- */
void hanoi3(int n, int src, int aux, int dst) {
    if (n == 0) return;
    hanoi3(n - 1, src, dst, aux);
    moves[moveCount].from = src; moves[moveCount].to = dst; moveCount++;
    hanoi3(n - 1, aux, src, dst);
}

/* ---- Frame-Stewart 4-peg algorithm ------------------------------------
   pegs[4] = {src, spareA, spareB, dst} logically, but we pass explicit
   peg numbers so any labelling of the 4 physical pegs works. */
void frameStewart(int n, int src, int p2, int p3, int dst) {
    if (n == 0) return;
    if (n == 1) {
        moves[moveCount].from = src; moves[moveCount].to = dst; moveCount++;
        return;
    }
    int k = bestK[n];
    /* Step 1: move top k disks from src to p2 (spare), using all 4 pegs
       (src, p3, dst, p2 act as the 4 pegs for this sub-problem)         */
    frameStewart(k, src, p3, dst, p2);
    /* Step 2: move remaining n-k disks from src to dst using only 3 pegs
       (src, p3, dst) -- p2 is "in use" holding the k smaller disks      */
    hanoi3(n - k, src, p3, dst);
    /* Step 3: move the k disks from p2 back on top, using all 4 pegs    */
    frameStewart(k, p2, src, p3, dst);
}

/* ---- O(n^2) DP to compute FS(n) and the optimal k for every size ----- */
void compute_FS(int n) {
    FS = malloc((n + 1) * sizeof(long long));
    bestK = malloc((n + 1) * sizeof(int));
    FS[0] = 0; bestK[0] = 0;
    if (n >= 1) { FS[1] = 1; bestK[1] = 0; }
    for (int i = 2; i <= n; i++) {
        long long best = -1; int bk = 1;
        for (int k = 1; k < i; k++) {
            long long twoP = 1;
            for (int j = 0; j < i - k; j++) twoP *= 2;      /* 2^(i-k) */
            long long val = 2 * FS[k] + (twoP - 1);
            if (best == -1 || val < best) { best = val; bk = k; }
        }
        FS[i] = best; bestK[i] = bk;
    }
}

/* ---- validate a generated move list against the disk-size rules ------ */
int validate(int n, int nPegs, int startPeg, int endPeg) {
    /* pegStack[p] holds disk sizes, larger number = bigger disk, stored
       with the smallest on "top" (end of array) */
    int **pegStack = malloc(nPegs * sizeof(int *));
    int *height = calloc(nPegs, sizeof(int));
    for (int p = 0; p < nPegs; p++) pegStack[p] = malloc((n + 1) * sizeof(int));
    for (int d = n; d >= 1; d--) pegStack[startPeg][height[startPeg]++] = d;

    int ok = 1;
    for (long long i = 0; i < moveCount && ok; i++) {
        int f = moves[i].from, t = moves[i].to;
        if (height[f] == 0) { ok = 0; break; }
        int disk = pegStack[f][height[f] - 1];
        if (height[t] > 0 && pegStack[t][height[t] - 1] < disk) { ok = 0; break; }
        height[f]--;
        pegStack[t][height[t]++] = disk;
    }
    if (ok) {
        if (height[endPeg] != n) ok = 0;
        for (int p = 0; p < nPegs && ok; p++)
            if (p != endPeg && height[p] != 0) ok = 0;
    }
    for (int p = 0; p < nPegs; p++) free(pegStack[p]);
    free(pegStack); free(height);
    return ok;
}

int main(void) {
    printf("=== Q3: Reve's puzzle (4-peg Tower of Hanoi, Frame-Stewart algorithm) ===\n\n");

    int N = 20;
    compute_FS(N);
    printf("FS(n) for n = 1..12 (4 pegs), with optimal split k:\n");
    printf("%4s %8s %6s\n", "n", "FS(n)", "k");
    for (int i = 1; i <= 12; i++) printf("%4d %8lld %6d\n", i, FS[i], bestK[i]);

    printf("\nExpected classical sequence: 1 3 5 9 13 17 25 33 41 49 65 81\n");
    printf("FS(8) = %lld (expected 33)\n", FS[8]);

    int n;
    printf("\nEnter number of disks n to generate & validate an explicit move sequence: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > N) { printf("Invalid input (1..%d).\n", N); return 1; }

    moves = malloc((size_t)(FS[n] + 10) * sizeof(Move));
    moveCount = 0;
    totalDisks = n;
    /* pegs numbered 0,1,2,3 ; move all disks from peg0 to peg3 */
    frameStewart(n, 0, 1, 2, 3);

    printf("\nGenerated %lld moves for n = %d disks (formula says %lld).\n", moveCount, n, FS[n]);
    int valid = validate(n, 4, 0, 3);
    printf("Move sequence obeys the disk-size rule and ends with all disks on the target peg: %s\n",
           valid ? "YES (valid)" : "NO (INVALID!)");

    if (n <= 4) {
        printf("\nFull move list (peg 0=A,1=B,2=C,3=D):\n");
        for (long long i = 0; i < moveCount; i++)
            printf("  %lld: peg %c -> peg %c\n", i + 1, 'A' + moves[i].from, 'A' + moves[i].to);
    }

    free(moves); free(FS); free(bestK);
    return 0;
}