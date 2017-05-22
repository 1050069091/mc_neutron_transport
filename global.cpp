#include "global.h"
#include "input_xml.h"
#include "ace.h"
#include "iostream"
//#include <omp.h>


map<string,xslisting> *global::xsliistings = new map<string,xslisting>;
materials *global::gmaterials = new materials();

source *global::gsource = new source();
vector<particle> *global::gparticles = new vector<particle>;
vector<particle> *global::new_produce_gparticles = new vector<particle>;

int global::now_particle_num = 0;
int global::absorption_num = 0;
int global::elastic_num = 0;
int global::fission_num = 0;
int global::nu_fission_num = 0;

int global::produce_particle_num = 0,global::dead_particle_num = 0,global::out_particle_num = 0;

const int global::ASCII = 0,global::BINARY = 1;
const int global::ACE_NEUTRON = 0,global::ACE_THERMAL=1;
const double global::N_AVOGADRO = 0.602214129,global::MASS_NEUTRON=1.008664916;
const double global::PI = 3.1415926;


global::global(){

}

void global::post_handle(){

    //材料所占内存
//    for(map<string,nuclide>::iterator it=global::gmaterials->nuclides->begin();
//        it !=global::gmaterials->nuclides->end();it++){
//        free(&(it->second));
//    }
    global::gmaterials->nuclides->clear();
    free(global::gmaterials);

    //粒子所占内存
//    for(vector<particle>::iterator it=global::gparticles->begin();it!=global::gparticles->end();it++){
//        free(&(it->second));
//    }
    global::gparticles->clear();
    free(global::gparticles);

    //源所在内存
    free(global::gsource);

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
        particle *tmp_particle = new particle(true);
        global::gparticles->push_back(*tmp_particle);
    }

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
            && global::gmaterials->geometry_y > now_y && -global::gmaterials->geometry_y < now_y
            && global::gmaterials->geometry_z > now_z && -global::gmaterials->geometry_z < now_z)
    {
        p.xyz[0] = now_x;
        p.xyz[1] = now_y;
        p.xyz[2] = now_z;
    }else{
        p.is_alive = false;
        p.is_out = true;
//        std::cout << "1out" << global::now_particle_num << std::endl;
        global::now_particle_num = global::now_particle_num - 1;
        global::out_particle_num += 1;
//        std::cout << "2out" << global::now_particle_num << std::endl;

//        particle *tmp_p = &p
    }

}

void global::absorption(particle &p,nuclide &n){
    p.is_alive = false;
    p.is_absorption = true;
//    std::cout << "1absorption" << global::now_particle_num << std::endl;
    global::now_particle_num = global::now_particle_num - 1;
//    std::cout << "2absorption" << global::now_particle_num << std::endl;
    global::absorption_num = global::absorption_num + 1;

    global::dead_particle_num += 1;
}

void global::elastic(particle &p,nuclide &n){

    //能量不变，求次级中子的出射方向
    double cm_cosa,lb_cosa;
    angle_enger_dist *tmp_p_angle_enger_dist = n.angle_values->at(0); //弹性散射的角分布（质心系）
    //根据入射中子能量选则角分布序列
    int index = global::binary_search(p.engery,tmp_p_angle_enger_dist->engery_grid);
    double random_flo = (rand()%RAND_MAX) / double(RAND_MAX);
    int angle_index;

    if(tmp_p_angle_enger_dist->type > 0) //32分布
    {
        angle_index = floor(random_flo * 32);
        cm_cosa = *((tmp_p_angle_enger_dist->angle_vals_32->at(index))+angle_index);

    }else if(tmp_p_angle_enger_dist->type < 0){

        for(int i=0;i<tmp_p_angle_enger_dist->angle_num->at(index);i++){
            if(random_flo <= (tmp_p_angle_enger_dist->cdf->at(index))->at(i)){
                angle_index = i;
                break;
            }
        }
        cm_cosa = (tmp_p_angle_enger_dist->consines->at(index))->at(angle_index);

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
    if(p.engery <= 1e-6){
        p.is_alive = false;
//        std::cout << "1elastic" << global::now_particle_num << std::endl;
        global::now_particle_num = global::now_particle_num - 1;
        global::dead_particle_num += 1 ;
//        std::cout << "2elastic" << global::now_particle_num << std::endl;
    }
    global::elastic_num ++;
}

void global::fission(particle &p,nuclide &n){

    //次级中子个数
    double cm_cosa,lb_cosa;

    int second_particle_num;
    double tmp_double = 0.0,random_flo;
    double input_engery_in_Mev = p.engery / 1000000.0;

    if(n.p_fission_second_particle->flag == 1) //多项式求个数{
    {
        for(int i=0;i<n.p_fission_second_particle->coefficients_length;i++){
            tmp_double += n.p_fission_second_particle->coefficients->at(i) * pow(input_engery_in_Mev,i);
        }
        second_particle_num = ceil(tmp_double);

    }else{  //插值求个数
        int index = global::binary_search(p.engery,n.p_fission_second_particle->engery_grid);
        second_particle_num = n.p_fission_second_particle->second_particle_nums->at(index);
    }

    //将原中子置为死亡状态
    p.is_alive = false;
    p.is_fission = true;
    global::fission_num = global::fission_num + 1;

    //随机抽样具体进行哪种类型的裂变
//    int mts_index = (rand()%n.fission_index_in_reaction_mts->size());

    //建立新的second_particle_num个中子
    for(int i=0;i<second_particle_num;i++){

        particle *new_particle = new particle(false);
        new_particle->xyz[0] = p.xyz[0];
        new_particle->xyz[1] = p.xyz[1];
        new_particle->xyz[2] = p.xyz[2];

        //抽样能量
        int engery_index = rand()%(n.energy->size());
        new_particle->engery = n.energy->at(engery_index);

        //抽样角度
        random_flo = (rand()%RAND_MAX) / double(RAND_MAX);

        cm_cosa = 2*random_flo - 1;
        //质心系到实验室系的转换
        lb_cosa = (1+n.A * cm_cosa) / sqrt(n.A*n.A + 2*n.A*cm_cosa + 1);
        //计算速度
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

        new_particle->uvw[0] = new_uvw[0];
        new_particle->uvw[1] = new_uvw[1];
        new_particle->uvw[2] = new_uvw[2];

        global::new_produce_gparticles->push_back(new_particle);
    }

//    std::cout << "1fission" << global::now_particle_num << std::endl;
    global::now_particle_num = global::now_particle_num + second_particle_num - 1;
//    std::cout << "2fission" << global::now_particle_num << std::endl;

    global::produce_particle_num += second_particle_num - 1;
}

void global::nu_fission(particle &p,nuclide &n){

    //随机抽样具体进行哪种非弹性散射
    int mts_index =
            n.nu_fission_index_in_reaction_mts->at((rand()%n.nu_fission_index_in_reaction_mts->size()));
    //确认二级中子个数
    int second_particle_num = n.second_particle_num_dis->at(mts_index);

    double cosa,lb_cosa;

    double random_flo,random_angle;
    double new_uvw[3];

    if(second_particle_num > 100){
        second_particle_num = second_particle_num - 99;
    }else if(second_particle_num < -100){
        second_particle_num = second_particle_num + 99;
    }

    for(int j=0;j<abs(second_particle_num);j++){

        random_flo = (rand()%RAND_MAX) / double(RAND_MAX);

        particle *new_particle = new particle(false);
        new_particle->xyz[0] = p.xyz[0];
        new_particle->xyz[1] = p.xyz[1];
        new_particle->xyz[2] = p.xyz[2];

        //抽样能量
        int engery_index = rand()%(n.energy->size());
        new_particle->engery = n.energy->at(engery_index);

        //抽样角度
        random_flo = (rand()%RAND_MAX) / double(RAND_MAX);
        cosa = 2*random_flo - 1;

        //坐标系转换
        if(second_particle_num < 0){  //质心系
            //质心系到实验室系的转换
            lb_cosa = (1+n.A * cosa) / sqrt(n.A*n.A + 2*n.A*cosa + 1);
        }else{//实验室系
            lb_cosa = cosa;
        }

        //获得次级中子出射方向
        random_angle = global::PI * 2 * (rand()%RAND_MAX) / double(RAND_MAX);

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

        new_particle->uvw[0] = new_uvw[0];
        new_particle->uvw[1] = new_uvw[1];
        new_particle->uvw[2] = new_uvw[2];

        //得到中子出射能量
        new_particle->engery = pow((n.A/(n.A+1)),2)*(p.engery-(n.A+1)*n.Q_values->at(mts_index)/n.A);

        global::new_produce_gparticles->push_back(new_particle);
    }

    p.is_alive = false;

//    std::cout << "1nu_fission" << global::now_particle_num << std::endl;
    global::now_particle_num = global::now_particle_num + abs(second_particle_num) - 1;
//    std::cout << "2nu_fission" << global::now_particle_num << std::endl;
    global::produce_particle_num += abs(second_particle_num) - 1;
}

void global::collision(particle &p){

    if(!p.is_alive) return;

    //决定跟哪个核碰撞
    nuclide goal_n;
    global::choose_nuclide(goal_n,p);

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
//        std::cout << "elastic" << std::endl;
        return 0;   //弹性散射
    }else if(random_flo <= 1.06*(goal_n.elastic->at(index)+goal_n.absorption->at(index))/goal_n.total->at(index)){
//        std::cout << "absorption" << std::endl;
        return 1;   //吸收
    }else if(goal_n.can_fissioable && (random_flo <= (1-goal_n.nu_fission->at(index)/goal_n.total->at(index)))){
//        std::cout << "fission" << std::endl;
        return 2;   //裂变
    }else{
//        std::cout << "nu_fission" << std::endl;
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

        if(random_flo <= c_mat_total2/mat_total_cs && random_flo > c_mat_total1/mat_total_cs){
            goal_n = it->second;
            break;
        }
    }

}

void global::simulate(){

    int recur = 0;
    int last_recur_particle = global::gsource->particle_num;
    int alive_particle_num = 0;
    int last_produced_particle_num = global::gsource->particle_num;

    vector<particle>::iterator it;


    while(1){

        recur ++;
        alive_particle_num = 0;

        if(recur > 1)
            last_produced_particle_num = global::produce_particle_num;

        global::produce_particle_num = 0,global::dead_particle_num = 0,global::out_particle_num = 0;


        for(it=global::new_produce_gparticles->begin();it!=global::new_produce_gparticles->end();it++){
            global::gparticles->push_back(*it);
        }

        global::new_produce_gparticles->clear();

        for(it=global::gparticles->begin();it!=global::gparticles->end();){
            if(it->is_alive){
                alive_particle_num ++;
                global::transport((*it));
                global::collision((*it));
                it++;
            }else{
                it = global::gparticles->erase(it);
            }
        }

        last_recur_particle = global::gparticles->size();

        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << "运输碰撞代数:" << recur << std::endl;
        std::cout << "    初始中子数:" << global::gsource->particle_num << std::endl
                  << "    上代新产生的中子数:" << last_produced_particle_num << std::endl
                  << "    该代新产生的中子数:" << global::produce_particle_num << std::endl
                  << "    消亡的中子数:" << global::dead_particle_num/*+global::out_particle_num*/ << std::endl
                  << "    逃出材料边界的中子数:" << global::out_particle_num << std::endl
                  << "    有效增殖因子keff:" << global::produce_particle_num/double(last_produced_particle_num)/*global::produce_particle_num/double(global::dead_particle_num+global::out_particle_num+1)*/ << std::endl
                  << "    当前总存活中子数:" << alive_particle_num-(global::dead_particle_num+global::out_particle_num)<< std::endl
                  ;

        if(recur > global::gsource->max_recur) return;

        if(alive_particle_num <= 0) return;
    }

}


