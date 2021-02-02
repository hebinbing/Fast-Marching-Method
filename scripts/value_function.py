#!/usr/bin/env python3

import h5py as h5
import numpy as np
import math
import time
import subprocess
import argparse
import matplotlib.pyplot as plt
import cv2

parser = argparse.ArgumentParser()
parser.add_argument('--npts', type=int, default=201, help='Number of grid points per dimension')
parser.add_argument('--targets', type=float, nargs='+', default=[0.0, 0.0], help='Targets coordinates in a domain ['
                                                                                 '-1,1]² as vertices; Any number '
                                                                                 'incorrectly inserted will be '
                                                                                 'discarded')
parser.add_argument('--obstacles', type=float, nargs='+', help='Square obstacles coordinates in a domain [-1,1]² as '
                                                               'vertices; Any number incorrectly inserted will be '
                                                               'discarded')
parser.add_argument('--obs_size', type=int, default=10, help='Square obstacles size')
parser.add_argument('--cost_function', type=str, default='uniform', help='Type of cost function: {uniform, random}')
parser.add_argument('--no_plot', action='store_true', help='Don\'t plot the result.')
args = parser.parse_args()

targets = np.array(args.targets, dtype=float)
obstacles = np.array(args.obstacles, dtype=float)

# Creates or overwrite the data file
file = h5.File('../data/velocity_data.h5','w')

if args.cost_function == 'uniform':
    data = np.ones((args.npts, args.npts))
elif args.cost_function == 'random':
    data = np.random.randint(1, 100, size=(args.npts, args.npts))
else :
    print('Type of const function not valid')
    file.close()
    quit()

k = 0
while k < obstacles.size & obstacles.size != 1:
    row = int(np.floor((args.npts-1)*(1+args.obstacles[k])/2.0)) 
    col = int(np.floor((args.npts-1)*(1+args.obstacles[k + 1])/2.0))    

    if max(int(row + args.obs_size/2), int(col + args.obs_size/2)) > args.npts :
        print('Obstacle too big - invalid inputs')
        quit()

    for i in range(args.obs_size) :
        for j in range(args.obs_size) :
            data[int(row - args.obs_size/2) + i][int(col - args.obs_size/2) + j] = -np.average(data)/10
    
    k = k + 2

k = 0
while k < targets.size:
    i = int(np.floor((args.npts-1)*(1+args.targets[k])/2.0)) 
    j = int(np.floor((args.npts-1)*(1+args.targets[k + 1])/2.0))    
    data[i, j] = -1
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

print('Elapsed time:', end-start)

if not args.no_plot:
    plt.pcolormesh(x, y, data, cmap='jet', shading='auto')
    plt.colorbar()
    plt.contour(x, y, data, levels=15, linestyles='dashed', colors='k')
    plt.axis('equal')
    plt.show()