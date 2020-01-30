ANDROID_BUNDLED_JAR_DEPENDENCIES = \
    jar/GroupsIncAndroidMvc-bundled.jar
ANDROID_JAR_DEPENDENCIES = \
    jar/GroupsIncAndroidMvc.jar

INCLUDEPATH += $$PWD

MAJOR_VERSION = 1
MINOR_VERSION = 0

DEFINES += \
    MAJOR_VERSION=$$MAJOR_VERSION \
    MINOR_VERSION=$$MINOR_VERSION

PUBLIC_HEADERS += \
    gmvc_global.h \
    gmvcfacade.h \
    gmvcmodel.h \
    gassetmodel.h \
    gscreenmodel.h \
    gapimodel.h

PRIVATE_HEADERS += \
    gassetmodel_p.h \
    gscreenmodel_p.h \
    gapimodel_p.h \
    gapimodel_format_p.h \
    gapimodel_format_json_p.h \
    gapimodel_format_jsonp_p.h \
    gapimodel_format_query_p.h

SOURCES += \
    gmvcfacade.cpp \
    gmvcmodel.cpp \
    gassetmodel.cpp \
    gscreenmodel_p.cpp \
    gscreenmodel.cpp \
    gapimodel_format_json_p.cpp \
    gapimodel_format_jsonp_p.cpp \
    gapimodel_format_query_p.cpp \
    gapimodel.cpp \
    gapimodel_p.cpp \
    gapimodel_format_p.cpp

android {
    QT += androidextras

    PRIVATE_HEADERS += \
        gscreenmodel_android_p.h

    SOURCES += \
        gscreenmodel_android_p.cpp

} else:ios {
    PRIVATE_HEADERS += \
        gscreenmodel_ios_p.h

    OBJECTIVE_SOURCES += \
        $$PWD/gscreenmodel_ios_p.mm

} else {
    PRIVATE_HEADERS += \
        gscreenmodel_default_p.h

    SOURCES += \
        gscreenmodel_default_p.cpp
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

