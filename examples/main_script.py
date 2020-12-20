#!/usr/bin/env python3

import h5py as h5
import numpy as np
import math
import subprocess
import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser()
parser.add_argument('--npts', type=int, default=201, help='Number of grid points per dimension')
parser.add_argument('--target', type=float, nargs=2, default=[0.0, 0.0], help='Target coordinates in a domain with [-1,-1]x[1,1] as vertices')
parser.add_argument('--no_plot', action='store_true', help='Don\'t plot the result.')
args = parser.parse_args()

#Creates or overwrite the data file
file = h5.File('../data/velocity_data.h5','w')

data = np.ones((args.npts,args.npts))

#Coordinates transformation
i = int(np.floor((args.npts-1)*(1+args.target[0])/2.0)) 
j = int(np.floor((args.npts-1)*(1+args.target[1])/2.0))

data[i, j] = -1.0

dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
dataset.attrs['Units'] = 'Meters per second'

file.close()

subprocess.run('../build/examples/main_example')

file = h5.File('../data/value_function.h5', "r")
data = file['value_function'][()]
file.close()

t = np.linspace(-1, 1, data.shape[0])
x, y = np.meshgrid(t, t, indexing='ij')
z = np.sqrt((x - args.target[0])**2 + (y - args.target[1])**2)

data[i, j] = 1
rel_error = np.abs(z - data) / data * 100
rel_error[i, j] = data[i, j] = 0

print('Maximum absolute error:', np.amax(np.abs(z - data)))
print('Maximum relative error:', np.amax(np.abs(rel_error)), '%')

if not args.no_plot:
    plt.pcolormesh(x, y, data, cmap='jet', shading='auto')
    plt.colorbar()
    plt.contour(x, y, data, levels=10, linestyles='dashed', colors='k')
    plt.axis('equal')
    plt.show()