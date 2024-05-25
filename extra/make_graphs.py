import matplotlib.pyplot as plt

n = [1000, 10000, 100000, 1000000]

t_normal = [8, 84, 854, 8500]

t_optimized = [9, 96, 1100, 10500]

# Plot the data
plt.plot(n, t_normal, label='Normal')
plt.plot(n, t_optimized, label='Parallelized')

# Add a title
plt.title('Normal vs Parallelized')

# Add X and Y Labels
plt.xlabel('N')
plt.ylabel('Time (s)')

# Add a Legend
plt.legend()

plt.show()