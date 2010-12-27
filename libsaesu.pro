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
           include/sipcchannel.h \
           include/scloudstorage.h \
           include/scloudtablemodel.h

headers.path = /usr/include/saesu
headers.files = $$HEADERS

SOURCES += src/sipcchannel.cpp \
           src/sipcchannel_p.cpp \
           src/sipcserver.cpp \
           src/sipcconnection.cpp \
           src/scloudstorage.cpp \
           src/scloudstorage_p.cpp \
    src/scloudtablemodel.cpp \
    src/scloudtablemodel_p.cpp

# private headers
HEADERS += include/sipcchannel_p.h \
           include/sipcserver_p.h \
           include/sipcconnection_p.h \
           include/scloudstorage_p.h \
    include/scloudtablemodel_p.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
#QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc

INSTALLS += target headers
