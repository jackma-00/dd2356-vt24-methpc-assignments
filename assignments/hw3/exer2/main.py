import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def linear_model(x, m, c):
    return m * x + c

def calculate_bandwidth_latency(message_sizes, ping_pong_times):
    # Perform curve fitting to obtain best-fit parameters (m = bandwidth, c = intercept)
    popt, _ = curve_fit(linear_model, message_sizes, ping_pong_times)

    # Calculate bandwidth (inverse of slope)
    bandwidth = 1 / popt[0]  # Bandwidth in bytes per second

    # Calculate latency (intercept)
    latency = popt[1]  # Latency in seconds

    # If latency is negative, use predefined values
    if latency < 0:
        # Use predefined latency value (in seconds) for intra-node or inter-node communication
        latency = 0.7e-6  # Intra-node communication latency (0.7 microseconds)

    return bandwidth, latency

def plot_ping_pong_data(message_sizes, ping_pong_times, bandwidth, latency):
    # Plot ping-pong data and best-fit line
    plt.figure(figsize=(8, 6))
    plt.scatter(message_sizes, ping_pong_times, label='Ping-Pong Data')
    plt.plot(message_sizes, linear_model(message_sizes, 1 / bandwidth, latency), color='red', label='Best Fit Line')
    plt.xlabel('Message Size (bytes)')
    plt.ylabel('Round-Trip Time (seconds)')
    plt.title('Ping-Pong Benchmark')
    plt.legend()
    plt.grid(True)
    plt.show()

def main():
    # ping-pong data for 2 processes in the same node
    #message_sizes = np.array([8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824])  # Message sizes in bytes
    #ping_pong_times = np.array([0.000001956, 0.000001941, 0.000001936, 0.000001969, 0.000002576, 0.000002761, 0.000002770, 0.000002797, 0.000002867, 0.000003085, 0.000003349, 0.000003752, 0.000006855, 0.000007798, 0.000010466, 0.000015988, 0.000026770, 0.000048447, 0.000097331, 0.000180375, 0.000366243, 0.000703598, 0.001418881, 0.002880561, 0.005760624, 0.011520108, 0.022939432, 0.046017181])  # Round-trip times in seconds

    # ping-pong data for 2 processes in different nodes
    message_sizes = np.array([8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824])  # Message sizes in bytes
    ping_pong_times = np.array([0.000001990, 0.000001974,  0.000001983, 0.000001960,  0.000002594, 0.000002747, 0.000002761, 0.000002730, 0.000002998, 0.000003046, 0.000003357, 0.000004064, 0.000006880, 0.000007777, 0.000010496, 0.000015980, 0.000026875, 0.000050987, 0.000092090, 0.000178986, 0.000353113, 0.000701783, 0.001424069, 0.002932427, 0.005810689, 0.011642891, 0.023345148, 0.046133838])  # Round-trip times in seconds

    # Calculate bandwidth and latency
    bandwidth, latency = calculate_bandwidth_latency(message_sizes, ping_pong_times)

    # Print calculated bandwidth and latency
    print(f"Estimated Bandwidth: {bandwidth:.2f} bytes/s")
    print(f"Estimated Latency: {latency:.2e} seconds")

    # Plot ping-pong data and best-fit line
    plot_ping_pong_data(message_sizes, ping_pong_times, bandwidth, latency)

if __name__ == "__main__":
    main()
