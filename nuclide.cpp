#include "nuclide.h"

nuclide::nuclide()
{
    this->name = "NULL";
    //Z and A identifier, e.g. 92235
    this->zaid = -1.0;
    //Atomic Weight Ratio
    this->awr = -1.0;
    //temperature in MeV (k*T)
    this->kT = -1.0;
    //map xslistings中的key值
    this->key_in_xsmap = "NULL";
}
