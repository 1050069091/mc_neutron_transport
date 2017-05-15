#include <iostream>
#include "pugixml.hpp"
#include "global.h"

using namespace std;

int main(int argc, char *argv[])
{

    global::xsliistings = new map<string,xslisting>();
    string str = "wujiahao";
    xslisting tmp_xslisting;
    global::xsliistings->insert(str,tmp_xslisting);
//    pugi::xml_document doc;

//       pugi::xml_parse_result result = doc.load_file("cross_sections.xml");

//       if(!result)
//       {
//           std::cout << "read cross_sections.xm failed\n";
//           return 1;

//       }

//       pugi::xml_node cross_sections = doc.child("cross_sections");
//       int num = 0;

//       for (pugi::xml_node tool: cross_sections.children("ace_table"))
//       {
//   //        std::cout << "ace_table:";
//   //
//   //        for (pugi::xml_attribute attr: tool.attributes())
//   //        {
//   //            std::cout << " " << attr.name() << "=" << attr.value();
//   //        }

//   //        for (pugi::xml_node child: tool.children())
//   //        {
//   //            std::cout << ", child " << child.name();
//   //        }
//           num ++;

////           std::cout << std::endl;
//       }
//       std::cout << num << std::endl;

    return 0;
}
