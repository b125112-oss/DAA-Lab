# plot_vary_k_raw.gnuplot
# Measured comparisons vs k (n fixed), log-log axes.
# Method 1 should show slope ~2 (k^2 growth); Method 2 should show
# slope ~1 with a slowly-growing log factor (k log k growth) -- visibly
# much flatter than Method 1 on this log-log plot.
# Run:  gnuplot plot_vary_k_raw.gnuplot
# Requires kway_vary_k.dat.

set terminal pngcairo size 1000,650 enhanced font 'Verdana,11'
set output 'kway_vary_k_raw.png'

set title "Merging k sorted arrays (n fixed): measured comparisons vs k"
set xlabel "k (number of arrays)"
set ylabel "number of key comparisons (measured)"
set logscale x 2
set logscale y 10
set key top left
set grid

plot 'kway_vary_k.dat' using 1:3 with linespoints lw 2 pt 7 ps 0.7 title 'Method 1: sequential merge', \
     ''                using 1:4 with linespoints lw 2 pt 5 ps 0.7 title 'Method 2: pairwise/tournament merge'
