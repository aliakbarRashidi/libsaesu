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
           src/sipcchannel.h \
           src/sobject.h \
           src/sobjectmanager.h \
           src/sobjectid.h \
           src/speermodel.h

include(src/src.pri)

headers.path = /usr/include/saesu
headers.files = $$HEADERS

HEADERS += $$PRIVATE_HEADERS

SOURCES += src/sipcchannel.cpp \
           src/sipcchannel_p.cpp \
           src/sipcserver.cpp \
           src/sipcconnection.cpp \
           src/sobject.cpp \
           src/sobject_p.cpp \
           src/sobjectmanager.cpp \
           src/sobjectid.cpp \
           src/sobjectid_p.cpp \
           src/speermodel.cpp \
           src/speermodel_p.cpp

# private headers
HEADERS += src/sipcchannel_p.h \
           src/sipcserver_p.h \
           src/sipcconnection_p.h \
           src/sobjectmanager_p.h \
           src/sobject_p.h \
           src/sobjectid_p.h \
           src/speermodel_p.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc
pkgconfig.path = /usr/lib/pkgconfig

INSTALLS += target headers
