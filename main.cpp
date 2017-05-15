#include <iostream>
#include "pugixml.hpp"
#include "global.h"
#include "input_xml.h"

using namespace std;

int main(int argc, char *argv[])
{

    input_xml::read_cross_sections_xml();

    return 0;
}
