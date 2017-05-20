#include "particle.h"
#include "global.h"

particle::particle()
{
    this->init_info();
}


void particle::init_info()
{
    //init engery
//    this->engery = global::gsource->min_engery +
//            ((rand()%global::gsource->particle_num) / double(global::gsource->particle_num))
//            *(global::gsource->max_engery-global::gsource->min_engery);
    this->engery = global::gsource->min_engery +
            ((rand()%RAND_MAX) / double(RAND_MAX))
            *(global::gsource->max_engery-global::gsource->min_engery);

    this->xyz[0] = global::gsource->site_xyz[0];
    this->xyz[1] = global::gsource->site_xyz[1];
    this->xyz[2] = global::gsource->site_xyz[2];

    this->random_init_particle_direction();


    this->now_mat_total_cs = 0.0;
    this->is_alive = true;
    this->is_absorption = false;
    this->is_out = false;

//    this->now_totals_on_nuclides = new std::vector<double>;
//    this->now_totals_on_nuclides = new std::vector<double>;
//    for(int i=0;i<global::gmaterials->nuclides->size();i++){

//    }


}

void particle::random_init_particle_direction(){
    double xmax = global::gmaterials->geometry_x;
    double ymax = global::gmaterials->geometry_y;
    double zmax = global::gmaterials->geometry_z;

    this->uvw[0] = xmax * ((rand()%RAND_MAX) / double(RAND_MAX) - 0.5);
    this->uvw[1] = ymax * ((rand()%RAND_MAX) / double(RAND_MAX) - 0.5);
    this->uvw[2] = zmax * ((rand()%RAND_MAX) / double(RAND_MAX) - 0.5);

    double length = sqrt(this->uvw[0] * this->uvw[0] + this->uvw[1] * this->uvw[1] + this->uvw[2] * this->uvw[2]);

    this->uvw[0] = this->uvw[0] / length;
    this->uvw[1] = this->uvw[1] / length;
    this->uvw[2] = this->uvw[2] / length;
}
