#include "global.h"
#include "input_xml.h"
#include "ace.h"
#include "iostream"


map<string,xslisting> *global::xsliistings = new map<string,xslisting>;
materials *global::gmaterials = new materials();

source *global::gsource = new source();
vector<particle> *global::gparticles = new vector<particle>;

int global::now_particle_num = 0;
int global::absorption_num = 0;
int global::elastic_num = 0;
int global::fission_num = 0;
int global::nu_fission_num = 0;

const int global::ASCII = 0,global::BINARY = 1;
const int global::ACE_NEUTRON = 0,global::ACE_THERMAL=1;
const double global::N_AVOGADRO = 0.602214129,global::MASS_NEUTRON=1.008664916;
const double global::PI = 3.1415926;


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

    if(!p.is_alive) return;

    double mat_cs = global::get_mat_total_val(p.engery);
    p.now_mat_total_cs = mat_cs;

    double random_flo = (rand()%RAND_MAX) / double(RAND_MAX);

    double length = -log(random_flo) / mat_cs;

    double now_x = p.xyz[0] + length * p.uvw[0];
    double now_y = p.xyz[1] + length * p.uvw[1];
    double now_z = p.xyz[2] + length * p.uvw[2];

//    std::cout << length << ": (" << now_x << "," << now_y << "," << now_z << ")" << std::endl;

    if(global::gmaterials->geometry_x > now_x && -global::gmaterials->geometry_x < now_x
            && global::gmaterials->geometry_y > now_y && -global::gmaterials->geometry_x < now_y
            && global::gmaterials->geometry_z > now_z && -global::gmaterials->geometry_x < now_z)
    {
        p.xyz[0] = now_x;
        p.xyz[1] = now_y;
        p.xyz[2] = now_z;
    }else{
        p.is_alive = false;
        p.is_out = true;
        global::now_particle_num --;
    }

}

void global::absorption(particle &p,nuclide &n){
    p.is_alive = false;
    p.is_absorption = true;
    global::now_particle_num --;
    global::absorption_num ++;
}

void global::elastic(particle &p,nuclide &n){

    //能量不变，求次级中子的出射方向
    double cm_cosa,lb_cosa;
    angle_enger_dist *tmp_p_angle_enger_dist = n.angle_values->at(0); //弹性散射的角分布（质心系）
    //根据中子能量选则角分布序列
    int index = global::binary_search(p.engery,tmp_p_angle_enger_dist->engery_grid);
    double random_flo = (rand()%RAND_MAX) / double(RAND_MAX);
    int angle_index;

    if(tmp_p_angle_enger_dist->type > 0) //32分布
    {
        angle_index = floor(random_flo * 32);
        cm_cosa = *((tmp_p_angle_enger_dist->angle_vals_32->at(index))+angle_index);

    }else if(tmp_p_angle_enger_dist->type > 0){

//        if(random_flo <= (tmp_p_angle_enger_dist->angle_num->at(index))->at(0)){

//            angle_index = 0;
//        }else{
        for(int i=0;i<tmp_p_angle_enger_dist->angle_num->at(index);i++){
            if(random_flo <= (tmp_p_angle_enger_dist->cdf->at(index))->at(i)){
                angle_index = i;
                break;
            }
        }
        cm_cosa = (tmp_p_angle_enger_dist->consines->at(index))->at(angle_index);
//        }
    }else{
        cm_cosa = 2*random_flo - 1;
    }

    //质心系到实验室系的转换
    lb_cosa = (1+n.A * cm_cosa) / sqrt(n.A*n.A + 2*n.A*cm_cosa + 1);

    //获得次级中子出射方向
    double new_uvw[3];
    double random_angle = global::PI * 2 * (rand()%RAND_MAX) / double(RAND_MAX);

    new_uvw[0] = lb_cosa*p.uvw[0] +
            sqrt(1-lb_cosa*lb_cosa)
            *(p.uvw[0]*p.uvw[2]*cos(random_angle) - p.uvw[1]*sin(random_angle))
            /sqrt(1-p.uvw[2]*p.uvw[2]);

    new_uvw[1] = lb_cosa*p.uvw[1] +
            sqrt(1-lb_cosa*lb_cosa)
            *(p.uvw[1]*p.uvw[2]*cos(random_angle) - p.uvw[0]*sin(random_angle))
            /sqrt(1-p.uvw[2]*p.uvw[2]);

    new_uvw[2] = lb_cosa*p.uvw[2] -
            sqrt(1-lb_cosa*lb_cosa)
            *sqrt(1-p.uvw[2]*p.uvw[2])
            *cos(random_angle);

    p.uvw[0] = new_uvw[0];
    p.uvw[1] = new_uvw[1];
    p.uvw[2] = new_uvw[2];

    //得到中子出射能量
    p.engery = p.engery*(1+n.A*n.A+2*cm_cosa*n.A)/(1+n.A)/(1+n.A);

    global::elastic_num ++;

}

void global::fission(particle &p,nuclide &n){



}

void global::nu_fission(particle &p,nuclide &n){

}

void global::collision(particle &p){

    if(!p.is_alive) return;

    //决定跟哪个核碰撞
    nuclide goal_n;
    global::choose_nuclide(goal_n,p);

//    std::cout << "name:" << goal_n.name << goal_n.awr<< std::endl;
    //发生哪种反应
    switch (global::choose_reaction_type(goal_n,p)) {
    case 0:
        global::elastic(p,goal_n);
        break;
    case 1:
        global::absorption(p,goal_n);
        break;
    case 2:
        global::fission(p,goal_n);
        break;
    default:
        global::nu_fission(p,goal_n);
        break;
    }

}

int global::choose_reaction_type(nuclide &goal_n,particle &p){

    int index = global::binary_search(p.engery,goal_n.energy);
    double random_flo = (rand()%RAND_MAX) / double(RAND_MAX);

    if(random_flo <= goal_n.elastic->at(index)/goal_n.total->at(index)){
        return 0;   //弹性散射
    }else if(random_flo <=(goal_n.elastic->at(index)+goal_n.absorption->at(index))/goal_n.total->at(index)){
        return 1;   //吸收
    }else if(random_flo <= (1-goal_n.nu_fission->at(index)/goal_n.total->at(index))){
        return 2;   //裂变
    }else{
        return 3;   //除裂变外的非弹性散射
    }


}

void global::choose_nuclide(nuclide &goal_n,particle &p){

    std::map<string,nuclide>::iterator it=global::gmaterials->nuclides->begin();
    double random_flo = (rand()%RAND_MAX) / double(RAND_MAX);
    int index;
    double mat_total_cs = p.now_mat_total_cs;
    double c_mat_total1 = 0.0,c_mat_total2=0.0;

    for(;it!=global::gmaterials->nuclides->end();it++){

        index = global::binary_search(p.engery,it->second.energy);

        if(it == global::gmaterials->nuclides->begin()){
            c_mat_total1 = 0.0;
            c_mat_total2 = c_mat_total1 + (it->second.awr * (*(it->second.total))[index]);
        }else{
            c_mat_total1 = c_mat_total2;
            c_mat_total2 = c_mat_total1 + (it->second.awr * (*(it->second.total))[index]);
        }

        //std::cout << it->first << ":" << random_flo << ": (" << c_mat_total1 << "," << c_mat_total2 << ")" << std::endl;
//        std::cout << it->first << ":" << random_flo <<  ": (" << c_mat_total1/mat_total_cs << "," << c_mat_total2/mat_total_cs << ")" << std::endl;

        if(random_flo <= c_mat_total2/mat_total_cs && random_flo > c_mat_total1/mat_total_cs){
            goal_n = it->second;
            break;
        }
    }


}

void global::simulate(){

    while(1){

        for(int i=0;i<10/*global::gsource->particle_num*/;i++){

            global::transport((*global::gparticles)[i]);
            global::collision(global::gparticles->at(i));

        }

        break;
    }

}


