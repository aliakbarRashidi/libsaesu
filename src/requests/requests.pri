INCLUDEPATH += src/requests

HEADERS += src/requests/sabstractobjectrequest.h \
           src/requests/sobjectfetchrequest.h \
           src/requests/sobjectsaverequest.h \
           src/requests/sobjectremoverequest.h \
           src/requests/sdeletelistfetchrequest.h

PRIVATE_HEADERS += src/requests/sabstractobjectrequest_p.h \
                   src/requests/sobjectfetchrequest_p.h \
                   src/requests/sobjectsaverequest_p.h \
                   src/requests/sobjectremoverequest_p.h \
                   src/requests/sdeletelistfetchrequest_p.h

SOURCES += src/requests/sabstractobjectrequest.cpp \
           src/requests/sabstractobjectrequest_p.cpp \
           src/requests/sobjectsaverequest.cpp \
           src/requests/sobjectsaverequest_p.cpp \
           src/requests/sobjectfetchrequest.cpp \
           src/requests/sobjectfetchrequest_p.cpp \
           src/requests/sobjectremoverequest.cpp \
           src/requests/sobjectremoverequest_p.cpp \
           src/requests/sdeletelistfetchrequest.cpp \
           src/requests/sdeletelistfetchrequest_p.cpp
