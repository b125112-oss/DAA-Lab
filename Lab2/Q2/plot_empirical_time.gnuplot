# plot_empirical_time.gnuplot
# Plots the MEASURED wall-clock time (averaged over several trials) of
# the real merge-sort implementations against n.
# Run:  gnuplot plot_empirical_time.gnuplot
# Requires mergesort_empirical.dat (produced by mergesort_empirical).

set terminal pngcairo size 1000,650 enhanced font 'Verdana,11'
set output 'mergesort_empirical_time.png'

set title "Measured wall-clock time: standard (2-way) vs modified (3-way) merge sort"
set xlabel "n (input size)"
set ylabel "time (ms), averaged over trials"
set key top left
set grid

plot 'mergesort_empirical.dat' using 1:4 with linespoints lw 2 pt 7 ps 0.7 title 'Standard merge sort (measured)', \
     ''                        using 1:5 with linespoints lw 2 pt 5 ps 0.7 title 'Modified merge sort (measured)'
