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
    int XSS_index,XSS_index1,XSS_index2;


    for(it=global::gmaterials->nuclides->begin();it!=global::gmaterials->nuclides->end();it++)
    {
        XSS.clear();

        if((*global::xsliistings)[it->first].filetype == global::ASCII){

            fin = new std::ifstream((*global::xsliistings)[it->first].path,std::ios::in);

            for(int i=1;i<(*global::xsliistings)[it->first].location;i++)
                (*fin) >> tmp_buf;

            (*fin) >> tm_name >> tmp_awr >> tmp_kT >> tmp_date;

            fin->getline(tmp_buf,1024,'\n');
            fin->getline(tmp_buf,1024,'\n');

            for(int i=0;i<4;i++){
                fin->getline(tmp_buf,1024,'\n');
            }

            for(int i=0;i<16;i++)
                *fin >> NXS[i];

            for(int i=0;i<32;i++)
                *fin >> JXS[i];

            for(int i=0;i<NXS[0];i++){
                *fin >> tmp_double;
                XSS.push_back(tmp_double);
            }

            fin->close();

            it->second.kT = tmp_kT;//
            it->second.zaid = (*global::xsliistings)[it->first].zaid;

            XSS_index = JXS[0] - 1;
            XSS_index2 = XSS_index + macro_length;

            XSS_index1 = JXS[0] - 1 + 3*macro_length;

            macro_length = NXS[2];
            it->second.macro_length = macro_length;

            for(int i=0;i<macro_length;i++){
                it->second.energy->push_back(XSS[XSS_index]);
                it->second.elastic->push_back(XSS[XSS_index1]);
                it->second.total->push_back(XSS[XSS_index1]);
                XSS_index ++;
                XSS_index1 ++;
//                XSS_index2 ++;
            }

            XSS_index = JXS[2] - 1;
            XSS_index1 = JXS[3] - 1;
            (it->second).reaction_num = NXS[3] /*+ 1*/;

//            it->second.reaction_mts->push_back(2);
//            it->second.Q_values->push_back(0.0);

            for(int i=0;i<NXS[3];i++){
                it->second.reaction_mts->push_back(XSS[XSS_index]);
                it->second.Q_values->push_back(XSS[XSS_index1]);
                XSS_index ++;
                XSS_index1 ++;
            }

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
               XSS_index ++;
            }

//            std::cout << it->first << "---->" << NXS[3] << ":" << std::endl;
//            for(int i=0;i<NXS[3];i++){
//                std::cout << (*(it->second.reaction_mts))[i] << "="
////                          << (*(it->second.Q_values))[i] << "="
////                          << (*(it->second.thresholds))[i] << "="
////                          << (*(it->second.reaction_cs_values))[i]->size() << "------->"<< NXS[2]
//                          << std::endl;
////                for(int j=0;j<(*(it->second.reaction_cs_values))[i]->size();j++){
////                     std::cout << (*((*(it->second.reaction_cs_values))[i]))[j] << "   ";
////                }
//            }
//            std::cout << std::endl;

//             for(int j=0;j<NXS[2];j++){
//                 (*(it->second.total))[j] = (*(it->second.total))[j] + (*(it->second.elastic))[j];
//             }

            //读取微观截面信息

            it->second.can_fissioable = (JXS[1] > 0);

            int mt_num,reaction_type,tmp_index,tmp_size,tmp_up;
            double tmp_mi_cs_val;

            for(int i=0;i<NXS[3];i++){  //反应数（不包括弹性散射）

                mt_num = (*(it->second.reaction_mts))[i];
                if(mt_num>=101 && mt_num <=117){
                    reaction_type = 1;//吸收
                }else if((mt_num>=18 && mt_num <=21) || mt_num==38 ){
                    reaction_type = 2;//裂变
                }else{
                    reaction_type = 3;//除裂变外的非弹性散射
                }

                tmp_index = (*(it->second.thresholds))[i];
                tmp_size = (*(it->second.reaction_cs_values))[i]->size();
                tmp_up = tmp_index + tmp_size;

                for(int j=0,k=0;j<NXS[2];j++){  //能量个数（一个中子能量，对应特定的微观反应截面）
                    if(j>=tmp_index && j<tmp_up){
                        tmp_mi_cs_val = (*((*(it->second.reaction_cs_values))[i]))[k];
                        k ++;
                    }else{
                        tmp_mi_cs_val = 0.0;
                    }
                    if(reaction_type == 1){
                        it->second.absorption->push_back(tmp_mi_cs_val);
                    }else if(reaction_type == 2){
                        it->second.fission->push_back(tmp_mi_cs_val);
                    }else{
                        it->second.nu_fission->push_back(tmp_mi_cs_val);
                    }
                    (*(it->second.total))[j] = (*(it->second.total))[j] + tmp_mi_cs_val;
                }

            }

//            for(int i=0;i<NXS[2];i++){
//                std::cout << "total:" << (*(it->second.total))[i]
//                             << "elastic:" << (*(it->second.elastic))[i]
//                                << "absorption:" << (*(it->second.absorption))[i];
//                if(it->second.can_fissioable){
//                    std::cout << "fission:" << (*(it->second.fission))[i];
//                }
//                std::cout << "nu_fission:" << (*(it->second.nu_fission))[i]<< std::endl;

//            }


            //读取碰撞后次级中子的角分布

            for(int i=0;i<NXS[4]+1;i++){



            }



        }else
        {
            std::cout << "error:can't handle binary file " << (*global::xsliistings)[it->first].path << std::endl;
            exit(-1);
        }

    }


}
