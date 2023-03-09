set title "Compare"
set xlabel "n^{th} of F(n)"
set ylabel "time(ns)"
set terminal png font " Times_New_Roman,12 "
set output "plot_time.png"
set xtics 0 ,10 ,100
set key left 

plot \
"test_time" using 1 with linespoints linewidth 2 title "user time", \
"test_time" using 2 with linespoints linewidth 2 title "kernel time", \
"test_time" using 3 with linespoints linewidth 2 title "system call", \

