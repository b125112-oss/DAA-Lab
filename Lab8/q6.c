/* Q6: Edit Distance (Levenshtein) with traceback
 * INPUT : two lines, A and B (spaces allowed, may be empty)
 * OUTPUT: minimum #operations and the sequence of operations turning A into B
 *
 * D[i][0]=i ; D[0][j]=j
 * D[i][j] = D[i-1][j-1]                          if A[i-1]==B[j-1]
 *         = 1 + min(D[i-1][j-1] (substitute),
 *                   D[i-1][j]   (delete A[i-1]),
 *                   D[i][j-1]   (insert B[j-1]))  otherwise
 * Traceback from (m,n) re-derives which choice produced each cell.
 * Time  : O(m*n) fill + O(m+n) traceback
 * Space : O(m*n)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_line(void) {
    size_t cap = 64, len = 0;
    char *b = malloc(cap);
    int ch;
    if (!b) exit(1);
    while ((ch = getchar()) != EOF && ch != '\n') {
        if (len + 2 > cap) { cap *= 2; b = realloc(b, cap); if (!b) exit(1); }
        b[len++] = (char)ch;
    }
    if (len && b[len - 1] == '\r') len--;
    b[len] = '\0';
    return b;
}
static int min3(int a, int b, int c) { int m = a < b ? a : b; return m < c ? m : c; }

int main(void) {
    printf("Enter string A: ");
    char *A = read_line();
    printf("Enter string B: ");
    char *B = read_line();
    size_t m = strlen(A), n = strlen(B), W = n + 1;
    int *D = malloc((m + 1) * W * sizeof(int));
    if (!D) { fprintf(stderr, "Out of memory\n"); return 1; }

    for (size_t i = 0; i <= m; i++) D[i * W] = (int)i;
    for (size_t j = 0; j <= n; j++) D[j] = (int)j;
    for (size_t i = 1; i <= m; i++)
        for (size_t j = 1; j <= n; j++) {
            if (A[i - 1] == B[j - 1]) D[i * W + j] = D[(i - 1) * W + j - 1];
            else D[i * W + j] = 1 + min3(D[(i - 1) * W + j - 1], D[(i - 1) * W + j], D[i * W + j - 1]);
        }
    printf("Edit distance: %d\n", D[m * W + n]);

    size_t cap = m + n + 1, cnt = 0;
    char *op = malloc(cap);
    size_t *oi = malloc(cap * sizeof(size_t)), *oj = malloc(cap * sizeof(size_t));
    size_t i = m, j = n;
    while (i > 0 || j > 0) {
        int cur = D[i * W + j];
        if (i > 0 && j > 0 && A[i - 1] == B[j - 1] && cur == D[(i - 1) * W + j - 1]) {
            op[cnt] = 'M'; oi[cnt] = i; oj[cnt] = j; cnt++; i--; j--;
        } else if (i > 0 && j > 0 && cur == D[(i - 1) * W + j - 1] + 1) {
            op[cnt] = 'S'; oi[cnt] = i; oj[cnt] = j; cnt++; i--; j--;
        } else if (i > 0 && cur == D[(i - 1) * W + j] + 1) {
            op[cnt] = 'D'; oi[cnt] = i; oj[cnt] = j; cnt++; i--;
        } else {
            op[cnt] = 'I'; oi[cnt] = i; oj[cnt] = j; cnt++; j--;
        }
    }
    printf("Edit script (M=match S=substitute D=delete I=insert): ");
    for (size_t k = cnt; k-- > 0;) putchar(op[k]);
    printf("\nOperations:\n");
    int step = 0;
    for (size_t k = cnt; k-- > 0;) {
        switch (op[k]) {
            case 'M': break;
            case 'S': printf("  %d. Substitute A[%zu]='%c' with '%c'\n", ++step, oi[k], A[oi[k]-1], B[oj[k]-1]); break;
            case 'D': printf("  %d. Delete A[%zu]='%c'\n", ++step, oi[k], A[oi[k]-1]); break;
            case 'I': printf("  %d. Insert '%c' (B[%zu]) after A[%zu]\n", ++step, B[oj[k]-1], oj[k], oi[k]); break;
        }
    }
    if (step == 0) printf("  (strings are identical)\n");
    free(A); free(B); free(D); free(op); free(oi); free(oj);
    return 0;
}