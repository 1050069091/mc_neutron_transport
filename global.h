#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include "xslisting.h"
#include <string>
#include <vector>
#include "materials.h"
#include "nuclide.h"
#include "source.h"
#include "particle.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;

class global
{
public:
    global();

    static map<string,xslisting> *xsliistings;
    static materials *gmaterials;
    static source *gsource;
    static vector<particle> *gparticles;

    static const int BINARY,ASCII,ACE_NEUTRON,ACE_THERMAL;
    static const double N_AVOGADRO,MASS_NEUTRON;
    static const double PI;

    static int now_particle_num;
    static int last_particle_num;

//    static int max_alive_particle_index;

    static int absorption_num;
    static int elastic_num;
    static int fission_num;
    static int nu_fission_num;

    static int produce_particle_num,dead_particle_num,out_particle_num;

    static void convert_density();
    static double get_mat_total_val(double engery);
    static int binary_search(double gloal,std::vector<double> *tmp);

    static void init_particles();
    static void init();
    static void post_handle();

    static void transport(particle &p);
    static void collision(particle &p);

    static void elastic(particle &p,nuclide &n);
    static void absorption(particle &p,nuclide &n);
    static void fission(particle &p,nuclide &n);
    static void nu_fission(particle &p,nuclide &n);

    static void simulate();
    static void choose_nuclide(nuclide &goal_n,particle &p);
    static int choose_reaction_type(nuclide &goal_n,particle &p);

//    static random_init_num

};

#endif // GLOBAL_H
