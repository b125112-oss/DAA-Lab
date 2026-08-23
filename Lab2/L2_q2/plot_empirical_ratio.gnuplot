# plot_empirical_ratio.gnuplot
# Plots measured_comparisons(n) / (n * log_k(n)) for both algorithms
# (k=2 for standard, k=3 for modified). If the real implementation is
# Theta(n log n), this ratio should FLATTEN toward a constant as n grows
# -- that flattening, computed from measured data, is the empirical
# validation of the O(n log n) order of growth (as opposed to just
# plotting the formula itself).
# Run:  gnuplot plot_empirical_ratio.gnuplot
# Requires mergesort_empirical.dat (produced by mergesort_empirical).

set terminal pngcairo size 1000,650 enhanced font 'Verdana,11'
set output 'mergesort_empirical_ratio.png'

set title "Measured comparisons / (n log_k n): flattening confirms Theta(n log n)"
set xlabel "n (input size)"
set ylabel "measured cost / (n log_k n)"
set logscale x 10
set key top right
set grid
set yrange [0:2.2]

plot 'mergesort_empirical.dat' using 1:6 with linespoints lw 2 pt 7 ps 0.7 title 'Standard: cmp2/(n log_2 n)', \
     ''                        using 1:7 with linespoints lw 2 pt 5 ps 0.7 title 'Modified: cmp3/(n log_3 n)'
