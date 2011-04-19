TEMPLATE = app

# FIXME: this is a horrid hack
INCLUDEPATH= ../../src ../../src/filters ../../src/requests

LIBS += -L../.. -lsaesu

QT += testlib

check.depends = $$TARGET
check.commands += @LD_LIBRARY_PATH=../..
check.commands += ./$$TARGET
QMAKE_EXTRA_TARGETS += check
