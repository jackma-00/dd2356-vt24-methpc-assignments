# Set output file and terminal
set terminal pngcairo enhanced font 'Arial,12'
set output 'quiver_plot.png'

# Define arrow style
set style arrow 1 head filled size screen 0.03,15,60

#set xrange [0:10]
#set yrange [0:10]
#set size ratio -1

# Plot vector arrows
plot 'data.txt' using 1:2:3:4 with vectors arrowstyle 1
