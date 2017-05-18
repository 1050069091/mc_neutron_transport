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
            ((rand()%global::gsource->particle_num) / double(global::gsource->particle_num))
            *(global::gsource->max_engery-global::gsource->min_engery);

    this->xyz[0] = global::gsource->site_xyz[0];
    this->xyz[1] = global::gsource->site_xyz[1];
    this->xyz[2] = global::gsource->site_xyz[2];
}
