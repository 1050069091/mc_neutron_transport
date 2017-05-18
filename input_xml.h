#ifndef INPUT_XML_H
#define INPUT_XML_H


class input_xml
{
public:
    input_xml();
    static void read_xml();
    static void read_cross_sections_xml();
    static void read_materials_xml();
    static void read_source_xml();
};

#endif // INPUT_XML_H
