# plot_empirical_comparisons.gnuplot
# Plots the MEASURED number of key comparisons performed by the real
# merge-sort implementations (not a formula) against n, on log-log axes.
# A straight-ish line with slope ~1 (times a log factor) on log-log axes
# is the classic empirical signature of n log n growth.
# Run:  gnuplot plot_empirical_comparisons.gnuplot
# Requires mergesort_empirical.dat (produced by mergesort_empirical).

set terminal pngcairo size 1000,650 enhanced font 'Verdana,11'
set output 'mergesort_empirical_comparisons.png'

set title "Measured comparisons: standard (2-way) vs modified (3-way) merge sort"
set xlabel "n (input size)"
set ylabel "number of key comparisons (measured)"
set logscale x 10
set logscale y 10
set key top left
set grid

plot 'mergesort_empirical.dat' using 1:2 with linespoints lw 2 pt 7 ps 0.7 title 'Standard merge sort (measured)', \
     ''                        using 1:3 with linespoints lw 2 pt 5 ps 0.7 title 'Modified merge sort (measured)'
