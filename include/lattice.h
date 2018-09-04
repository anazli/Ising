#ifndef LATTICE_H
#define LATTICE_H

#include<iostream>
#include<fstream>
#include<random>
#include<vector>
#include<string>


class Lattice {

public:

    Lattice(const int& r, const int& c):m_rows{r}, m_columns{c}
    { }

    void initialize(const int& state = 0);
    int rows()const{return m_rows;}
    int columns()const{return m_columns;}
    int size()const{return m_rows * m_columns;}

    int index(const int& i, const int& j)const{return m_lat[i][j];}
    int& index(const int& i, const int& j){return m_lat[i][j];}

    void spinsState()const;
    void configuration(std::ofstream&, std::string);

private:

    int m_rows;
    int m_columns;
    std::vector< std::vector<int> > m_lat;

};

#endif // LATTICE_H
