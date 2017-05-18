#ifndef SOURCE_H
#define SOURCE_H


class source
{
public:
    source();

    int particle_num;
    double site_xyz[3];
    double min_engery,max_engery;
    double max_angle;
};

#endif // SOURCE_H
