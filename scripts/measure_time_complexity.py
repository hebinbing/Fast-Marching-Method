#!/usr/bin/env python3

import h5py as h5
import numpy as np
import math
import subprocess
import time
import matplotlib.pyplot as plt

# Elapsed time lists - measured vs theoretical
measured_time = []
expected_time = []

# Dataset inputs
npts = [21, 101, 201, 301, 401, 501, 701]
target = [0, 0]

for k in range(len(npts)) :
    print('Subprocess iteration', k, 'with grid total number of points =', npts[k])

    #Creates or overwrite the data file
    file = h5.File('../data/velocity_data.h5','w')

    data = np.ones((npts[k],npts[k]))

    #Coordinates transformation
    i = int(np.floor((npts[k]-1)*(1+target[0])/2.0)) 
    j = int(np.floor((npts[k]-1)*(1+target[1])/2.0))

    data[i, j] = -1.0

    dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
    dataset.attrs['Units'] = 'Meters per second'

    file.close()

    start = time.time()
    subprocess.run('../build/examples/main_example')
    end = time.time()

    measured_time.append(end-start)
    expected_time.append(npts[k]*np.log(npts[k]))

print('Measured time')
print(measured_time)
print('Expected time')
print(expected_time)

fig, ax1 = plt.subplots()

color = 'tab:red'
ax1.set_xlabel('Grid Points')
ax1.set_ylabel('Thoeritcal constant : O(Nlog(N))', color=color)
ax1.plot(npts, expected_time, color=color)
ax1.tick_params(axis='y', labelcolor=color)

ax2 = ax1.twinx() 

color = 'tab:blue'
ax2.set_ylabel('Measured Time (s)', color=color) 
ax2.plot(npts, measured_time, color=color)
ax2.tick_params(axis='y', labelcolor=color)

fig.tight_layout() 
plt.show()