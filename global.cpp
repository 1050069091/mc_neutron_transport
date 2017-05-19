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

//    global::gmaterials->nuclides;

    srand((unsigned)time(NULL));
    global::init_particles();

//    std::cout << global::get_mat_total_val(0.1) << std::endl;

}

double global::get_mat_total_val(double engery){

    double mat_total = 0.0;
    int index;
    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
        it != global::gmaterials->nuclides->end();it++){
        index = global::binary_search(engery,it->second.energy);

//        std::cout << index << ":" << (*(it->second.total))[index] << ":" << it->second.awr << std::endl;
        mat_total += (*(it->second.total))[index] * (it->second.awr);
    }
    return mat_total;

}

int global::binary_search(double gloal,std::vector<double> *tmp){

//    int mid;

    int size = tmp->size();

    if(gloal <= (*tmp)[0]) return 0;
    if(gloal >= (*tmp)[size-1]) return size-1;

//    if(low >= high) return low;

//    mid = floor((high+low)/2);

//    if((*tmp)[mid -1] > gloal)
//        return global::binary_search(gloal,tmp,low,mid -1);

//    if((*tmp)[mid + 1] > gloal)
//        return global::binary_search(gloal,tmp,mid,high);

//    if((*tmp)[mid -1] <= gloal && (*tmp)[mid] > gloal)
//        return mid -1;

//    if((*tmp)[mid] <= gloal && (*tmp)[mid+1] > gloal)
//        return mid;

    for(int i=0;i<tmp->size();i++){
        if((*tmp)[i] <= gloal && (*tmp)[i+1] > gloal)
            return i;
    }

}

void global::convert_density(){

    double sum_percent = 0.0,tmp_awr;

    for(std::map<string,nuclide>::iterator it= global::gmaterials->nuclides->begin();
        it != global::gmaterials->nuclides->end();it++)
    {
        sum_percent += (it->second).awr;
    }

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


