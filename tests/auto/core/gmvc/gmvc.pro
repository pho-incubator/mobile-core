CONFIG += testcase parallel_test
TARGET = tst_gmvc
osx:CONFIG -= app_bundle

QT += mvc mvc-private testlib
SOURCES += \
    tst_gmvc.cpp
