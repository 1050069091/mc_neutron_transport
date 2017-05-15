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

};

#endif // GLOBAL_H
