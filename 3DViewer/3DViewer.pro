linux-g++ | linux-g++-64 | linux-g++-32 | win32 {
QT += core gui widgets printsupport
LIBS += -lGLU
}
macx {
QT += core gui openglwidgets
LIBS +=-framework OpenGL
}

#QMAKE_CFLAGS -= -O2
#QMAKE_CFLAGS -= -O1
#QMAKE_CXXFLAGS -= -O2
#QMAKE_CXXFLAGS -= -O1

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17
CONFIG += debug_and_release
CONFIG += sdk_no_version_check

INCLUDEPATH += \
    objFileLib \
    view \
    controller \
    model \
    3rdparty 

SOURCES += \
    controller/controller.cpp \
    model/affine_ts.cpp \
    model/viewfieldgl.cpp \
    view/mainwindow.cpp \
    view/main.cpp

HEADERS += \
    controller/controller.h \
    model/affine_ts.h \
    model/viewfieldgl.h \
    view/mainwindow.h \
    3rdparty/gif.h

FORMS += \
    view/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
