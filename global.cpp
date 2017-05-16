#include "global.h"

global::global(){

}

map<string,xslisting> *global::xsliistings = new map<string,xslisting>;
materials *global::gmaterials = new materials();
//map<string,nuclide> *global::nuclides = new mapmap<string,nuclide>;

const int global::ASCII = 0,global::BINARY = 1;
const int global::ACE_NEUTRON = 0,global::ACE_THERMAL=1;
