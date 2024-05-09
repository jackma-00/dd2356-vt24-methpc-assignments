set terminal pngcairo enhanced font 'Verdana,12'
set output 'execution_times.png'
set title "Execution Time vs. Number of MPI Processes"
set xlabel "Number of MPI Processes"
set ylabel "Execution Time (s)"
set xtics 8,8,128
set grid
plot "execution_times.txt" using 1:2 with linespoints lw 2 title "Execution Time"
