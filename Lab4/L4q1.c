
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int number;
    char color; /* 'R', 'B', or 'Y' */
} Item;

static int colorIndex(char c) {
    if (c == 'R' || c == 'r') return 0;
    if (c == 'B' || c == 'b') return 1;
    if (c == 'Y' || c == 'y') return 2;
    return -1;
}

static const char *colorName(int idx) {
    switch (idx) {
        case 0: return "Red";
        case 1: return "Blue";
        case 2: return "Yellow";
        default: return "?";
    }
}

/* O(n) stable sort by colour (Red, Blue, Yellow). */
static void sortByColour(const Item *items, int n, Item *out) {
    int count[3] = {0, 0, 0};
    for (int i = 0; i < n; i++) count[colorIndex(items[i].color)]++;

    int start[3];
    start[0] = 0;
    start[1] = count[0];
    start[2] = count[0] + count[1];

    for (int i = 0; i < n; i++) {
        int c = colorIndex(items[i].color);
        out[start[c]++] = items[i];
    }
}

int main(void) {
    int n;
    printf("Enter number of items (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Invalid n\n");
        return 1;
    }

    Item *items = malloc((size_t)n * sizeof(Item));
    Item *out   = malloc((size_t)n * sizeof(Item));
    if (!items || !out) { fprintf(stderr, "Out of memory\n"); return 1; }

    printf("Enter %d pairs as: <number> <colour: R/B/Y>\n", n);
    printf("(assumed already sorted by number)\n");
    for (int i = 0; i < n; i++) {
        char c;
        if (scanf("%d %c", &items[i].number, &c) != 2 || colorIndex(c) == -1) {
            fprintf(stderr, "Invalid input at item %d\n", i);
            free(items); free(out);
            return 1;
        }
        items[i].color = c;
    }

    sortByColour(items, n, out);

    printf("\nSorted by colour (Red, Blue, Yellow); numbers stay sorted within a colour:\n");
    for (int i = 0; i < n; i++)
        printf("(%d, %s)\n", out[i].number, colorName(colorIndex(out[i].color)));

    free(items);
    free(out);
    return 0;
}