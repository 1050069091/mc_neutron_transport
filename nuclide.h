#ifndef NUCLIDE_H
#define NUCLIDE_H

#include <string>
#include <vector>

class nuclide
{
public:
    nuclide();

    // name of nuclide, e.g. 92235.03c
    std::string name;
    //Z and A identifier, e.g. 92235
    int zaid;
    //Atomic Weight Ratio
    double awr;
    //temperature in MeV (k*T)
    double kT;
    //map xslistings中的key值
    std::string key_in_xsmap;


    //宏观截面信息
    //total cross section
//    std::vector<double> *total;
//    //elastic scattering
//    std::vector<double> *elastic;
//    //fission
//    std::vector<double> *fission;
//    //neutron production
//    std::vector<double> *nu_fission;
//    //absorption (MT > 100)
//    std::vector<double> *absorption;
//    //heating
//    std::vector<double> *heating;

};

#endif // NUCLIDE_H
