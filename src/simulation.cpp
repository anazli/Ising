#include "../include/simulation.h"
#include<iostream>

std::mt19937 gen(45494);
std::mt19937 gen_rows(2644534);
std::mt19937 gen_cols(1876132);


using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::ofstream;
using std::cout;
using std::endl;

double pdf(const double& x)
{
	return exp(-x);
}

void Simulation::thermalization(const int& r, const int& c)
{
    uniform_int_distribution<int> randRows(0, r-1), randCols(0, c-1);
    uniform_real_distribution<double> rand;
    for(int i = 0 ; i < 10000 ; ++i)
    {
	int r1 = randRows(gen_rows);
	int r2 = randCols(gen_cols);
	double r3 = rand(gen);
    }
}

void Simulation::frequency(const int& r, const int& c)
{
    uniform_int_distribution<int> randRows(0, r-1), randCols(0, c-1);
    uniform_real_distribution<double> rand;
    for(int i = 0 ; i < 50 ; ++i)
    {
	int r1 = randRows(gen_rows);
	int r2 = randCols(gen_cols);
	double r3 = rand(gen);
    }
}


double energy(const Lattice& lat, const double& param)
{
    double energy = 0.;
    int r = lat.rows();
    int c = lat.columns();

    for(int i = 0 ; i < r ; ++i)
    {
        for(int j = 0 ; j < c ; ++j)
        {

            energy += spinEnergy(lat, i, j);
        }
    }

    return -param * energy;
}

double spinEnergy(const Lattice& lat, const int& i, const int& j)
{
    double s = 0;
    int r = lat.rows();
    int c = lat.columns();

    for(int k = i-1 ; k < i+2 ; k += 2)
    {
        int ii = k;
        if(k < 0)
            ii += r;
        if(k >= r)
            ii -= r;

        s += lat.index(ii,j);
    }

    for(int k = j-1 ; k < j+2 ; k += 2)
    {
        int jj = k;
        if(k < 0)
            jj += c;
        if(k >= c)
            jj -= c;

        s += lat.index(i,jj);
    }

    return lat.index(i,j) * s;
}

double magnetization(const Lattice& lat)
{
    double m = 0.;
    int r = lat.rows();
    int c = lat.columns();

    for(int i = 0 ; i < r ; ++i)
        for(int j = 0 ; j < c ; ++j)
            m += lat.index(i,j);

    return m;
}

Lattice Simulation::metropolis(const Lattice& lat, double& energy, double& magnetization, ofstream& out)
{
    uniform_int_distribution<int> randRows(0, lat.rows()-1), randCols(0, lat.columns()-1);
    uniform_real_distribution<double> rand;
    Lattice temp = lat;
    double beta = 1./temperature;

    int i = randRows(gen_rows);
    int j = randCols(gen_cols);

    int spin_index{0};

    double dE = 2. * J * spinEnergy(temp, i, j);

    if(dE <= 0.)
    {
        temp.index(i,j) *= -1;
        accepted++;
        energy += dE;
        magnetization += 2. * temp.index(i, j);
	    spin_index = lat.rows() * i + j; //no 10*i because when rows < 10 gives seg fault
        out << spin_index << endl;
        return temp;
    }
    else if(pdf(dE * beta) > rand(gen))
    {
        temp.index(i, j) *= -1;
        accepted++;
        energy += dE;
        magnetization += 2. * temp.index(i, j);
        spin_index = lat.rows() * i + j;
        out << spin_index << endl;
        return temp;
    }
    else
    {
        return lat;
    }

}
