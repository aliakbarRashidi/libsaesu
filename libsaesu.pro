QT += network sql
CONFIG += create_pc create_prl

TEMPLATE = lib
TARGET = saesu
DEPENDPATH += . src
INCLUDEPATH += src

MOC_DIR = ./.moc/
OBJECTS_DIR = ./.obj/

PRIVATE_HEADERS =

# Input
HEADERS += src/sglobal.h \
           src/sobject.h \
           src/sobjectmanager.h \
           src/sobjectid.h

include(src/src.pri)

headers.path = /usr/include/saesu
headers.files = $$HEADERS

HEADERS += $$PRIVATE_HEADERS

SOURCES += \
           src/sobject.cpp \
           src/sobject_p.cpp \
           src/sobjectmanager.cpp \
           src/sobjectid.cpp \
           src/sobjectid_p.cpp \
           src/bonjour/bonjourserviceregister.cpp \
           src/bonjour/bonjourservicebrowser.cpp \
           src/bonjour/bonjourserviceresolver.cpp


# private headers
HEADERS += \
           src/sobjectmanager_p.h \
           src/sobject_p.h \
           src/sobjectid_p.h \
           src/bonjour/bonjourrecord.h \
           src/bonjour/bonjourservicebrowser.h \
           src/bonjour/bonjourserviceregister.h \
           src/bonjour/bonjourserviceresolver.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc
pkgconfig.path = /usr/lib/pkgconfig

INSTALLS += target headers
