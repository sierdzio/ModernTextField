QT = core gui widgets qml quick quickwidgets

unix {
    QMAKE_CXXFLAGS += -Werror
}

HEADERS = mainwindow.h \
    block.h \
    moderntextfield.h \
    moderntextfieldwidget.h \
    painter.h

SOURCES = main.cpp \
    block.cpp \
    mainwindow.cpp \
    moderntextfield.cpp \
    moderntextfieldwidget.cpp \
    painter.cpp

FORMS = mainwindow.ui

RESOURCES += \
    qml.qrc

CONFIG += qmltypes
QML_IMPORT_NAME = ModernFields
QML_IMPORT_MAJOR_VERSION = 1

OTHER_FILES = README.md LICENSE
