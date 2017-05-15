#include "input_xml.h"
#include "global.h"
#include "xslisting.h"
#include "pugixml.hpp"
#include "iostream"


void input_xml::read_cross_sections_xml()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    pugi::xml_node cross_sections_xml_node,tmp_xml_node;
    pugi::xml_attribute tmp_xml_attribute;

    int filetype = global::ASCII;
    int recl,entries;
    string filetype_str;

    xslisting *p_tmp_xslisting;

    result = doc.load_file("cross_sections.xml");

    if(!result)
    {
        std::cout << "read cross_sections.xm failed\n";
        exit(0);
    }

    cross_sections_xml_node = doc.child("cross_sections");

    if(tmp_xml_node=cross_sections_xml_node.child("filetype"))
    {
        filetype_str = tmp_xml_node.text().as_string();
        if(filetype_str.find("binary") != string::npos)
        {
            filetype = global::BINARY;
        }

    }

    recl = cross_sections_xml_node.child("record_length").text().as_int();
    entries = cross_sections_xml_node.child("entries").text().as_int();

//    std::cout << filetype << recl << entries << std::endl;

    for (pugi::xml_node tmp_node: cross_sections_xml_node.children("ace_table"))
    {
        p_tmp_xslisting = new xslisting();

        p_tmp_xslisting->name = tmp_node.attribute("name").as_string();

        if(!(tmp_xml_attribute = tmp_node.attribute("alias")).empty())
            p_tmp_xslisting->alias = tmp_xml_attribute.as_string();

        if(!(tmp_xml_attribute = tmp_node.attribute("zaid")).empty())
            p_tmp_xslisting->zaid = tmp_xml_attribute.as_int();

        p_tmp_xslisting->awr = tmp_node.attribute("awr").as_double();

        if(!(tmp_xml_attribute = tmp_node.attribute("temperature")).empty())
            p_tmp_xslisting->kT = tmp_xml_attribute.as_double();

        p_tmp_xslisting->location = tmp_node.attribute("location").as_int();

//        std::cout << p_tmp_xslisting->name.rfind("c") << p_tmp_xslisting->name.length()-1 << std::endl;
        if(p_tmp_xslisting->name.rfind("c") == p_tmp_xslisting->name.length()-1){
            p_tmp_xslisting->type = global::ACE_NEUTRON;
        }else{
            p_tmp_xslisting->type = global::ACE_THERMAL;
        }

        if(!(tmp_xml_attribute = tmp_node.attribute("filetype")).empty()){
            if(tmp_xml_attribute.as_string() == "binary"){

                p_tmp_xslisting->filetype = global::BINARY;
            }
            else{
                p_tmp_xslisting->filetype = global::ASCII
            }
        }else{
            p_tmp_xslisting->filetype = filetype;
        }
//        std::cout << "    name:"<< p_tmp_xslisting->name << "    alias:"<< p_tmp_xslisting->alias
//                  << "    zaid:"<< p_tmp_xslisting->zaid << "    awr:"<< p_tmp_xslisting->awr
//                  << "    kT:"<< p_tmp_xslisting->kT << "    location:"<< p_tmp_xslisting->location
//                  << "    type:"<< p_tmp_xslisting->type << std::endl;
    }

}
