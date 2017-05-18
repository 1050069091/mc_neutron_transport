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

    static void convert_density();
    static void init_particles();
    static void init();

    static void transport(particle &p);
    static void collision(particle &p);

    static void simulate();

//    static random_init_num

};

#endif // GLOBAL_H
