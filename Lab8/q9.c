/* Q9: Collatz Conjecture trajectory analyser (modular C program)
 *
 * Menu: 1) trajectory of a single n   2) statistics over an interval [a,b]
 * Concepts: iteration, functional decomposition, dynamic memory, overflow handling.
 *
 * Overflow: 3n+1 is only computed if n <= (ULLONG_MAX-1)/3, otherwise the walk is
 * reported as "overflow" instead of silently wrapping around (which could fake a cycle).
 * Complexity: one start value costs O(L) time, L = trajectory length (no known closed
 * bound -- that is exactly the open problem). Interval: O(sum of L) time, O(b-a+1) space.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long long u64;
typedef struct { u64 *v; size_t len, cap; } Vec;

static int collatz_next(u64 n, u64 *out) {
    if (n % 2 == 0) { *out = n / 2; return 1; }
    if (n > (ULLONG_MAX - 1) / 3) return 0;     /* 3n+1 would overflow */
    *out = 3 * n + 1;
    return 1;
}

static int collatz_stats(u64 n, u64 *steps, u64 *peak) {
    u64 s = 0, mx = n;
    while (n != 1) {
        if (!collatz_next(n, &n)) { *steps = s; *peak = mx; return 0; }
        s++;
        if (n > mx) mx = n;
    }
    *steps = s; *peak = mx;
    return 1;
}

static void vec_push(Vec *a, u64 x) {
    if (a->len == a->cap) {
        a->cap = a->cap ? a->cap * 2 : 64;
        a->v = realloc(a->v, a->cap * sizeof(u64));
        if (!a->v) { fprintf(stderr, "Out of memory\n"); exit(1); }
    }
    a->v[a->len++] = x;
}

static void analyse_single(u64 n) {
    Vec t = {0};
    u64 x = n, peak = n;
    int ok = 1;
    vec_push(&t, x);
    while (x != 1) {
        if (!collatz_next(x, &x)) { ok = 0; break; }
        vec_push(&t, x);
        if (x > peak) peak = x;
    }
    printf("Trajectory of %llu:\n", n);
    for (size_t i = 0; i < t.len; i++) printf("%llu%s", t.v[i], i + 1 < t.len ? " -> " : "\n");
    if (ok) printf("Steps to reach 1: %zu | Peak value: %llu\n", t.len - 1, peak);
    else printf("Stopped: next 3n+1 would overflow 64 bits after %zu steps (peak so far %llu)\n", t.len - 1, peak);
    free(t.v);
}

static void analyse_interval(u64 a, u64 b) {
    u64 cnt = b - a + 1;
    if (cnt > 100000000ULL) { fprintf(stderr, "Interval too large\n"); return; }
    u64 *steps = malloc(cnt * sizeof(u64)), *peak = malloc(cnt * sizeof(u64));
    if (!steps || !peak) { fprintf(stderr, "Out of memory\n"); exit(1); }
    u64 bestS = 0, bestSn = a, bestP = 0, bestPn = a, sum = 0, ovf = 0, okCnt = 0;
    for (u64 n = a; n <= b; n++) {
        int ok = collatz_stats(n, &steps[n - a], &peak[n - a]);
        if (!ok) { ovf++; }
        else {
            okCnt++;
            sum += steps[n - a];
            if (steps[n - a] > bestS) { bestS = steps[n - a]; bestSn = n; }
            if (peak[n - a] > bestP)  { bestP = peak[n - a];  bestPn = n; }
        }
        if (n == b) break;                      /* guards b == ULLONG_MAX */
    }
    if (cnt <= 30) {
        printf("%8s %8s %20s\n", "n", "steps", "peak");
        for (u64 i = 0; i < cnt; i++) printf("%8llu %8llu %20llu\n", a + i, steps[i], peak[i]);
    }
    printf("Interval [%llu, %llu]: %llu numbers, %llu reached 1, %llu overflowed\n", a, b, cnt, okCnt, ovf);
    if (okCnt) {
        printf("Longest trajectory : n=%llu with %llu steps\n", bestSn, bestS);
        printf("Highest peak       : n=%llu reaches %llu\n", bestPn, bestP);
        printf("Average steps      : %.3f\n", (double)sum / (double)okCnt);
    }
    free(steps); free(peak);
}

int main(void) {
    int choice;
    long long a, b;
    printf("1) Single start value\n2) Interval [a,b]\nChoice: ");
    if (scanf("%d", &choice) != 1) return 1;
    if (choice == 1) {
        printf("Enter n >= 1: ");
        if (scanf("%lld", &a) != 1 || a < 1) { fprintf(stderr, "n must be >= 1\n"); return 1; }
        analyse_single((u64)a);
    } else if (choice == 2) {
        printf("Enter a and b (1 <= a <= b): ");
        if (scanf("%lld %lld", &a, &b) != 2 || a < 1 || b < a) { fprintf(stderr, "Invalid interval\n"); return 1; }
        analyse_interval((u64)a, (u64)b);
    } else { fprintf(stderr, "Invalid choice\n"); return 1; }
    return 0;
}