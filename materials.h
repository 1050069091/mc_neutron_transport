#ifndef MATERIALS_H
#define MATERIALS_H

#include "nuclide.h"
#include <map>
#include <string>

using namespace std;
class materials
{
public:
    materials();
    static map<string,nuclide> *nuclides;
    static double geometry_x,geometry_y,geometry_z;
    static double density;

    static std::vector<double> *atom_density;//mat_total
    static std::vector<double> *mat_total;
//    static string xs_name;
};

#endif // MATERIALS_H
