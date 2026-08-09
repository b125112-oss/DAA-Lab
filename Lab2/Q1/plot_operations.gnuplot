# plot_operations.gnuplot
# One panel per Dictionary ADT operation. Each panel overlays the
# worst-case cost curve (as a function of n) for all six implementations:
#   UA  = unsorted array           SA  = sorted array
#   SLU = singly linked, unsorted  SLS = singly linked, sorted
#   DLU = doubly linked, unsorted  DLS = doubly linked, sorted
#
# Run:  gnuplot plot_operations.gnuplot
# Requires op_search.dat, op_insert.dat, op_delete.dat, op_max.dat,
# op_min.dat, op_predecessor.dat, op_successor.dat (from complexity_gen).

set terminal pngcairo size 1400,1700 enhanced font 'Verdana,9'
set output 'op_comparison.png'

set multiplot layout 4,2 title "Worst-case cost vs n, by operation and implementation"
set grid
set key top left font 'Verdana,7'
set xlabel "n"
set ylabel "cost"

UA  = "using 1:2 with lines lw 1.5 title 'UA'"
SA  = "using 1:3 with lines lw 1.5 title 'SA'"
SLU = "using 1:4 with lines lw 1.5 dt 2 title 'SLU'"
SLS = "using 1:5 with lines lw 1.5 dt 2 title 'SLS'"
DLU = "using 1:6 with lines lw 1.5 dt 3 title 'DLU'"
DLS = "using 1:7 with lines lw 1.5 dt 3 title 'DLS'"

set title "Search"
plot 'op_search.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

set title "Insert"
plot 'op_insert.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

set title "Delete"
plot 'op_delete.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

set title "Max"
plot 'op_max.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

set title "Min"
plot 'op_min.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

set title "Predecessor"
plot 'op_predecessor.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

set title "Successor"
plot 'op_successor.dat' @UA, '' @SA, '' @SLU, '' @SLS, '' @DLU, '' @DLS

unset multiplot
