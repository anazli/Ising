#include<iostream>
#include "../include/simulation.h"

using namespace std;


int main()
{

    Lattice lat(20, 20);
    lat.initialize();

    int N = lat.size();
 
    Simulation S(100000000, 2.4, 1.);
    S.thermalization(lat.rows(), lat.columns());
	int runs = S.steps();
    ofstream out, en_out, mag_out, visual;
    en_out.open("./data/energy.dat");
    mag_out.open("./data/magnetization.dat");
    visual.open("./data/indices.dat");
    visual << lat.rows() << endl;

    double nrg = energy(lat, S.inter_param());
    double mgnt = magnetization(lat);

    lat.spinsState();
    lat.configuration(out, "./data/init_state.dat");

	int uncorr = N * 50;
	
    for(int t = 0 ; t < runs ; ++t)
    {
        lat = S.metropolis(lat, nrg, mgnt, visual);

	    if( (t % uncorr) == 0 )
	    {
        	en_out << nrg/N << endl;
        	mag_out << mgnt/N << endl;
	    }
    }

    lat.spinsState();
    lat.configuration(out, "./data/final_state.dat");

    cout << "Steps:" << S.steps() << endl;
    cout << "Accepted:" << S.accpt() << endl;

    out.close();
    en_out.close();
    mag_out.close();
    visual.close();

    return 0;
}
