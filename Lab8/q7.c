/* Q7: Rod Cutting with reconstruction
 * INPUT : n, then prices p1..pn (price of a piece of length i)
 * OUTPUT: maximum revenue and the piece lengths of an optimal cut
 *
 * r[0]=0 ; r[j] = max_{1<=i<=j} ( p[i] + r[j-i] ) ; s[j] = the i achieving the max
 * Reconstruction: while j>0: output s[j]; j -= s[j].
 * Time  : O(n^2)   Space : O(n)
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    printf("Enter rod length n: ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }
    long long *p = malloc((n + 1) * sizeof(long long)), *r = malloc((n + 1) * sizeof(long long));
    int *s = malloc((n + 1) * sizeof(int));
    if (!p || !r || !s) return 1;
    printf("Enter prices p1..p%d: ", n);
    for (int i = 1; i <= n; i++)
        if (scanf("%lld", &p[i]) != 1) { fprintf(stderr, "Bad input\n"); return 1; }

    r[0] = 0; s[0] = 0;
    for (int j = 1; j <= n; j++) {
        r[j] = p[1] + r[j - 1]; s[j] = 1;
        for (int i = 2; i <= j; i++)
            if (p[i] + r[j - i] > r[j]) { r[j] = p[i] + r[j - i]; s[j] = i; }
    }
    printf("Maximum revenue: %lld\nPieces: ", r[n]);
    int cnt = 0;
    for (int j = n; j > 0; j -= s[j]) { printf("%d ", s[j]); cnt++; }
    printf("\n(%d piece%s; %s)\n", cnt, cnt == 1 ? "" : "s", cnt == 1 ? "rod left uncut" : "rod was cut");
    free(p); free(r); free(s);
    return 0;
}