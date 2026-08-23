#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int time;
    int type; /* +1 = arrival, -1 = departure */
} Event;

static int cmpEvent(const void *a, const void *b) {
    return ((const Event *)a)->time - ((const Event *)b)->time;
}

/* O(n log n): build 2n arrival/departure events, sort by time, sweep. */
static int maxOverlapTime(const int *a, const int *b, int n, int *bestCount) {
    Event *events = malloc((size_t)(2 * n) * sizeof(Event));
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){a[i], +1};
        events[2 * i + 1] = (Event){b[i], -1};
    }
    qsort(events, (size_t)(2 * n), sizeof(Event), cmpEvent);

    int count = 0, maxCount = 0, bestTime = 0;
    for (int i = 0; i < 2 * n; i++) {
        if (events[i].type == +1) {
            count++;
            if (count > maxCount) { maxCount = count; bestTime = events[i].time; }
        } else {
            count--;
        }
    }
    free(events);
    *bestCount = maxCount;
    return bestTime;
}

int main(void) {
    int n;
    printf("Enter number of persons (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) { fprintf(stderr, "Invalid n\n"); return 1; }

    int *a = malloc((size_t)n * sizeof(int));
    int *b = malloc((size_t)n * sizeof(int));
    if (!a || !b) { fprintf(stderr, "Out of memory\n"); return 1; }

    printf("Enter %d (entry exit) pairs, entry < exit, all 2n times distinct:\n", n);
    for (int i = 0; i < n; i++) if (scanf("%d %d", &a[i], &b[i]) != 2) { fprintf(stderr, "Bad input\n"); return 1; }

    int bestCount;
    int bestTime = maxOverlapTime(a, b, n, &bestCount);

    printf("Maximum simultaneous presence: %d people, at time %d\n", bestCount, bestTime);

    free(a);
    free(b);
    return 0;
}