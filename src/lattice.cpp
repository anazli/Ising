#include "../include/lattice.h"

using std::vector; using  std::cout;
using std::endl; using std::ofstream;
using std::string;

std::mt19937 gener(1943);

void Lattice::initialize(const int& state)
{
    std::uniform_real_distribution<double> rand;
    if(state == 0)
    {
        for(int i = 0 ; i < m_rows ; ++i)
        {
            vector<int> v;
            for(int j = 0 ; j < m_columns ; ++j)
            {
                if(rand(gener) < 0.5)
                {
                    v.push_back(1);
                }
                else
                {
                    v.push_back(-1);
                }
            }
            m_lat.push_back(v);
        }
    }
    else 
    {
        for(int i = 0 ; i < m_rows ; ++i)
        {
            vector<int> v;
            for(int j = 0 ; j < m_columns ; ++j)
            {
                v.push_back(state);
            }
            m_lat.push_back(v);
        }
    }
}

void Lattice::spinsState()const
{
    int up = 0;
    int down = 0;
    for(int i = 0 ; i < rows() ; ++i)
    {
        for(int j = 0 ; j < columns() ; ++j)
        {
            if(m_lat[i][j] == 1)
                up++;
        }
    }

    cout << "Up:" << up << endl;
    cout << "Down:" << rows()*columns() - up << endl;
}

void Lattice::configuration(ofstream& out, string file)
{
    out.open(file);
    for(int i = 0 ; i < rows() ; ++i)
    {
        for(int j = 0 ; j < columns() ; ++j)
        {
            out << m_lat[i][j] << " ";
        }
        out << "\n";
    }
    out.close();
}
