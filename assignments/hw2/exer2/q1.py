TC1_copy = [16271.6, 16554.6, 14128.2, 14904.5, 15328.7]

TC1_avg_time_copy = [0.009915, 0.00976, 0.011637, 0.014633, 0.014375]



TC32_copy = [55595.1, 43873.5, 32732.8, 51052.8, 38050.0]

TC32_avg_time_copy = [0.004208, 0.005369, 0.0066, 0.00579, 0.005966]



TC64_copy = [13607.9, 10289.5, 28985.0, 29728.4, 10874.7]

TC64_avg_time_copy = [0.019645, 0.022365, 0.021306, 0.019802, 0.020214]


TC128_copy = [4001.8, 5847.1, 5117.1, 5000.3, 4471.5]

TC128_avg_time_copy = [0.0504, 0.04998, 0.054902, 0.052309, 0.050854]

import numpy as np


threads = [1, 32, 64, 128]
copy_data = np.array([TC1_copy, TC32_copy, TC64_copy, TC128_copy])

# Calculate the mean and standard deviation of the bandwidth for each benchmark and each number of threads
copy_mean = np.mean(copy_data, axis=1)
copy_std = np.std(copy_data, axis=1)

import matplotlib.pyplot as plt

plt.title("STREAM benchmark")
plt.plot(threads, copy_mean, label="" + " kernel")
plt.errorbar(threads, copy_mean, yerr=copy_std, fmt="o", label="Standard Deviation")
plt.ylabel("Bandwidth (MB/s)")
plt.xlabel("")
plt.legend(loc="best")
plt.savefig("test" + ".png")