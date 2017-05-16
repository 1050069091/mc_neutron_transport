#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include <xslisting.h>
#include <string>
#include <materials.h>
#include <nuclide.h>

using namespace std;

class global
{
public:
    global();

    static map<string,xslisting> *xsliistings;
    static materials *gmaterials;


    static const int BINARY,ASCII,ACE_NEUTRON,ACE_THERMAL;

};

#endif // GLOBAL_H
