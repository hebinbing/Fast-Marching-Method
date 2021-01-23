#!/usr/bin/env python3

import h5py as h5
import numpy as np
import math
import subprocess
import time
import matplotlib.pyplot as plt

# Dataset inputs
npts = np.array([501, 701, 1001, 1501, 2001, 3001, 5001])
target = [0, 0]

# Elapsed time lists - measured vs theoretical
measured_time = np.zeros(npts.size)
expected_time = np.zeros(npts.size)

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

    measured_time[k] = end - start
    expected_time[k] = npts[k] * np.log10(npts[k])

measured_time = measured_time/np.linalg.norm(measured_time)
expected_time = expected_time/np.linalg.norm(expected_time)

print('Measured time:')
print(measured_time)
print('Expected time:')
print(expected_time)

plt.plot(npts, measured_time, 'ro-', label = 'measured time')
plt.plot(npts, expected_time, 'bo-', label = 'expected_time')
plt.legend()
plt.show()