#!/usr/bin/env python3
######################################################
# Plot the measurements at different temperatures
# with the analytic solutions
######################################################
import numpy as np
from matplotlib import pyplot as plt
import scipy.integrate as si
import matplotlib
matplotlib.rcParams.update({'font.size': 15})

########################################################################
# Analytical solution
########################################################################
def f(x):
    return 2. * np.sinh(2. * x * J) / np.cosh(2. * x * J)**2

J = 1.
Tc = 2. * J / np.log(1. + np.sqrt(2.))
T_e = np.linspace(1., 3., 500)
T_m = np.linspace(1., Tc, 500)
beta_e = 1./T_e
beta_m = 1./T_m

E_anal = np.zeros(len(T_e))
dK = 4*J/np.cosh(2*beta_e*J)*(1-2*J*np.tanh(2*beta_e*J)**2)

for i in range(len(beta_e)):
    K = f(beta_e[i]) 
    iK = si.quad(lambda x: K * dK[i] * np.sin(x)**2 / ( np.sqrt(1. - K**2 * np.sin(x)**2)  + 1. - K**2 * np.sin(x)**2) , 0, np.pi)[0]
    E_anal[i] = -2. * J * np.tanh(2. * beta_e[i] * J) + iK/(2. * np.pi) 


M_anal = (1. - np.sinh(2. * beta_m * J)**(-4.))**(1./8.)
cv = (E_anal[2:] - E_anal[0:len(E_anal)-2])/(T_e[2]-T_e[0])
########################################################################
# Numerical solution
########################################################################

data = np.loadtxt('../data/thermquantLast.dat')
T_num = data[:,0] 
E_num = data[:,1]
M_num = data[:,2]
cv_num = data[:,3]
x_num = data[:,4]
# red vertical line at critical temperature
y1 = np.linspace(0, min(E_num), 1000)
y2 = np.linspace(0, max(M_num), 1000)
y3 = np.linspace(0, max(cv_num), 1000)
y4 = np.linspace(0, max(x_num), 1000)
xc = Tc * np.ones(len(y1))

# Include errors
error = np.loadtxt('../data/thermquantLast_error.dat')
e_error = error[:,1]
m_error = error[:,2]
c_error = error[:,3]
x_error = error[:,4]

plt.style.use('dark_background') 
plt.figure(figsize=(17,12))

# Plot energy with analytical solution
plt.subplot(221)
plt.errorbar(T_num, E_num, yerr=(e_error, e_error), linestyle='None',  color= 'w', marker='.')
plt.plot(T_e, E_anal, 'b-' )
plt.plot(xc, y1, 'r--')
plt.xlabel('Temperature (kT)')
plt.ylabel(r'$\frac{<E>}{N}$')

# Plot Magnetization with analytical solution
plt.subplot(222)
plt.errorbar(T_num, M_num, yerr=(m_error, m_error), linestyle='None', color='w', marker='.')
plt.plot(T_m, M_anal, 'b-')
plt.plot(xc, y2, 'r--')
plt.xlabel('Temperature (kT)')
plt.ylabel(r'$\frac{<M>}{N}$')

# Plot Specific heat with analytical solution
plt.subplot(223)
plt.errorbar(T_num, cv_num, yerr=(c_error, c_error), linestyle='None', color='w', marker='.')
plt.plot(T_e[1:len(T_e)-1], cv, 'b-')
plt.plot(xc, y3, 'r--')
plt.xlabel('Temperature (kT)')
plt.ylabel(r'$C_v$')

# Plot susceptibility
plt.subplot(224)
plt.errorbar(T_num, x_num, yerr=(x_error, x_error), linestyle='None', color='w', marker='.')
plt.plot(xc, y4, 'r--')
plt.xlabel('Temperature (kT)')
plt.ylabel(r'$\chi_{\mu}$')
plt.savefig('quantitiesVsT.svg')
plt.show()

