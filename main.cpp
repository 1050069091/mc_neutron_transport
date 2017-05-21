#include <iostream>
//#include "pugixml.hpp"
#include "global.h"
//#include "input_xml.h"
//#include "ace.h"

//using namespace std;

int main(int argc, char *argv[])
{
    time_t start_time = time(NULL);
    time_t end_time;

    global::init();

    global::simulate();

    global::post_handle();

    end_time = time(NULL);

    std::cout << "---------------------串行mc中子输运程序----------------------"<< std::endl;
    std::cout << "用时" << (end_time - start_time) << "s" << std::endl;

    return 0;
}
