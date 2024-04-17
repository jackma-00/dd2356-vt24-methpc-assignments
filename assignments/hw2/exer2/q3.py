import numpy as np
import matplotlib.pyplot as plt

static_best_rates_copy = [5664.5, 5707.2, 4956.0, 5013.7]
dynamic_best_rates_copy = [156.9, 156.3, 155.7, 158.1, 154.4]
guided_best_rates_copy = [5001.4, 4377.3, 5000.9, 5770.1, 4463.0]

# take average of best rates
static_best_rates_copy_avg = np.mean(static_best_rates_copy)
dynamic_best_rates_copy_avg = np.mean(dynamic_best_rates_copy)
guided_best_rates_copy_avg = np.mean(guided_best_rates_copy)

# data to plot
functions = ['Static', 'Dynamic', 'Guided']
averages = [static_best_rates_copy_avg, dynamic_best_rates_copy_avg, guided_best_rates_copy_avg]

# custom colors and shapes
colors = ['skyblue', 'lightgreen', 'salmon']
patterns = ['/', 'o', '+']

# create plot
for i in range(len(functions)):
    plt.bar(functions[i], averages[i], color=colors[i], hatch=patterns[i])

plt.xlabel('Function')
plt.ylabel('Best Rate MB/s')
plt.title('Best Rate MB/s by Function')
plt.savefig('best_rates.png')
plt.show()
