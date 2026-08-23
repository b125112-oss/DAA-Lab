# plot_vary_k_ratio.gnuplot
# Two panels: cmp1/(k^2*n) and cmp2/(k*n*log2 k), both vs k.
# Flattening to a constant is the empirical proof of the stated bounds.
# Run:  gnuplot plot_vary_k_ratio.gnuplot
# Requires kway_vary_k.dat.

set terminal pngcairo size 1000,900 enhanced font 'Verdana,10'
set output 'kway_vary_k_ratio.png'

set multiplot layout 2,1 title "Empirical validation: ratio -> constant as k grows"
set grid
set logscale x 2
set xlabel "k"

set title "Method 1: measured comparisons / (k^2 * n)  ->  Theta(k^2 n)"
set ylabel "ratio"
set yrange [0:1]
unset key
plot 'kway_vary_k.dat' using 1:7 with linespoints lw 2 pt 7 ps 0.8 lc rgb 'web-blue'

set title "Method 2: measured comparisons / (k * n * log_2 k)  ->  Theta(kn log k)"
set ylabel "ratio"
set yrange [0:1.2]
plot 'kway_vary_k.dat' using 1:8 with linespoints lw 2 pt 5 ps 0.8 lc rgb 'dark-orange'

unset multiplot
