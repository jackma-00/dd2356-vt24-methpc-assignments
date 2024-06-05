# Set the output file format and name
set terminal pngcairo enhanced font 'Verdana,12'
set output 'speedup_plot.png'

# Set the title and labels for axes
set title "Speed-up Plot"
set xlabel "Number of Processes"
set ylabel "Speed-up (times faster)"

# Set the range for x and y axes
set xrange [1:128]
set yrange [0:*]

# Plot the speed-up
plot "speedup_data.txt" using 1:(1.0/$2) with linespoints title "Speed-up"
