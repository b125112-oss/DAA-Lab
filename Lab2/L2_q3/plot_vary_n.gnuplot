# plot_vary_n.gnuplot
# Measured comparisons vs n (k fixed). Both methods should be straight
# lines through the origin (linear in n for fixed k), with Method 1's
# slope k^2 times steeper than Method 2's k*log2(k) slope.
# Run:  gnuplot plot_vary_n.gnuplot
# Requires kway_vary_n.dat.

set terminal pngcairo size 1000,650 enhanced font 'Verdana,11'
set output 'kway_vary_n.png'

set title "Merging k sorted arrays (k fixed): measured comparisons vs n"
set xlabel "n (size of each of the k arrays)"
set ylabel "number of key comparisons (measured)"
set key top left
set grid

plot 'kway_vary_n.dat' using 1:3 with linespoints lw 2 pt 7 ps 0.7 title 'Method 1: sequential merge', \
     ''                using 1:4 with linespoints lw 2 pt 5 ps 0.7 title 'Method 2: pairwise/tournament merge'
