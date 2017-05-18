#include "ace.h"

#include "nuclide.h"
#include <fstream>
#include <iostream>

ace::ace()
{

}

void ace::read_ace_xs(){


    int JXS[32];
    int NXS[16];
    std::vector<double> XSS;

    map<string,nuclide>::iterator it ;
    std::ifstream *fin;

    char tmp_buf[1024] = {0};
    std::string tm_name,tmp_date;
    double tmp_kT,tmp_awr,tmp_double;

    int macro_length;
    int XSS_index,XSS_index1;


    for(it=global::gmaterials->nuclides->begin();it!=global::gmaterials->nuclides->end();it++)
    {
        XSS.clear();

        if((*global::xsliistings)[it->first].filetype == global::ASCII){

            fin = new std::ifstream((*global::xsliistings)[it->first].path,std::ios::in);

//            std::cout << (*global::xsliistings)[it->first].path << std::endl;

            for(int i=1;i<(*global::xsliistings)[it->first].location;i++)
                (*fin) >> tmp_buf;

            (*fin) >> tm_name >> tmp_awr >> tmp_kT >> tmp_date;

//            it->second.awr = tmp_awr;

            fin->getline(tmp_buf,1024,'\n');
            fin->getline(tmp_buf,1024,'\n');

//            std::cout << tmp_buf << std::endl;

            for(int i=0;i<4;i++){
                fin->getline(tmp_buf,1024,'\n');
            }

//            fin->getline(tmp_buf,1024,'\n');


            for(int i=0;i<16;i++)
                *fin >> NXS[i];

            for(int i=0;i<32;i++)
                *fin >> JXS[i];

            for(int i=0;i<NXS[0];i++){

                *fin >> tmp_double;

                XSS.push_back(tmp_double);
            }

             fin->close();


//            fin->getline(tmp_buf,1024,'\n');

//            for(int i=0;i<4;i++){
//                for(int j=0;j<8;j++){
//                    *fin >> NXS[i*8+j];
//                }
//            }

//            for(int i=0;i<NXS[1];i++)
//                std::cout << XSS[i] << "\t";

//            it->second.awr = tmp_awr;
            it->second.kT = tmp_kT;//
            it->second.zaid = (*global::xsliistings)[it->first].zaid;

            XSS_index = JXS[0] - 1;
            XSS_index1 = JXS[0] - 1 + 3*macro_length;

            macro_length = NXS[2];
            it->second.macro_length = macro_length;

            for(int i=0;i<macro_length;i++){
                it->second.energy->push_back(XSS[XSS_index]);
                it->second.elastic->push_back(XSS[XSS_index1]);
                XSS_index ++;
                XSS_index1 ++;
            }

//            XSS_index = 1*macro_length;
//            for(int i=0;i<macro_length;i++){
//                it->second.total->push_back(XSS[++XSS_index]);
//            }

//            XSS_index = 2*macro_length;
//            for(int i=0;i<macro_length;i++){
//                it->second.absorption->push_back(XSS[++XSS_index]);
//            }

//            XSS_index = 3*macro_length;
//            for(int i=0;i<macro_length;i++){
//                it->second.elastic->push_back(XSS[XSS_index]);
//                 XSS_index ++;
//            }

//            for(int i=0;i<2;i++){
//                std::cout << it->first << "----->" <<
//                            "   en:" << (*(it->second.energy))[i] <<
//                            "   to:" <<(*(it->second.total))[i] <<
//                            "   ab:" <<(*(it->second.absorption))[i] <<
//                            "   el:" <<(*(it->second.elastic))[i] <<
//                            std::endl;

//            }

//            for(int i=0;i<macro_length;i++)
//                std::cout << (*(it->second.elastic))[i] << "\t";


//            std::cout << it->second.key_in_xsmap << "---" <<it->second.zaid << std::endl;
//            std::cout << std::endl;


            XSS_index = JXS[2] - 1;
            XSS_index1 = JXS[3] - 1;
            (it->second).reaction_num = NXS[3] + 1;

            it->second.reaction_mts->push_back(2);
            it->second.Q_values->push_back(0.0);

            for(int i=0;i<NXS[3];i++){
                it->second.reaction_mts->push_back(XSS[XSS_index]);
                it->second.Q_values->push_back(XSS[XSS_index1]);
                XSS_index ++;
                XSS_index1 ++;
            }

//            std::cout << std::endl << it->first << "---->" << (it->second).reaction_num << ":";
//            for(int i=0;i<(it->second).reaction_num;i++){
//                std::cout << (*(it->second.reaction_mts))[i] << "=" << (*(it->second.Q_values))[i] << "    ";
//            }

//            std::cout << std::endl;

//            for(int i=0;i<20;i++){
//                std::cout << XSS[i] << "  ";
//            }



//            for(int i=-10;i<10;i++){
//                std::cout << i << ":" <<XSS_index+10+i <<":"<< XSS[XSS_index + i] << "  ";
//            }
//            std::cout << std::endl;

            int tmp_loc,loc;
            XSS_index = JXS[5] - 1;
            loc = JXS[6] - 1;

            for(int i=0;i<NXS[3];i++){

               tmp_loc = loc + int(XSS[XSS_index]) - 1;

               it->second.thresholds->push_back(int(XSS[tmp_loc]) - 1);

               XSS_index1 = tmp_loc + 2;

               std::vector<double> *tmp_vector = new std::vector<double>();

               for(int j=0;j<int(XSS[tmp_loc + 1]);j++){
                    tmp_vector->push_back(XSS[XSS_index1]);

                    XSS_index1 ++;
               }
               it->second.reaction_cs_values->push_back(tmp_vector);
//               std::cout << tmp_vector->size() << ":" << ((*(it->second.reaction_cs_values))[i])->size() << std::endl;
               XSS_index ++;
//                 std::cout << XSS[XSS_index] << "    ";
//                 XSS_index ++;
            }

//            std::cout << it->first << "---->" << NXS[3] << ":" << std::endl;
//            for(int i=0;i<NXS[3];i++){
//                std::cout << (*(it->second.reaction_mts))[i+1] << "="
//                          << (*(it->second.Q_values))[i+1] << "="
//                          << (*(it->second.thresholds))[i] << "="
//                          << (*(it->second.reaction_cs_values))[i]->size() << "------->"<< NXS[2]
//                          << std::endl;
//                for(int j=0;j<(*(it->second.reaction_cs_values))[i]->size();j++){
//                     std::cout << (*((*(it->second.reaction_cs_values))[i]))[j] << "   ";
//                }
//            }
//            std::cout << std::endl;


            for(int i=0;i<NXS[2];i++){

            }



        }else

        {
            std::cout << "error:can't handle binary file " << (*global::xsliistings)[it->first].path << std::endl;
            exit(-1);
        }


//        std::cout << (*global::xsliistings)[it->first].path << std::endl;
    }


}
