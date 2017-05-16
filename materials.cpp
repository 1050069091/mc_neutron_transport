#include "materials.h"

materials::materials(){

}

std::map<std::string,nuclide> *materials::nuclides = new std::map<std::string,nuclide>;
double materials::geometry_x = -1.0,materials::geometry_y = -1.0,materials::geometry_z = -1.0;
double materials::density = -1.0;
//string materials::xs_name = "";
