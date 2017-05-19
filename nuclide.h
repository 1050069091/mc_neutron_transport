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

    //反应信息

    int reaction_num;
    std::vector<int> *reaction_mts;
    std::vector<double> *Q_values;

    std::vector<int> *thresholds;
    std::vector<std::vector<double> *> *reaction_cs_values;

    std::vector<int> *second_particle_num_dis;

    std::vector<std::vector<double> *> *angle_values;
    std::vector<std::vector<double> *> *engery_values;


    bool can_fissioable;

    //宏观截面信息
    int macro_length;

    std::vector<double> *energy;
    //total cross section
    std::vector<double> *total;
    //elastic scattering
    std::vector<double> *elastic;
    //fission
    std::vector<double> *fission;
    //neutron production
    std::vector<double> *nu_fission;
    //absorption (MT > 100)
    std::vector<double> *absorption;
    //heating
    std::vector<double> *heating;

};

#endif // NUCLIDE_H
