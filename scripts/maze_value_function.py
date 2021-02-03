#!/usr/bin/env python3

import h5py as h5
import numpy as np
import time
import subprocess
import matplotlib.pyplot as plt
import cv2

img = cv2.imread('../images/maze.jpg', cv2.IMREAD_IGNORE_ORIENTATION | cv2.IMREAD_GRAYSCALE)
ret, img = cv2.threshold(img, 127, 1, cv2.THRESH_BINARY)
data = np.array(img, dtype=float)

target = np.array([0.72, 0.81], dtype=float)

for i in range(data.shape[0]):
    for j in range(data.shape[1]):
        if data[i, j] == 0:
            data[i, j] = -0.0001

file = h5.File('../data/velocity_data.h5', 'w')

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

file = h5.File('../data/velocity_data.h5', 'w')

for i in range(data.shape[0]):
    for j in range(data.shape[1]):
        if data[i, j] == -0.0001:
            data[i, j] = 0

dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
dataset.attrs['Units'] = 'Meters per second'
file.close()

file = h5.File('../data/value_function.h5', "r")
val_function = file['value_function'][()]
file.close()

tx = np.linspace(-1, 1, val_function.shape[0])
ty = np.linspace(-1, 1, val_function.shape[1])
[x, y] = np.meshgrid(tx, ty, indexing='ij')

plt.pcolormesh(x, y, val_function, cmap='jet', shading='auto')
plt.colorbar()
plt.axis('equal')
plt.plot(target[0], target[1], 'ro', markersize=8)
plt.show()
