include(def_test.pro.inc)

SRC_DIR = ../src/$$TEST

QMAKE_INCDIR += $$SRC_DIR

SOURCES += $$files($$SRC_DIR/*.cpp)
HEADERS += $$files($$SRC_DIR/*.h) $$files($$SRC_DIR/*.hpp)

