#ifndef FISSION_SECOND_PARTICLE_H
#define FISSION_SECOND_PARTICLE_H

#include <vector>

class fission_second_particle
{
public:
    fission_second_particle();

    int flag;

    int coefficients_length;
    std::vector<double> *coefficients;

    int engery_length;
    std::vector<double> *engery_grid;
    std::vector<int> *second_particle_nums;


};

#endif // FISSION_SECOND_PARTICLE_H
