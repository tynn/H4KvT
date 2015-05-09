TEMPLATE = app
TARGET = tmp/$${TEST}_test.run

QT -= core gui
CONFIG += c++11 testcase

OBJECTS_DIR = tmp
SOURCES += $${TEST}_test.cpp

