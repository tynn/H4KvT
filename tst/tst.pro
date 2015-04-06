TEMPLATE = subdirs
SUBDIRS = $$files(*_test.pro)
QMAKE_DISTCLEAN += -r tmp

