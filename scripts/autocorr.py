#!/usr/bin/env python
# A simple script to calculate the autocorrelation function of some quantities
# data is the file that contains the samples
# equilibrium is the time when the system reaches equilibrium states

from sys import argv
import numpy as np
from matplotlib import pyplot as plt
import matplotlib
matplotlib.rcParams.update({'font.size': 13})


script, data, equilibrium = argv

f = np.loadtxt(data)
equilibrium = int(equilibrium)
f = f[equilibrium:]
f -= np.mean(f)

autocorr = np.correlate(f, f, mode='full')
normautocorr = autocorr[autocorr.size/2:]/autocorr[autocorr.size/2]

plt.style.use('dark_background')
plt.figure(figsize=(15, 9))
plt.subplot(121)
plt.plot(normautocorr, 'b-')
plt.ylabel('Autocorrelation')
plt.xlabel('Time (MC steps)')
plt.subplot(122)
plt.plot(normautocorr[:20], 'b-')
plt.ylabel('Autocorrelation')
plt.xlabel('Time (MC steps)')
#plt.savefig('../figures/autocorrLastSim.svg')
plt.show()
