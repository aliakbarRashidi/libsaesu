QT += network sql
CONFIG += create_pc create_prl

TEMPLATE = lib
TARGET = saesu
DEPENDPATH += . src

MOC_DIR = ./.moc/
OBJECTS_DIR = ./.obj/

# Input
HEADERS += src/sglobal.h \
           src/sipcchannel.h \
           src/sobject.h \
           src/sabstractobjectrequest.h \
           src/sobjectfetchrequest.h \
           src/sobjectsaverequest.h \
           src/sobjectmanager.h \
           src/sabstractobjectfilter.h \
           src/sobjectidfilter.h \
           src/sobjectdetailfilter.h

headers.path = /usr/include/saesu
headers.files = $$HEADERS

SOURCES += src/sipcchannel.cpp \
           src/sipcchannel_p.cpp \
           src/sipcserver.cpp \
           src/sipcconnection.cpp \
           src/sobject.cpp \
           src/sobject_p.cpp \
           src/sabstractobjectrequest.cpp \
           src/sabstractobjectrequest_p.cpp \
           src/sobjectfetchrequest.cpp \
           src/sobjectfetchrequest_p.cpp \
           src/sobjectsaverequest.cpp \
           src/sobjectsaverequest_p.cpp \
           src/sobjectmanager.cpp \
           src/sabstractobjectfilter.cpp \
           src/sabstractobjectfilter_p.cpp \
           src/sobjectidfilter.cpp \
           src/sobjectidfilter_p.cpp \
           src/sobjectdetailfilter.cpp \
           src/sobjectdetailfilter_p.cpp

# private headers
HEADERS += src/sipcchannel_p.h \
           src/sipcserver_p.h \
           src/sipcconnection_p.h \
           src/sabstractobjectrequest_p.h \
           src/sobjectsaverequest_p.h \
           src/sobjectfetchrequest_p.h \
           src/sabstractobjectfilter_p.h \
           src/sobjectidfilter_p.h \
           src/sobjectdetailfilter_p.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc
pkgconfig.path = /usr/lib/pkgconfig

INSTALLS += target headers
