# Merge Sort vs Modified (3-way) Merge Sort — EMPIRICAL Validation

This version does **not** plot a formula. It actually implements both
sorting algorithms in C, runs them on real (randomly generated) input
arrays of increasing size, measures the real cost, and plots the
measured data.

## 1. What's measured

For each algorithm, `mergesort_empirical.c`:

1. Generates a random array of size `n` (same array reused for both
   algorithms at a given trial, for a fair comparison).
2. Runs the **actual recursive merge sort** (2-way, halves) and the
   **actual recursive modified merge sort** (3-way, thirds) on it.
3. Counts the real number of **key comparisons** executed inside the
   merge step of each call (the dominant operation — this is what
   makes the measurement robust to system noise, unlike raw wall-clock
   time alone).
4. Also records real **wall-clock time** (`clock()`), for reference.
5. Repeats 5 trials per size and averages both metrics.
6. Also computes `measured_cost / (n log_k n)` for each algorithm
   (k=2 standard, k=3 modified) — if this ratio **flattens out** as n
   grows, that is the empirical proof that the measured running time
   really is Θ(n log n), without assuming the formula up front.

Sizes tested: 500 up to 2,000,000, logarithmically spaced.

## 2. Result (from the run used to build the plots here)

| n | comparisons (2-way) | comparisons (3-way) | time 2-way (ms) | time 3-way (ms) | cmp2/(n log2n) | cmp3/(n log3n) |
|---|---|---|---|---|---|---|
| 500 | 3,858 | 4,732 | 0.03 | 0.04 | 0.861 | 1.673 |
| 32,000 | 438,646 | 545,191 | 2.58 | 2.91 | 0.916 | 1.804 |
| 500,000 | 8,837,150 | 11,039,771 | 52.1 | 57.2 | 0.934 | 1.849 |
| 2,000,000 | 39,348,985 | 49,191,621 | 236.4 | 247.6 | 0.940 | 1.862 |

The last two columns **flatten** toward ~0.94 and ~1.86 respectively as
n grows — direct empirical confirmation that both algorithms run in
Θ(n log n), and that the 3-way version has a larger constant factor
(more comparisons per merge step) even though it does fewer recursion
levels. Full data: `mergesort_empirical.dat`.

## 3. Files

- `mergesort_empirical.c` — implements both merge-sort variants for
  real, runs them, counts comparisons and times them.
- `plot_empirical_comparisons.gnuplot` — measured comparison counts vs
  n, log-log axes (`mergesort_empirical_comparisons.png`).
- `plot_empirical_time.gnuplot` — measured wall-clock time vs n
  (`mergesort_empirical_time.png`).
- `plot_empirical_ratio.gnuplot` — measured_cost/(n log_k n), the
  flattening-to-constant validation plot
  (`mergesort_empirical_ratio.png`).
- `*_preview.png` — preview renders (gnuplot isn't installed in this
  sandbox; use the `.gnuplot` scripts to reproduce exactly with gnuplot
  on your machine).
- `mergesort_empirical.dat` — the raw measured data.

## 4. Build & run

```bash
gcc -O3 -o mergesort_empirical mergesort_empirical.c -lm
./mergesort_empirical                 # uses the built-in size list, 5 trials
./mergesort_empirical 0 10            # same sizes, 10 trials (less noise)

gnuplot plot_empirical_comparisons.gnuplot   # -> mergesort_empirical_comparisons.png
gnuplot plot_empirical_time.gnuplot          # -> mergesort_empirical_time.png
gnuplot plot_empirical_ratio.gnuplot         # -> mergesort_empirical_ratio.png
```

To change the sizes tested, edit the `default_sizes[]` array at the top
of `main()` in `mergesort_empirical.c`.
