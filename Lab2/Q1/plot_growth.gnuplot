# plot_growth.gnuplot
# Plots the three worst-case growth-rate functions that appear in the
# dictionary-operations table: O(1), O(log2 n), O(n).
# Run:  gnuplot plot_growth.gnuplot
# Requires growth_functions.dat (produced by complexity_gen).

set terminal pngcairo size 900,600 enhanced font 'Verdana,11'
set output 'growth_functions.png'

set title "Order of Growth: O(1) vs O(log_2 n) vs O(n)"
set xlabel "n (number of elements in dictionary D)"
set ylabel "Worst-case cost f(n)"
set key top left
set grid

plot 'growth_functions.dat' using 1:2 with lines lw 2 title 'O(1)', \
     ''                     using 1:3 with lines lw 2 title 'O(log_2 n)', \
     ''                     using 1:4 with lines lw 2 title 'O(n)'
