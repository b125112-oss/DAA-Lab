# Merging k Sorted Arrays — Method 1 vs Method 2 (Empirical Validation)

## 1. Worst-case running time

**Method 1 (sequential merging):** merge array 1 with array 2 (cost
Θ(2n)), merge that Θ(2n)-length result with array 3 (cost Θ(3n)), then
with array 4 (cost Θ(4n)), ..., finally with array k (cost Θ(kn)).

```
T(n,k) = sum_{i=2}^{k} Theta(i*n) = Theta(n) * sum_{i=2}^{k} i
       = Theta(n) * Theta(k^2)          [since sum_{i=2}^{k} i = k(k+1)/2 - 1]
       = Theta(k^2 * n)
```

**Method 2 (pairwise / tournament merging):** pair up the k arrays and
merge each pair (k/2 merges), then pair up the k/2 results and merge
again, and so on — a balanced binary merge tree with ⌈log₂k⌉ rounds.
In every round, every element participates in exactly one merge, so
each round costs Θ(kn) regardless of round number:

```
T(n,k) = Theta(log_2 k) rounds * Theta(kn) per round = Theta(k * n * log_2 k)
```

**Conclusion:** Method 2 is asymptotically much better — Θ(kn log k)
vs Θ(k²n). For large k the gap is huge (e.g. k=1024: Method 1 does
~50× more comparisons than Method 2 in the measurements below).

## 2. Empirical approach (same method as the merge-sort part)

Like the previous merge-sort deliverable, this does **not** just plot
the formulas. `kway_merge_empirical.c` actually:

1. Builds k real sorted arrays of n random elements each.
2. Runs the **real Method 1** (sequential 2-way merge, repeated k−1
   times) and the **real Method 2** (pairwise/tournament merge tree)
   on the same input, counting actual key comparisons and wall-clock
   time.
3. **Experiment A** — fixes n = 200, varies k over powers of two from
   2 to 1024, averaged over 3 trials.
4. **Experiment B** — fixes k = 16, varies n from 1,000 to 500,000,
   averaged over 3 trials.
5. Computes `cmp1/(k²·n)` and `cmp2/(k·n·log₂k)`. If these ratios
   **flatten to a constant** as k grows, that is the empirical proof of
   the Θ(k²n) and Θ(kn log k) bounds — not an assumed formula.

## 3. Measured results

Experiment A (n = 200 fixed), ratio columns:

| k | cmp Method 1 | cmp Method 2 | cmp1/(k²n) | cmp2/(kn·log₂k) |
|---|---|---|---|---|
| 2 | 398 | 398 | 0.498 | 0.996 |
| 32 | 104,916 | 31,930 | 0.512 | 0.998 |
| 256 | 6,547,499 | 409,095 | 0.500 | 0.999 |
| 1024 | 104,445,077 | 2,045,955 | 0.498 | 0.999 |

Both ratio columns flatten (to ≈0.5 and ≈1.0 respectively) as k grows —
direct empirical confirmation of Θ(k²n) and Θ(kn log k). At k=1024,
Method 1 needs ~51× more comparisons than Method 2 for the same data.

Experiment B (k = 16 fixed) confirms both costs scale **linearly in n**
(cmp/n stays flat at ≈135 and ≈64 respectively across all tested n) —
consistent with both formulas being linear in n for fixed k.
Full data: `kway_vary_k.dat`, `kway_vary_n.dat`.

## 4. Files

- `kway_merge_empirical.c` — implements both merge methods for real,
  runs both experiments, counts comparisons and times them.
- `plot_vary_k_raw.gnuplot` — measured comparisons vs k, log-log, both
  methods (`kway_vary_k_raw.png`).
- `plot_vary_k_ratio.gnuplot` — two-panel ratio-flattening plot, the
  empirical proof of Θ(k²n) and Θ(kn log k) (`kway_vary_k_ratio.png`).
- `plot_vary_n.gnuplot` — measured comparisons vs n, fixed k, confirms
  linear-in-n scaling (`kway_vary_n.png`).
- `*_preview.png` — preview renders (gnuplot isn't installed in this
  sandbox; use the `.gnuplot` scripts to reproduce exactly with gnuplot
  on your machine).
- `kway_vary_k.dat`, `kway_vary_n.dat` — the raw measured data.

## 5. Build & run

```bash
gcc -O3 -o kway_merge_empirical kway_merge_empirical.c -lm
./kway_merge_empirical

gnuplot plot_vary_k_raw.gnuplot     # -> kway_vary_k_raw.png
gnuplot plot_vary_k_ratio.gnuplot   # -> kway_vary_k_ratio.png
gnuplot plot_vary_n.gnuplot         # -> kway_vary_n.png
```

To change the k or n values tested, edit the `ks[]` / `ns[]` arrays in
`main()` in `kway_merge_empirical.c`.
