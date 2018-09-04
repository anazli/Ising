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
    double temp()const{return temperature;}
    double pdf(const double& x){return exp(-x);}
    void thermalization(const int&, const int&);
    void frequency(const int&, const int&);
    double energy(const Lattice&);
    double spinEnergy(const Lattice&, const int&, const int&);
    double magnetization(const Lattice&);
    Lattice metropolis(const Lattice&, double&, double&, std::ofstream&);

private:

    int Nsteps;
    int accepted;
    double temperature;
    double J;

};

#endif // SIMULATION_H
