QT += network
CONFIG += create_pc create_prl

QT -= gui
TEMPLATE = lib
TARGET = saesu
DEPENDPATH += . include src
INCLUDEPATH += . include

MOC_DIR = ./.moc/
OBJECTS_DIR = ./.obj/

# Input
HEADERS += include/sglobal.h \
           include/qtipcchannel.h

headers.path = /usr/include/saesu
headers.files = $$HEADERS

SOURCES += src/qtipcchannel.cpp \
           src/qtipcchannel_p.cpp \
           src/qtipcserver.cpp \
           src/qtipcconnection.cpp

# private headers
HEADERS += include/qtipcchannel_p.h \
           include/qtipcserver_p.h \
           include/qtipcconnection_p.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
#QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc

INSTALLS += target headers
