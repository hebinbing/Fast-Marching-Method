#!/usr/bin/env python3

import h5py as h5
import numpy as np
import subprocess
import time
import matplotlib.pyplot as plt

# Dataset inputs
npts = np.array([601, 801, 1001, 1101, 1301, 1501, 1801])
target = [0, 0]

# Elapsed time lists - measured vs theoretical
measured_time = np.zeros(npts.size)
expected_time = np.zeros(npts.size)

for k in range(len(npts)):
    print('Subprocess iteration', k, 'with grid size =', npts[k])

    # Creates or overwrite the data file
    file = h5.File('../data/velocity_data.h5', 'w')

    data = np.ones((npts[k], npts[k]))

    # Coordinates transformation
    i = int(np.floor((npts[k]-1)*(1+target[0])/2.0)) 
    j = int(np.floor((npts[k]-1)*(1+target[1])/2.0))

    data[i, j] = -1.0

    dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
    dataset.attrs['Units'] = 'Meters per second'

    file.close()

    subprocess.run('../build/examples/main_example')

    f = open('../data/last_measured_time.txt', 'r')
    measured_time[k] = float(f.read())
    f.close()

    expected_time[k] = npts[k]**2 * np.log(npts[k]**2)

measured_time = measured_time/measured_time[0]
expected_time = expected_time/expected_time[0]

print('Measured time:')
print(measured_time)
print('Expected time:')
print(expected_time)

plt.plot(npts, measured_time, 'ro-', label='measured time')
plt.plot(npts, expected_time, 'bo-', label='expected_time')
plt.legend()
plt.show()
