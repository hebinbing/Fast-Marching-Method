#!/usr/bin/env python3

import h5py as h5
import numpy as np
import math
import time
import subprocess
import matplotlib.pyplot as plt
import cv2

img = cv2.imread('../images/maze.jpg', cv2.IMREAD_IGNORE_ORIENTATION | cv2.IMREAD_GRAYSCALE)

data = np.array(img, dtype=float)
data = data / data[0]

target = np.array([0.72, 0.81], dtype=float)

file = h5.File('../data/velocity_data.h5','w')

i = int(np.floor((data.shape[0]-1)*(1+target[0])/2.0))
j = int(np.floor((data.shape[1]-1)*(1+target[1])/2.0))
data[i, j] = -1

dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
dataset.attrs['Units'] = 'Meters per second'

file.close()

start = time.time()
subprocess.run('../build/examples/main_example')
end = time.time()

print('Elapsed time:', end-start)

file = h5.File('../data/value_function.h5', "r")
data = file['value_function'][()]
file.close()

tx = np.linspace(-1, 1, data.shape[0])
ty = np.linspace(-1, 1, data.shape[1])
[x, y] = np.meshgrid(tx, ty, indexing='ij')

plt.pcolormesh(x, y, data, cmap='jet', shading='auto')
plt.colorbar()
plt.axis('equal')
plt.plot(target[0], target[1], 'ro', markersize=8)
plt.show()