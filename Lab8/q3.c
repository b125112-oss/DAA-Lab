/* Q3: Longest Common Subsequence with reconstruction
 * INPUT : two lines, X and Y (any characters, spaces allowed, may be empty)
 * OUTPUT: LCS length and one LCS string
 *
 * L[i][j] = 0                          if i=0 or j=0
 *         = L[i-1][j-1] + 1            if X[i-1]==Y[j-1]
 *         = max(L[i-1][j], L[i][j-1])  otherwise
 * Reconstruction: walk back from (m,n) using only the table.
 * Time  : O(m*n) to fill, O(m+n) to reconstruct
 * Space : O(m*n) (O(min(m,n)) if only the length were needed)
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

int main(void) {
    printf("Enter string X: ");
    char *X = read_line();
    printf("Enter string Y: ");
    char *Y = read_line();
    size_t m = strlen(X), n = strlen(Y), W = n + 1;

    int *L = calloc((m + 1) * W, sizeof(int));
    if (!L) { fprintf(stderr, "Out of memory\n"); return 1; }

    for (size_t i = 1; i <= m; i++)
        for (size_t j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) L[i * W + j] = L[(i - 1) * W + j - 1] + 1;
            else {
                int up = L[(i - 1) * W + j], left = L[i * W + j - 1];
                L[i * W + j] = up >= left ? up : left;
            }
        }

    int len = L[m * W + n];
    char *lcs = malloc((size_t)len + 1);
    if (!lcs) return 1;
    lcs[len] = '\0';
    size_t i = m, j = n;
    int k = len;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) { lcs[--k] = X[i - 1]; i--; j--; }
        else if (L[(i - 1) * W + j] >= L[i * W + j - 1]) i--;
        else j--;
    }
    printf("LCS length: %d\nLCS: \"%s\"\n", len, lcs);
    free(X); free(Y); free(L); free(lcs);
    return 0;
}