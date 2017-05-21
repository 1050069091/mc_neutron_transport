#include "input_xml.h"
#include "global.h"
#include "xslisting.h"
#include "pugixml.hpp"
#include "iostream"
#include "nuclide.h"

void input_xml::read_xml(){

    input_xml::read_cross_sections_xml();

    input_xml::read_materials_xml();

    input_xml::read_source_xml();

//    map<string,nuclide>::iterator it;

//    for(it=global::gmaterials->nuclides->begin();it!=global::gmaterials->nuclides->end();it++)
//        std::cout << it->first << std::endl ;

}

void input_xml::read_source_xml()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    pugi::xml_node source_xml_node,tmp_xml;

    result = doc.load_file("source.xml");

    if(!result)
    {
        std::cout << "read source.xm failed\n";
        exit(-1);
    }
    source_xml_node = doc.child("source");//default_xs

    global::gsource->particle_num = source_xml_node.child("particle_num").attribute("count").as_int();

    global::now_particle_num = global::gsource->particle_num;

    global::gsource->max_angle = source_xml_node.child("direction").attribute("max_angle").as_double();

    tmp_xml = source_xml_node.child("default_engery");
    global::gsource->max_engery = tmp_xml.attribute("max").as_double();
    global::gsource->min_engery = tmp_xml.attribute("min").as_double();

    tmp_xml = source_xml_node.child("location");
    global::gsource->site_xyz[0] = tmp_xml.attribute("x").as_double();
    global::gsource->site_xyz[1] = tmp_xml.attribute("y").as_double();
    global::gsource->site_xyz[2] = tmp_xml.attribute("z").as_double();

    tmp_xml = source_xml_node.child("max_recur");//max_recur
    global::gsource->max_recur = tmp_xml.attribute("value").as_int();

//    std::cout << global::gsource->particle_num << "\t"
//                 << global::gsource->max_angle << "\t"
//                    << global::gsource->max_engery << "\t"
//                       << global::gsource->min_engery << "\t"
//                          << global::gsource->site_xyz[0] << "\t"
//                             << global::gsource->site_xyz[1] << "\t"
//                                << global::gsource->site_xyz[2] << "\n";


}

void input_xml::read_materials_xml()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    pugi::xml_node materials_xml_node,tmp_xml_node;
    pugi::xml_attribute tmp_xml_attribute;

    nuclide *tmp_nuclide ;

    string default_xs,tmp_xs;

    result = doc.load_file("materials.xml");

    if(!result)
    {
        std::cout << "read materials.xm failed\n";
        exit(-1);
    }

    materials_xml_node = doc.child("materials");//default_xs

    tmp_xml_node = materials_xml_node.child("default_xs");
    default_xs = tmp_xml_node.child_value();

    tmp_xml_node = materials_xml_node.child("geometry");
    global::gmaterials->geometry_x = tmp_xml_node.attribute("x").as_double();
    global::gmaterials->geometry_y = tmp_xml_node.attribute("y").as_double();
    global::gmaterials->geometry_z = tmp_xml_node.attribute("z").as_double();

//    std::cout << global::gmaterials->geometry_x << default_xs << std::endl;

    tmp_xml_node = materials_xml_node.child("density");
    global::gmaterials->density = tmp_xml_node.attribute("value").as_double();

    for (pugi::xml_node tmp_node: materials_xml_node.children("nuclide"))
    {
        tmp_nuclide = new nuclide();
        tmp_nuclide->name = tmp_node.attribute("name").as_string();
        tmp_nuclide->awr = tmp_node.attribute("ao").as_double();
        if(!(tmp_xml_attribute=tmp_node.attribute("xs")).empty())
            tmp_xs = tmp_xml_attribute.as_string();
        else
            tmp_xs = default_xs;
        tmp_nuclide->key_in_xsmap = tmp_nuclide->name+"."+tmp_xs;

        (*global::gmaterials->nuclides)[tmp_nuclide->key_in_xsmap] = *tmp_nuclide;
    }

}

void input_xml::read_cross_sections_xml()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result;
    pugi::xml_node cross_sections_xml_node,tmp_xml_node;
    pugi::xml_attribute tmp_xml_attribute;

    int filetype = global::ASCII;
    int recl,entries;
    string filetype_str,tmp_key_str;

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
                p_tmp_xslisting->filetype = global::ASCII;
            }
        }else{
            p_tmp_xslisting->filetype = filetype;
        }

        if(!(tmp_xml_attribute = tmp_node.attribute("metastable")).empty())
            p_tmp_xslisting->metastable = true;

        p_tmp_xslisting->path = tmp_node.attribute("path").as_string();

        if(p_tmp_xslisting->filetype == global::BINARY)
        {
            p_tmp_xslisting->recl = recl;
            p_tmp_xslisting->entries = entries;
        }

        tmp_key_str = p_tmp_xslisting->alias;
        if(tmp_key_str == "NULL")
            tmp_key_str = p_tmp_xslisting->name;

        (*global::xsliistings)[tmp_key_str] = *p_tmp_xslisting;

//        std::cout << "    name:"<< p_tmp_xslisting->name << "    alias:"<< p_tmp_xslisting->alias
//                  << "    zaid:"<< p_tmp_xslisting->zaid << "    awr:"<< p_tmp_xslisting->awr
//                  << "    kT:"<< p_tmp_xslisting->kT << "    location:"<< p_tmp_xslisting->location
//                  << "    type:"<< p_tmp_xslisting->type << "    filetype:"<< p_tmp_xslisting->filetype
//                  << std::endl;
    }

}
