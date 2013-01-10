TEMPLATE = app
TARGET = tst_mockhttpnetworkreply

QT += testlib
macx:CONFIG -= app_bundle

include(../../../mkspecs/test.pri)

HEADERS += tst_mockhttpnetworkreply.h
SOURCES += tst_mockhttpnetworkreply.cpp
