TEMPLATE = app
TARGET = H4KvT
VERSION = 1.0

QT += concurrent widgets
CONFIG += c++11
DEFINES += APP_NAME=\\\"$$TARGET\\\" APP_VERSION=\\\"$$VERSION\\\"

SOURCES += $$files(src/*.cpp, true)
HEADERS += $$files(src/*.h, true) $$files(src/*.hpp, true)

!win32 {
	MOC_DIR = tmp
	OBJECTS_DIR = tmp
	RCC_DIR = tmp
}

macx:ICON = img/icon.icns
win32:RC_ICONS = img/icon.ico

RESOURCES += H4KvT.qrc
TRANSLATIONS += $$files(qtr/H4KvT_*.ts)

DISTFILES += LICENSE README* img/* qtr/qt_*.ts qtr/*.qm tst/*

static {
	RESOURCES += qtr/qt.qrc
	DEFINES += QTR_PATH=\\\":/\\\"
} else {
	DISTFILES += qtr/qt.qrc
}

run.depends = $$TARGET
run.commands = $$PWD/$$TARGET
QMAKE_EXTRA_TARGETS += run

qtr.depends = $$SOURCES
qtr.commands = lupdate $$_PRO_FILE_ -locations none && lrelease $$_PRO_FILE_
QMAKE_EXTRA_TARGETS += qtr

QMAKE_TARGET_COPYRIGHT = GPLv3+
QMAKE_TARGET_DESCRIPTION = \43\43\43\43\43

QMAKE_DISTCLEAN += -r tmp debug release object_script.H4KvT.* H4KvT_resource.rc

