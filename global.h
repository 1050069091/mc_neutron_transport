#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include <xslisting.h>
#include <string>

using namespace std;

class global
{
public:
    global();

    static map<string,xslisting> *xsliistings;

    static const int BINARY,ASCII,ACE_NEUTRON,ACE_THERMAL;

};

#endif // GLOBAL_H
