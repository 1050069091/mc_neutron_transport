#include "ace.h"

#include "nuclide.h"
#include <iostream>

ace::ace()
{

}

void ace::read_ace_xs(){

    map<string,nuclide>::iterator it ;
    std::ifstream fin;


    for(it=global::gmaterials->nuclides->begin();it!=global::gmaterials->nuclides->end();it++)
    {
        if((*global::xsliistings)[it->first].filetype == global::BINARY){

            fin = new std::ifstream((*global::xsliistings)[it->first].path,std::ios::binary);

        }else
        {
            fin = new std::ifstream((*global::xsliistings)[it->first].path);
        }


//        std::cout << (*global::xsliistings)[it->first].path << std::endl;
    }


}
