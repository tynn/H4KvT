TEMPLATE = app
TARGET = H4KvT
VERSION = 0.1

QT += concurrent widgets
CONFIG += c++11

MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp

macx:ICON = img/icon.icns
win32:RC_ICONS = img/icon.ico

DISTFILES += LICENSE README* img/* qtr/*.qm tst/*

RESOURCES += default.qrc
TRANSLATIONS += $$files(qtr/default_*.ts)

SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true) $$files(src/*.hpp, true)

QMAKE_DISTCLEAN += -r tmp

run.depends = $$TARGET
run.commands = $$PWD/$$TARGET
QMAKE_EXTRA_TARGETS += run

