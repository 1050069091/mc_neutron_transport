#include <iostream>
#include "pugixml.hpp"
#include "global.h"
#include "input_xml.h"
#include "ace.h"

//using namespace std;

int main(int argc, char *argv[])
{

    input_xml::read_xml();

    ace::read_ace_xs();


    return 0;
}
