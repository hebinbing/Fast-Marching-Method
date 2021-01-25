#!/usr/bin/env python3

import h5py as h5
import numpy as np
import math
import time
import subprocess
import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('--npts', type=int, default=201, help='Number of grid points per dimension')
parser.add_argument('--targets', type=float, nargs='+', default=[0.0, 0.0], help='Targets coordinates in a domain with [-1,1]² as vertices; Any number incorrectly inserted will be discarded')
parser.add_argument('--no_plot', action='store_true', help='Don\'t plot the result.')
args = parser.parse_args()

targets = np.array(args.targets, dtype=float)

#Creates or overwrite the data file
file = h5.File('../data/velocity_data.h5','w')

# data = np.ones((args.npts,args.npts))
data = np.random.randint(1, 100, size=(args.npts, args.npts))

k = 0
while k < int(targets.size):
    print(targets[k], targets[k+1])
    print(k)
    i = int(np.floor((args.npts-1)*(1+args.targets[k])/2.0)) 
    j = int(np.floor((args.npts-1)*(1+args.targets[k + 1])/2.0))    
    data[i, j] = -1
    print('data in ', i, j, ':', data[i,j])
    k = k + 2

dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
dataset.attrs['Units'] = 'Meters per second'

file.close()

start = time.time()
subprocess.run('../build/examples/main_example')
end = time.time()

file = h5.File('../data/value_function.h5', "r")
data = file['value_function'][()]
file.close()

t = np.linspace(-1, 1, data.shape[0])
x, y = np.meshgrid(t, t, indexing='ij')
z = np.sqrt((x - args.target[0])**2 + (y - args.target[1])**2)

data[i, j] = z[i,j] = 1
rel_error = np.abs(data - z) / z * 100
rel_error[i, j] = data[i, j] = z[i,j] = 0

print('Elapsed time:', end-start)
print('Maximum absolute error:', np.amax(np.abs(z - data)))
print('Maximum relative error:', np.amax(np.abs(rel_error)), '%')

if not args.no_plot:
    plt.pcolormesh(x, y, data, cmap='jet', shading='auto')
    plt.colorbar()
    plt.contour(x, y, data, levels=15, linestyles='dashed', colors='k')
    plt.axis('equal')
    plt.show()