QT += network sql
CONFIG += create_pc create_prl

TEMPLATE = lib
TARGET = saesu
DEPENDPATH += . src src/bonjour
INCLUDEPATH += src src/bonjour

MOC_DIR = ./.moc/
OBJECTS_DIR = ./.obj/

PRIVATE_HEADERS =

# Input
HEADERS += src/sglobal.h \
           src/sobject.h \
           src/sobjectmanager.h \
           src/sobjectid.h \
           src/rpc/srpcpeer.h \
           src/rpc/srpcconnection.h \
           src/rpc/srpcservice.h \
           src/rpc/sipclistener.h

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
           src/bonjour/bonjourserviceresolver.cpp \
           src/rpc/srpcpeer.cpp \
           src/rpc/srpcpeer_p.cpp \
           src/rpc/srpcconnection.cpp \
           src/rpc/srpcconnection_p.cpp \
           src/rpc/srpcservice.cpp \
           src/rpc/srpcservice_p.cpp \
           src/rpc/sipclistener.cpp \
           src/rpc/sipclistener_p.cpp \
           src/rpc/srpcsocket_p.cpp

# private headers
HEADERS += \
           src/sobjectmanager_p.h \
           src/sobject_p.h \
           src/sobjectid_p.h \
           src/bonjour/bonjourrecord.h \
           src/bonjour/bonjourservicebrowser.h \
           src/bonjour/bonjourserviceregister.h \
           src/bonjour/bonjourserviceresolver.h \
           src/rpc/srpcpeer_p.h \
           src/rpc/srpcconnection_p.h \
           src/rpc/srpcservice_p.h \
           src/rpc/sipclistener_p.h \
           src/rpc/srpcsocket_p.h

target.path = /usr/lib

QMAKE_PKGCONFIG_NAME = saesu
QMAKE_PKGCONFIG_DESCRIPTION = Generic library of useful magic.
QMAKE_PKGCONFIG_LIBDIR = $$target.path
QMAKE_PKGCONFIG_INCDIR = $$headers.path
QMAKE_PKGCONFIG_DESTDIR = pkgconfig
pkgconfig.files = $${TARGET}.pc
pkgconfig.path = /usr/lib/pkgconfig

INSTALLS += target headers
