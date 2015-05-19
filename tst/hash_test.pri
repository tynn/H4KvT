include(def_test.pri)

defined(HASH, var):SRC_DIR = ../src/$$HASH
else:SRC_DIR = ../src/$$TEST

QMAKE_INCDIR += $$SRC_DIR

SOURCES += $$files($$SRC_DIR/*.cpp)
HEADERS += ../src/hashbuf.hpp $$files($$SRC_DIR/*.h) $$files($$SRC_DIR/*.hpp)

