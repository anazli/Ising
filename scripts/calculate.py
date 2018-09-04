#!/usr/bin/env python3
#####################################################################
# Arguments:
# 1. the size of the lattice (for cv and x)
# 2. the temperature (again for cv and x)
# 3. the quantity we want to calculate its estimator
# 4. the steps until equilibrium (to throw out the initial states)
#####################################################################


import numpy as np
from sys import argv

script, size, temperature, quantity, equilibrium = argv

energy_data = np.loadtxt('../data/energy.dat')
magnet_data = np.loadtxt('../data/magnetization.dat')

beta = 1./float(temperature)
size = int(size)
equilibrium = int(equilibrium)

energy_data = energy_data[equilibrium:]
magnet_data = magnet_data[equilibrium:]

def jackknife(data):
	"""
	Implements the jackknife method for energy
	and magnetization.
	"""
	N = len(data)
	mu = np.mean(data)
	mui = np.zeros(N)
	for i in range(N):
		temp = np.delete(data, i)
		mui[i] = np.mean(temp)
	
	dif = mui - mu
	error = np.sqrt((N-1) * np.sum(dif**2)/N )
	return error

def jackknife_cv(data):
	"""
	Implements the jackknife method for cv.
	"""
	N = len(data)
	data2 = data**2
	q = size * (beta**2)*(np.mean(data2) - np.mean(data)**2)
	qi = np.zeros(N)
	for i in range(N):
		temp = np.delete(data, i)
		temp2 = temp**2
		qi[i] = size * (beta**2)*(np.mean(temp2) - np.mean(temp)**2)
	
	dif = qi - q
	error = np.sqrt((N-1) * np.sum(dif**2)/N )
	return error	

def jackknife_x(data):
	"""
	Implements the jackknife method for x.
	"""
	N = len(data)
	data2 = data**2
	q = size * beta*(np.mean(data2) - np.mean(data)**2)
	qi = np.zeros(N)
	for i in range(N):
		temp = np.delete(data, i)
		temp2 = temp**2
		qi[i] = size * beta*(np.mean(temp2) - np.mean(temp)**2)
	
	dif = qi - q
	error = np.sqrt((N-1) * np.sum(dif**2)/N )
	return error	

def bootstrap(data):
	"""
	Implements the bootstrap method for energy
	and magnetization.
	"""
	B = 10000
	N = len(data)
	mu = np.zeros(B)
	for i in range(B):
		index = np.random.choice(N, N)
		temp = data[index]
		mu[i] = np.mean(temp)
		
	mu_b = np.mean(mu)
	dif = mu - mu_b
	se = np.sqrt( ( 1. / (B-1.) ) * np.sum(dif**2) )
	return se
		
def bootstrap_cv(data):
	"""
	Implements the bootstrap method for cv.
	"""
	B = 10000
	N = len(data)
	q = np.zeros(B)
	for i in range(B):
		index = np.random.choice(N, N)
		temp = data[index]
		temp2 = temp**2
		q[i] = size * (beta**2)*(np.mean(temp2) - np.mean(temp)**2)
		
	mu_b = np.mean(q)
	dif = q - mu_b
	se = np.sqrt( ( 1. / (B-1.) ) * np.sum(dif**2) )
	return se
	
def bootstrap_x(data):
	"""
	Implements the bootstrap method for x.
	"""
	B = 10000
	N = len(data)
	q = np.zeros(B)
	for i in range(B):
		index = np.random.choice(N, N)
		temp = data[index]
		temp2 = temp**2
		q[i] = size * beta*(np.mean(temp2) - np.mean(temp)**2)
		
	mu_b = np.mean(q)
	dif = q - mu_b
	se = np.sqrt( ( 1. / (B-1.) ) * np.sum(dif**2) )
	return se

def magn():
    res = np.mean(magnet_data)
    error = jackknife(magnet_data)
    print("Mean value:", res)
    print("Std Error:", error)


def energy():
    res = np.mean(energy_data)
    error = jackknife(energy_data)
    print("Mean value:", res)
    print("Std Error:", error)


def specific_heat():
    en = energy_data  
    en2 = en**2
    c = size * (beta**2)*(np.mean(en2) - np.mean(en)**2)
    error = jackknife_cv(energy_data)
    print("Specific heat:", c)
    print("Std Error:", error)

def susceptibility():
    mag = magnet_data 
    mag2 = mag**2
    x = size * beta * (np.mean(mag2) - np.mean(mag)**2)
    error = jackknife_x(magnet_data)
    print("Magnetic susceptibility:", x)
    print("Std Error:", error)


if quantity == 'm' or quantity == 'magnetization':
    magn()
elif quantity == 'e' or quantity == 'energy':
    energy()
elif quantity == 'c' or quantity == 'specific':
    specific_heat()
elif quantity == 'x' or quantity == 'susceptibility':
    susceptibility()
