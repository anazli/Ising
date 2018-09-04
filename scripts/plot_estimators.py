#!/usr/bin/env python
# A simple script that plots some quantities 
# such as energy, magnetization, aggregates,
# diameters and the configuration of the particles.

from matplotlib import pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from sys import argv
import numpy as np
import pylab as P 
import matplotlib
matplotlib.rcParams.update({'font.size': 15})

script, string = argv

#plt.ticklabel_format(style='sci', axis='x', scilimits=(0,0))

def plot_energy():
    data = np.loadtxt('../data/energy.dat')

    plt.style.use('dark_background')
    plt.figure()

    plt.plot(data,'b-', label=r'$T=2.0$')
    plt.xlabel(r'$MC steps$')
    plt.ylabel(r'$\frac{Energy}{N}$')
    plt.legend(loc=1)
    #plt.savefig('plotEnergy14.svg')
    plt.show()

def plot_magnetization():
    data = np.loadtxt('../data/magnetization.dat')
    plt.style.use('dark_background')
    plt.figure()
    plt.plot(data,'b-', label=r'$T=2.0$')
    plt.xlabel(r'$MC steps$')
    plt.ylabel(r'$\frac{Magnetization}{N}$')
    plt.legend(loc=1)
    #plt.savefig('plotMag24.svg')
    plt.show()

def plot_all():
    data = np.loadtxt('../data/energy.dat')
    data1 = np.loadtxt('../data/magnetization.dat')

    plt.style.use('dark_background')
    plt.figure(figsize=(13,8))
    plt.subplot(121)
    plt.plot(data,'b-', label=r'$T=2.0$')
    plt.xlabel(r'$MC steps$')
    plt.ylabel(r'$\frac{Energy}{N}$')
    plt.legend(loc=1)

    plt.subplot(122)
    plt.plot(data1,'b-', label=r'$T=2.0$')
    plt.xlabel(r'$MC steps$')
    plt.ylabel(r'$\frac{Magnetization}{N}$')
    plt.legend(loc=1)
    #plt.savefig('EnMag24.svg')
    plt.show()
   
    
    

if string == "energy" or string == "e":
    
    plot_energy()

elif string == "magnetization" or string == "m":

    plot_magnetization()

elif string == "all" or string == "a":

    plot_all()

else:
 
    print ("????........................")
    print ("What do you want me to plot?")


