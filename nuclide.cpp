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

    this->can_fissioable = false;

    this->reaction_num = -1;
    this->reaction_mts = new std::vector<int>();
    this->Q_values = new std::vector<double>();
    this->thresholds = new std::vector<int>();

    this->reaction_cs_values = new std::vector<std::vector<double> *>();
    this->angle_values = new std::vector<std::vector<double> *>();

    this->macro_length = -1;
    this->energy = new std::vector<double>();
    //total cross section
    this->total = new std::vector<double>();
    //elastic scattering
    this->elastic = new std::vector<double>();
    //fission
    this->fission = new std::vector<double>();
    //neutron production
    this->nu_fission = new std::vector<double>();
    //absorption (MT > 100)
    this->absorption = new std::vector<double>();
    //heating
    this->heating = new std::vector<double>();

}
