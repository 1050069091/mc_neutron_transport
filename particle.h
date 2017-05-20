#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>

class particle
{
public:
    particle(bool is_init);
    double xyz[3];
    double uvw[3];
    double engery;
    bool is_alive;
    bool is_absorption;
    bool is_fission;
    bool is_nu_fisson;
//    bool is_
    bool is_out;

    double now_mat_total_cs;
//    std::vector<double> *now_totals_on_nuclides;
//    std::vector<double> *now_c_totals_on_nuclides;
private:
    void init_info(bool is_init);
    void random_init_particle_direction();
};

#endif // PARTICLE_H
