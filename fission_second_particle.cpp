#include "fission_second_particle.h"

fission_second_particle::fission_second_particle()
{

    this->flag = 0;

    this->coefficients_length = -1;
    this->coefficients = new std::vector<double>;

    this->engery_length = -1;
    this->engery_grid = new std::vector<double>;
    this->second_particle_nums = new std::vector<int>;

}
