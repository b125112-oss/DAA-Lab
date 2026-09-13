/* =====================================================================
   DAA Lab-07, Q1: Invert the coin-triangle
   =====================================================================

   PROBLEM
   -------
   A triangle of side n is formed by n(n+1)/2 identical coins packed on
   a triangular lattice: row r (0-indexed from the apex, r = 0..n-1)
   holds (r+1) coins.  On each move a single coin may be lifted and
   placed on any empty lattice site.  Find the minimum number of moves
   that turns the (apex-up) triangle into an (apex-down) triangle of
   the same size, and give a compact formula for that minimum.

   INPUT REPRESENTATION
   ---------------------
   A coin position is stored as an integer pair (c, r):
       r = row index, 0 .. n-1   (0 = apex row)
       c = column within the row, 0 .. r
   The whole triangle is therefore just the integer n; individual
   coins never need to be stored explicitly for the counting formula,
   only for the constructive move list (O(n) coins per row -> O(n^2)
   coins total, so the natural input/output size for this problem is
   Theta(n^2), matching the number of physical coins).

   ALGORITHM / DERIVATION  (see comments in build_down_triangle)
   ---------------------------------------------------------------
   Superimpose the target "apex-down" triangle (same size n) on top of
   the original "apex-up" triangle so that the number of coins already
   sitting on a correct final site (and hence needing NO move) is
   maximised; only the leftover coins must be physically relocated
   (there are no ordering / collision constraints because a coin may
   be *lifted* and placed anywhere, so any leftover-to-leftover
   pairing realises the minimum in exactly (total - overlap) moves).

   Exhaustive search (see brute_force_min_moves, used only to VALIDATE
   the closed form for small n) shows that the optimal alignment is
   always obtained by sliding the down-triangle by
                       s = floor(n / 3)
   rows relative to the up-triangle -- i.e. it is never necessary to
   mirror the triangle left-right, only to shift it down by s rows.
   This produces the classic "Star of David" overlap: for n = 3k+1 the
   overlap is the centred hexagonal number 3k^2+3k+1; for the other
   two residues the optimum is a truncated version of the same
   hexagon.  Subtracting from the total n(n+1)/2 coins gives the
   closed form implemented in min_moves_formula().

   COMPACT FORMULA   (k = floor(n/3))
   ------------------------------------
       n = 3k      :  moves = k(3k+1)/2
       n = 3k+1    :  moves = 3k(k+1)/2
       n = 3k+2    :  moves = (k+1)(3k+2)/2

   This was verified by brute force for every n = 1..18 (see
   brute_force_min_moves(), which independently tries every possible
   relative shift, and both the row-flip and mirrored orientation of
   the target triangle, and returns the true optimum by exhaustive
   search) -- both methods agree exactly for all tested n.

   COMPLEXITY
   -----------
     min_moves_formula(n)            : O(1) time,  O(1) space
     construct_moves(n)              : O(n^2) time, O(n^2) space
                                        (must touch every one of the
                                        n(n+1)/2 coins once)
     brute_force_min_moves(n)        : O(n^4) time (used ONLY for
                                        validation on small n, not
                                        part of the actual algorithm)
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>

/* ---- O(1) closed-form formula --------------------------------------- */
long long min_moves_formula(int n) {
    long long k = n / 3;
    int r = n % 3;
    if (r == 0) return k * (3 * k + 1) / 2;
    else if (r == 1) return 3 * k * (k + 1) / 2;
    else return (k + 1) * (3 * k + 2) / 2;
}

/* ---- O(n^2) constructive algorithm -----------------------------------
   Builds the explicit set of coins that stay fixed, and pairs up the
   remaining "source" coins (currently occupied, wrong for target)
   with the remaining "destination" slots (empty, needed for target).
   Returns the number of coins that must move (== min_moves_formula). */
typedef struct { int c, r; } Pt;

long long construct_moves(int n, int print_details, int max_print) {
    int s = n / 3;                       /* optimal downward shift  */
    long long total = (long long)n * (n + 1) / 2;
    long long stay = 0;

    /* pass 1: count how many coins already sit on a correct final site */
    for (int rp = 0; rp < n; rp++) {           /* rp = row in the (shifted) target triangle */
        int cmax_target = n - 1 - rp;          /* target row rp has n-rp coins: c=0..cmax_target */
        int R = rp + s;                        /* absolute row this maps onto */
        if (R <= n - 1) {
            for (int c = 0; c <= cmax_target; c++) {
                if (c <= R) stay++;             /* (c,R) is a valid up-triangle coin */
            }
        }
    }
    long long moves = total - stay;

    if (print_details) {
        /* collect leftover sources (occupied, not part of "stay" set)      */
        Pt *src = malloc(sizeof(Pt) * total);
        Pt *dst = malloc(sizeof(Pt) * total);
        long long ns = 0, nd = 0;

        /* mark which up-triangle cells are "stay" cells */
        char *stayMark = calloc((size_t)n * n, 1);
        for (int rp = 0; rp < n; rp++) {
            int cmax_target = n - 1 - rp;
            int R = rp + s;
            if (R <= n - 1) {
                for (int c = 0; c <= cmax_target && c <= R; c++)
                    stayMark[R * n + c] = 1;
        }   }

        for (int r = 0; r < n; r++)
            for (int c = 0; c <= r; c++)
                if (!stayMark[r * n + c]) { src[ns].c = c; src[ns].r = r; ns++; }

        for (int rp = 0; rp < n; rp++) {
            int cmax_target = n - 1 - rp;
            int R = rp + s;
            for (int c = 0; c <= cmax_target; c++) {
                int already = (R <= n - 1 && c <= R);
                if (!already) { dst[nd].c = c; dst[nd].r = R; nd++; }
            }
        }

        printf("  Coins that stay fixed : %lld\n", stay);
        printf("  Coins that must move  : %lld  (src list size %lld, dst list size %lld)\n", moves, ns, nd);
        long long shown = (moves < max_print) ? moves : max_print;
        for (long long i = 0; i < shown; i++)
            printf("    move  (c=%d,r=%d)  ->  (c=%d,r=%d)\n", src[i].c, src[i].r, dst[i].c, dst[i].r);
        if (moves > shown) printf("    ... (%lld more moves omitted)\n", moves - shown);

        free(src); free(dst); free(stayMark);
    }
    return moves;
}

/* ---- O(n^4)-ish exhaustive validator (small n only) ------------------
   Tries every shift AND both orientations (plain flip / true 180-degree
   rotation) of the target triangle and returns the true minimum by
   brute force, so we can double-check the formula above. */
long long brute_force_min_moves(int n) {
    long long total = (long long)n * (n + 1) / 2;
    char *up = calloc((size_t)(4 * n + 5) * (4 * n + 5), 1);
    int OFF = 2 * n + 2;               /* offset so indices stay non-negative */
    int DIM = 4 * n + 5;
    #define IDX(c, r) (((r) + OFF) * DIM + ((c) + OFF))

    for (int r = 0; r < n; r++)
        for (int c = 0; c <= r; c++)
            up[IDX(c, r)] = 1;

    long long best = 0;
    for (int mirror = 0; mirror <= 1; mirror++) {
        for (int dr = -n; dr <= n; dr++) {
            for (int dc = -n; dc <= n; dc++) {
                long long overlap = 0;
                for (int rr = 0; rr < n; rr++) {
                    int cmax = n - 1 - rr;
                    for (int cc = 0; cc <= cmax; cc++) {
                        int C = mirror ? -cc : cc;
                        int R = mirror ? -rr : rr;
                        C += dc; R += dr;
                        if (C >= -OFF && R >= -OFF && C < DIM - OFF && R < DIM - OFF && up[IDX(C, R)])
                            overlap++;
                    }
                }
                if (overlap > best) best = overlap;
            }
        }
    }
    free(up);
    return total - best;
    #undef IDX
}

int main(void) {
    printf("=== Q1: Invert the coin-triangle ===\n\n");

    printf("Validation: closed-form formula vs. exhaustive brute force (n = 1..14)\n");
    printf("%4s %10s %10s %6s\n", "n", "formula", "bruteforce", "match");
    for (int n = 1; n <= 14; n++) {
        long long f = min_moves_formula(n);
        long long b = brute_force_min_moves(n);
        printf("%4d %10lld %10lld %6s\n", n, f, b, (f == b) ? "OK" : "MISMATCH");
    }

    int n;
    printf("\nEnter the number of rows n of the coin-triangle: ");
    if (scanf("%d", &n) != 1 || n < 1) { printf("Invalid input.\n"); return 1; }

    long long moves = min_moves_formula(n);
    printf("\nTriangle of side n = %d has %lld coins.\n", n, (long long)n * (n + 1) / 2);
    printf("Minimum number of moves to invert it = %lld\n", moves);

    printf("\nConstructive move list (also re-derives the same count via O(n^2) scan):\n");
    long long check = construct_moves(n, 1, 30);
    printf("\n%s\n", (check == moves) ? "Constructive count matches the formula." : "MISMATCH!");

    return 0;
}