#include "xslisting.h"

xslisting::xslisting()
{
    this->name = "NULL";       // table name, e.g. 92235.70c
    this->alias = "NULL";      // table alias, e.g. U-235.70c
    this->type = 0;          // type of table (cont-E neutron, S(A,b), etc)
    this->zaid = -1;         //ZAID identifier = 1000*Z + A
    this->filetype = 0;   //ASCII or BINARY
    this->location = -1;   //location of table within library
    this->recl = -1;   //record length for library
    this->entries = 0;    //number of entries per record
    this->awr = -1.0; //atomic weight ratio (# of neutron masses)
    this->kT = -1.0;  //Boltzmann constant * temperature (MeV)
    this->metastable = false;    //is this nuclide metastable?
    this->path = "NULL";    //path to library containing table
}
