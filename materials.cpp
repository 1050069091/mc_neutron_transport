#include "materials.h"

materials::materials(){

}

std::map<std::string,nuclide> *materials::nuclides = new std::map<std::string,nuclide>;
double materials::geometry_x = -1.0,materials::geometry_y = -1.0,materials::geometry_z = -1.0;
double materials::density = -1.0;

std::vector<double> *materials::atom_density = new std::vector<double>;

std::vector<double> *materials::mat_total = new std::vector<double>;

//string materials::xs_name = "";
