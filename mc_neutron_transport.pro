TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    pugixml.cpp \
    ace.cpp \
    xslisting.cpp

DISTFILES +=

HEADERS += \
    pugiconfig.hpp \
    pugixml.hpp \
    ace.h \
    xslisting.h \
    global.h
