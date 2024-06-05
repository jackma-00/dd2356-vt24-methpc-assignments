# Set the output file format and name
set terminal pngcairo enhanced font 'Verdana,12'
set output 'execution_time_plot.png'

# Set the title and labels for axes
set title "Execution Time Plot"
set xlabel "Number of Processes"
set ylabel "Execution Time (seconds)"

# Set the range for x and y axes
set xrange [1:128]
set yrange [0:*]

# Plot the execution time
plot "speedup_data.txt" using 1:2 with linespoints title "Execution Time"
