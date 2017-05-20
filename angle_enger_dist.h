#ifndef ANGLE_ENGER_DIST_H
#define ANGLE_ENGER_DIST_H

#include <vector>

class angle_enger_dist
{
public:
    angle_enger_dist();

    int engery_num;
    std::vector<double> *engery_grid;
    std::vector<int> *loc_angle_val;

    int type;

    std::vector<double *> *angle_vals_32;

    std::vector<int> *flags;
    std::vector<int> *angle_num;
    std::vector<std::vector<double> *> * consines;
    std::vector<std::vector<double> *> * pdf;
    std::vector<std::vector<double> *> * cdf;

};

#endif // ANGLE_ENGER_DIST_H
