#ifndef PARTICLE_H
#define PARTICLE_H


class particle
{
public:
    particle();
    double xyz[3];
    double uvw[3];
    double engery;
private:
    void init_info();
};

#endif // PARTICLE_H
