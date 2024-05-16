import matplotlib.pyplot as plt

L = 10

# Define the file path
file_path = 'data.txt'

# Initialize empty lists for each column
x = []
y = []
dx = []
dy = []

# Open the file and read its contents
with open(file_path, 'r') as file:
    for line in file:
        # Split the line into columns
        columns = line.split()
        # Append each column value to its respective list
        x.append(float(columns[0]))
        y.append(float(columns[1]))
        dx.append(float(columns[2]))
        dy.append(float(columns[3]))

# Print the lists to verify
print("x:", x)
print("y:", y)
print("dx:", dx)
print("dy:", dy)

"""
# Prep figure
fig = plt.figure(figsize=(4,4), dpi=80)
ax = plt.gca()

# plot in real time
plt.cla()
plt.quiver(x,y,vx,vy)
ax.set(xlim=(0, L), ylim=(0, L))
ax.set_aspect('equal')	
ax.get_xaxis().set_visible(False)
ax.get_yaxis().set_visible(False)
plt.pause(0.001)
				
# Save figure
plt.savefig('activematter.png',dpi=240)
plt.show()
"""
