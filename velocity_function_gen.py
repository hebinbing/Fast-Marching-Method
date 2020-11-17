#! /usr/bin/env python3

import h5py as h5
import numpy as np
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--npts', type=int, default=201, help='Number of grid points per dimension')
parser.add_argument('--target', type=float, nargs=2, default=[0.0, 0.0], help='Target coordinates')
args = parser.parse_args()

#Creates or overwrite the data file
file = h5.File('data/velocity_data.h5','w')

data = np.ones((args.npts,args.npts))

#Coordinates transformation
i = int(np.floor((args.npts-1)*(1+args.target[0])/2.0)) 
j = int(np.floor((args.npts-1)*(1+args.target[1])/2.0))

#Target velocity set to negative
data[i, j] = -1.0

# [TO DO] - Pass data set name as argument of C++ HDF5 read function
dataset = file.create_dataset('velocities', shape=data.shape, dtype=data.dtype, data=data)
dataset.attrs['Units'] = 'Meters per second'

file.close()

# TO DO: subprocess.run to run solver and continue in this script the trajectory interpretation