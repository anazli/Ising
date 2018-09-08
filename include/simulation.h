#ifndef SIMULATION_H
#define SIMULATION_H

#include<cmath>
#include<random>
#include<fstream>
#include "lattice.h"


class Simulation {


public:

    Simulation(const int& n, const double& t, const double& par):
        Nsteps{n},
        accepted{0},
        temperature{t},
        J{par}
    { }

    int steps()const{return Nsteps;}
    int accpt()const{return accepted;}
    double inter_param()const{return J;}
    double temp()const{return temperature;}
    void thermalization(const int&, const int&);
    void frequency(const int&, const int&);
    Lattice metropolis(const Lattice&, double&, double&, std::ofstream&);

private:

    int Nsteps;
    int accepted;
    double temperature;
    double J;

};


double pdf(const double&);
double energy(const Lattice&, const double&);
double spinEnergy(const Lattice&, const int&, const int&);
double magnetization(const Lattice&);

#endif // SIMULATION_H
