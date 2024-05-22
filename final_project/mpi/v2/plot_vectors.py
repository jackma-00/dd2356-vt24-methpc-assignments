import matplotlib.pyplot as plt
import numpy as np

L = 10

# Define the file path
file_path = 'data.txt'

# Initialize empty lists for each column
x = []
y = []
vx = []
vy = []


# Open the file and read its contents
with open(file_path, 'r') as file:
    for line in file:
        # Split the line into columns
        columns = line.split()
        # Append each column value to its respective list
        x.append(float(columns[0]))
        y.append(float(columns[1]))
        vx.append(float(columns[2]))
        vy.append(float(columns[3]))

# Print the lists to verify
#print("x:", x)
#print("y:", y)
#print("vx:", vx)
#print("vy:", vy)


# Prep figure
fig = plt.figure(figsize=(4,4), dpi=80)
ax = plt.gca()


x=np.array(x)
y=np.array(y)
vx=np.array(vx)
vy=np.array(vy)

# plot in real time
plt.cla()
plt.quiver(x,y,vx,vy)
ax.set(xlim=(0, L), ylim=(0, L))
ax.set_aspect('equal')	
ax.get_xaxis().set_visible(False)
ax.get_yaxis().set_visible(False)
#plt.pause(0.001)
				
# Save figure
plt.savefig('activematterpy.png',dpi=240)
plt.show()

