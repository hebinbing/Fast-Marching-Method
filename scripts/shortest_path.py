#!/usr/bin/env python3

import h5py as h5
import numpy as np
from scipy.interpolate import RegularGridInterpolator
import matplotlib.pyplot as plt
import math
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--error_tol', type=float, default=0.1, help='Finish condition')
parser.add_argument('--start', type=float, nargs=2, default=[-1, 1], help='Start coordinates in a domain with [-1,-1]x[1,1] as vertices')
parser.add_argument('--step', type=float, default=0.0001, help='Discretization step')
args = parser.parse_args()

targets = np.empty(0, dtype = float)
start = np.array([args.start[0], args.start[1]])

# Read Cost and Value Function

file = h5.File('../data/velocity_data.h5', "r")
velocities = file['velocities'][()]
file.close()

file = h5.File('../data/value_function.h5', "r")
value_function = file['value_function'][()]
file.close()

npts = value_function.shape[0]
grid_size = 2 / (npts - 1)

for i in range(npts):
    for j in range(npts):
        if value_function[i][j] == 0.0:
            targets = np.append(targets, [(2 * i) / (npts - 1) - 1, (2 * j) / (npts - 1) - 1])

# Gradient descent
gradient = np.gradient(value_function, grid_size, edge_order=2)

t = np.linspace(-1, 1, npts)
gt = [0, 1]
grad_interpol = RegularGridInterpolator((gt, t, t), gradient, method='linear', bounds_error=False, fill_value=None)
vel_interpol = RegularGridInterpolator((t, t), velocities, method='linear', bounds_error=False, fill_value=None)

trajectory = np.array([start], dtype=float)
k = 0

while True:
    grad = grad_interpol([[0, trajectory[k][0], trajectory[k][1]], [1, trajectory[k][0], trajectory[k][1]]])
    abs_grad = math.sqrt(grad[0]**2 + grad[1]**2)

    if math.isnan(grad[0]) | math.isnan(grad[1]) | (abs_grad == 0):
        print('ERROR - Gradient is not defined in point:', trajectory[k])
        quit()

    curr_i = trajectory[k][0] - args.step * grad[0] / abs_grad
    curr_j = trajectory[k][1] - args.step * grad[1] / abs_grad
    
    trajectory = np.append(trajectory, [[curr_i, curr_j]], axis=0)
        
    k = k + 1

    print('Discovering trajectory... | Iteration', k, '| Current position', trajectory[k])

    if vel_interpol(trajectory[k]) < 0.00005:
        break

    if (trajectory[k][0] < -1) | (trajectory[k][0] > 1) | (trajectory[k][1] < -1) | (trajectory[k][1] > 1):
        print('ERROR - Trajectory surpassed boundaries')
        quit()

# Graphics

x, y = np.meshgrid(t, t, indexing='ij')

plt.pcolormesh(x, y, value_function, cmap='jet', shading='auto')
plt.colorbar()
plt.axis('equal')

plt.plot(trajectory[:, 0], trajectory[:, 1], color='red', linestyle='dashed', linewidth = 2, marker='.', markersize=5)

plt.show()
