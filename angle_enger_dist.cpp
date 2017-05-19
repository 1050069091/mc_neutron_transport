#include "angle_enger_dist.h"

angle_enger_dist::angle_enger_dist()
{
    this->angle_num = 0;
    this->engery_grid = new std::vector<double>;
    this->loc_angle_val = new std::vector<int>;

    this->angle_vals_32 = new std::vector<double *>;

    this->flags = new std::vector<int>;
    this->angle_num = new std::vector<int>;
    this->consines = new std::vector<std::vector<double> *>;
    this->pdf = new std::vector<std::vector<double> *>;
    this->cdf = new std::vector<std::vector<double> *>;

}

//int engery_num;
//std::vector<double> *engery_grid;
//std::vector<int> *loc_angle_val;

//std::vector<double *> *angle_vals_32;

//std::vector<bool *> *flags;
//std::vector<std::vector<int> *> *angle_num;
//std::vector<std::vector<std::vector<double> *> *> * consines;
//std::vector<std::vector<std::vector<double> *> *> * pdf;
//std::vector<std::vector<std::vector<double> *> *> * cdf;
