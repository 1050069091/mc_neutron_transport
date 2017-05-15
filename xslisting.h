#ifndef XSLISTING_H
#define XSLISTING_H

#include <string>
using namespace std;

class xslisting
{
public:
    xslisting();
    string name;       // table name, e.g. 92235.70c
    string alias;      // table alias, e.g. U-235.70c
    int type;          // type of table (cont-E neutron, S(A,b), etc)
    int zaid;         //ZAID identifier = 1000*Z + A
    int filetype;   //ASCII or BINARY
    int location;   //location of table within library
    int recl;   //record length for library
    int entries;    //number of entries per record
    double awr; //atomic weight ratio (# of neutron masses)
    double kT;  //Boltzmann constant * temperature (MeV)
    bool metastable;    //is this nuclide metastable?
    string path;    //path to library containing table
};
#endif // XSLISTING_H
