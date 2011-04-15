QT += network
CONFIG += create_pc create_prl

TEMPLATE = lib
TARGET = saesu
DEPENDPATH += . src

MOC_DIR = ./.moc/
OBJECTS_DIR = ./.obj/

# Input
HEADERS += src/sglobal.h \
           src/sipcchannel.h \
           src/sobject.h

headers.path = /usr/include/saesu
headers.files = $$HEADERS

SOURCES += src/sipcchannel.cpp \
           src/sipcchannel_p.cpp \
           src/sipcserver.cpp \
           src/sipcconnection.cpp \
           src/sobject.cpp \
           src/sobject_p.cpp

# private headers
HEADERS += src/sipcchannel_p.h \
           src/sipcserver_p.h \
           src/sipcconnection_p.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc
pkgconfig.path = /usr/lib/pkgconfig

INSTALLS += target headers
