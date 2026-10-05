/* Q8: Optimal Binary Search Tree (CLRS 15.5)
 * INPUT : n, then p1..pn (key probabilities), then q0..qn (dummy-key probabilities)
 *         (p's and q's together should sum to 1)
 * OUTPUT: minimum expected search cost, root table, and the tree structure
 *
 * e[i][j] = expected cost of an optimal BST on keys k_i..k_j
 * w[i][j] = sum p_i..p_j + sum q_{i-1}..q_j
 * e[i][i-1] = q_{i-1}
 * e[i][j]   = min_{i<=r<=j} ( e[i][r-1] + e[r+1][j] + w[i][j] )
 * Time  : O(n^3)  (O(n^2) with Knuth's monotonicity root[i][j-1] <= root[i][j] <= root[i+1][j])
 * Space : O(n^2)
 * The cost is also recomputed independently by walking the built tree (cross-check).
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int n, W;
static double *e, *w, *p, *q;
static int *root;
#define E(i, j) e[(i) * W + (j)]
#define Wt(i, j) w[(i) * W + (j)]
#define R(i, j) root[(i) * W + (j)]

static void build(int i, int j, int parent, int isLeft) {
    if (j == i - 1) {
        if (parent == 0) printf("  d%d is the root\n", j);
        else printf("  d%d is the %s child of k%d\n", j, isLeft ? "left" : "right", parent);
        return;
    }
    int r = R(i, j);
    if (parent == 0) printf("  k%d is the root\n", r);
    else printf("  k%d is the %s child of k%d\n", r, isLeft ? "left" : "right", parent);
    build(i, r - 1, r, 1);
    build(r + 1, j, r, 0);
}
static double tree_cost(int i, int j, int depth) {
    if (j == i - 1) return q[j] * (depth + 1);
    int r = R(i, j);
    return p[r] * (depth + 1) + tree_cost(i, r - 1, depth + 1) + tree_cost(r + 1, j, depth + 1);
}

int main(void) {
    printf("Enter number of keys n: ");
    if (scanf("%d", &n) != 1 || n < 0) { fprintf(stderr, "Invalid n\n"); return 1; }
    p = calloc(n + 2, sizeof(double));
    q = calloc(n + 2, sizeof(double));
    printf("Enter p1..p%d: ", n);
    for (int i = 1; i <= n; i++) if (scanf("%lf", &p[i]) != 1) return 1;
    printf("Enter q0..q%d: ", n);
    for (int i = 0; i <= n; i++) if (scanf("%lf", &q[i]) != 1) return 1;

    double total = 0;
    for (int i = 1; i <= n; i++) total += p[i];
    for (int i = 0; i <= n; i++) total += q[i];
    if (fabs(total - 1.0) > 1e-6) printf("Note: probabilities sum to %.6f (not 1)\n", total);

    W = n + 1;
    e = calloc((size_t)(n + 2) * W, sizeof(double));
    w = calloc((size_t)(n + 2) * W, sizeof(double));
    root = calloc((size_t)(n + 2) * W, sizeof(int));
    if (!e || !w || !root) return 1;

    for (int i = 1; i <= n + 1; i++) { E(i, i - 1) = q[i - 1]; Wt(i, i - 1) = q[i - 1]; }
    for (int l = 1; l <= n; l++)
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            E(i, j) = INFINITY;
            Wt(i, j) = Wt(i, j - 1) + p[j] + q[j];
            for (int r = i; r <= j; r++) {
                double t = E(i, r - 1) + E(r + 1, j) + Wt(i, j);
                if (t < E(i, j)) { E(i, j) = t; R(i, j) = r; }
            }
        }

    printf("Minimum expected search cost: %.4f\n", E(1, n));
    if (n >= 1) {
        printf("Root table (row i, column j, entry root[i][j]):\n");
        for (int i = 1; i <= n; i++) {
            printf("  i=%d:", i);
            for (int j = 1; j <= n; j++) {
                if (j < i) printf("  . ");
                else printf(" %2d ", R(i, j));
            }
            printf("\n");
        }
    }
    printf("Tree structure:\n");
    build(1, n, 0, 0);
    double chk = tree_cost(1, n, 0);
    printf("Cost recomputed from tree: %.4f %s\n", chk, fabs(chk - E(1, n)) < 1e-9 ? "[match]" : "[MISMATCH]");
    free(e); free(w); free(root); free(p); free(q);
    return 0;
}