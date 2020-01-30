CXX_MODULE = mvc
TARGET  = declarative_mvc
TARGETPATH = GroupsIncMvc
IMPORT_VERSION = 1.0

QT += qml quick mvc mvc-private

SOURCES += \
    $$PWD/mvc.cpp

load(qml_plugin)

OTHER_FILES += qmldir

