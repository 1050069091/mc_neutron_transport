#include "global.h"
#include "input_xml.h"
#include "ace.h"
#include "iostream"


map<string,xslisting> *global::xsliistings = new map<string,xslisting>;
materials *global::gmaterials = new materials();

source *global::gsource = new source();
vector<particle> *global::gparticles = new vector<particle>;

const int global::ASCII = 0,global::BINARY = 1;
const int global::ACE_NEUTRON = 0,global::ACE_THERMAL=1;
const double global::N_AVOGADRO = 0.602214129,global::MASS_NEUTRON=1.008664916;


global::global(){

}

void global::init(){

    input_xml::read_xml();
    ace::read_ace_xs();

    global::convert_density();

    srand((unsigned)time(NULL));
    global::init_particles();
}


void global::convert_density(){

    double sum_percent = 0.0,tmp_awr;

    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
        it != global::gmaterials->nuclides->end();it++)
    {
        sum_percent += (it->second).awr;
    }

//    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
//        it != global::gmaterials->nuclides->end();it++)
//    {
//        std::cout << it->first << ":  " << (it->second).awr << std::endl;
//    }

    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
        it != global::gmaterials->nuclides->end();it++)
    {
        (it->second).awr = (it->second).awr / sum_percent;
    }

    sum_percent = 0.0;
    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
        it != global::gmaterials->nuclides->end();it++)
    {
        tmp_awr = (*global::xsliistings)[it->first].awr;
        sum_percent += tmp_awr * ((it->second).awr);
    }
    sum_percent = 1 / sum_percent;

    global::gmaterials->density = global::gmaterials->density * global::N_AVOGADRO / global::MASS_NEUTRON * sum_percent;


    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
        it != global::gmaterials->nuclides->end();it++)
    {
        (it->second).awr = (it->second).awr * global::gmaterials->density;
    }

//    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
//        it != global::gmaterials->nuclides->end();it++)
//    {
//        std::cout << it->first << ":  " << (it->second).awr << std::endl;
//    }


//    mat%density = -mat%density * N_AVOGADRO &
//                 / MASS_NEUTRON * sum_percent



}

void global::init_particles(){
    for(int i=0;i<global::gsource->particle_num;i++){
        particle *tmp_particle = new particle();
        global::gparticles->push_back(*tmp_particle);
    }
//    for(int i=0;i<global::gparticles->size();i++){
//        std::cout << (*global::gparticles)[i].engery << "   " << (*global::gparticles)[i].xyz[0] << std::endl;
//    }
}


void global::transport(particle &p){

}

void global::collision(particle &p){

}

void global::simulate(){

}


