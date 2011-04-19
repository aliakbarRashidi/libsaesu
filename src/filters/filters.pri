INCLUDEPATH += src/filters

HEADERS += src/filters/sabstractobjectfilter.h \
           src/filters/sobjectidfilter.h \
           src/filters/sobjectdetailfilter.h

PRIVATE_HEADERS += src/filters/sabstractobjectfilter_p.h \
                   src/filters/sobjectidfilter_p.h \
                   src/filters/sobjectdetailfilter_p.h

SOURCES += src/filters/sabstractobjectfilter.cpp \
           src/filters/sabstractobjectfilter_p.cpp \
           src/filters/sobjectidfilter.cpp \
           src/filters/sobjectidfilter_p.cpp \
           src/filters/sobjectdetailfilter.cpp \
           src/filters/sobjectdetailfilter_p.cpp
