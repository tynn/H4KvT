TEMPLATE = app
TARGET = tmp/$${TEST}_test

QT -= core gui
CONFIG += c++11 testcase

OBJECTS_DIR = tmp
SOURCES += $${TEST}_test.cpp

