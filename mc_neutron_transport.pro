TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    pugixml.cpp \
    ace.cpp \
    xslisting.cpp \
    global.cpp \
    input_xml.cpp \
    materials.cpp \
    nuclide.cpp

DISTFILES += \
    materials.xml

HEADERS += \
    pugiconfig.hpp \
    pugixml.hpp \
    ace.h \
    xslisting.h \
    global.h \
    input_xml.h \
    materials.h \
    nuclide.h
