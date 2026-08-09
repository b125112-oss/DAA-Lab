# Dictionary Operations — Worst-Case Complexity Analysis & Plots

## 1. Worst-case running times

| Operation   | Unsorted array | Sorted array | Singly linked (unsorted) | Singly linked (sorted) | Doubly linked (unsorted) | Doubly linked (sorted) |
|-------------|:---:|:---:|:---:|:---:|:---:|:---:|
| Search      | O(n) | O(log n) | O(n) | O(n) | O(n) | O(n) |
| Insert      | O(1) | O(n) | O(1) | O(n) | O(1) | O(n) |
| Delete      | O(1) | O(n) | O(n) | O(n) | O(1) | O(1) |
| Max         | O(n) | O(1) | O(n) | O(n) | O(n) | O(1) |
| Min         | O(n) | O(1) | O(n) | O(1) | O(n) | O(1) |
| Predecessor | O(n) | O(1) | O(n) | O(n) | O(n) | O(1) |
| Successor   | O(n) | O(1) | O(n) | O(1) | O(n) | O(1) |

**Assumptions (stated explicitly, since these change a few entries):**
- Linked lists keep only a **head** pointer, except the doubly linked **sorted**
  list, which also keeps a **tail** pointer (that's why its Max is O(1), matching
  its Min).
- `Delete(D, x)` is given a pointer to the node/element, as stated in the
  problem.
  - Array: index known → swap-with-last is O(1) for the *unsorted* array;
    a sorted array must shift elements, O(n).
  - Singly linked: even with a pointer to the node, you still need the
    **predecessor** to unlink it (no back-link), so it's O(n) — the pointer
    doesn't save you the traversal.
  - Doubly linked: the node's own `prev` pointer makes unlinking O(1).
- For the sorted linked lists, `Predecessor`/`Successor` are O(1) exactly
  when the direction that gives you the answer is a direct pointer
  (`next` for singly linked sorted → Successor O(1), Predecessor O(n);
  both `prev`/`next` for doubly linked sorted → both O(1)).
- `Insert`/`Search` on unsorted structures can't do better than O(n)/O(1)
  respectively; sorted array search is O(log n) via binary search, but
  sorted array insert still needs O(n) to shift elements into place.

## 2. Files

- `complexity_gen.c` — C program that evaluates the **closed-form**
  worst-case cost functions (O(1), O(log₂n), O(n)) for n = 1..N according
  to the table above. It does not build any data structures and does not
  time anything — it only computes the mathematical growth functions and
  writes them to `.dat` files gnuplot can read directly.
- `plot_growth.gnuplot` — plots the three base growth curves O(1), O(log₂n),
  O(n) on one chart (`growth_functions.png`).
- `plot_operations.gnuplot` — a 7-panel multiplot, one panel per operation,
  overlaying all six implementations' worst-case curves (`op_comparison.png`).
- `growth_functions_preview.png`, `op_comparison_preview.png` — preview
  renders of the two plots (generated here since gnuplot isn't installed
  in this sandbox; use the `.gnuplot` scripts below to reproduce them
  exactly with gnuplot on your own machine).

## 3. Build & run

```bash
# 1. Compile and run the generator (produces growth_functions.dat and op_*.dat)
gcc -O2 -o complexity_gen complexity_gen.c -lm
./complexity_gen 100        # 100 = max n; pass any N you like

# 2. Plot with gnuplot
gnuplot plot_growth.gnuplot       # -> growth_functions.png
gnuplot plot_operations.gnuplot   # -> op_comparison.png
```

Increase N (e.g. `./complexity_gen 100000`) to see the linear and
logarithmic curves separate even more dramatically.
