QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEPENDPATH += /usr/include
INCLUDEPATH += /usr/include
LIBS += -L/usr/include/ \
     -lopencv_core \
     -lopencv_imgproc \
     -lopencv_highgui \
     -lopencv_videoio \
     -lopencv_imgcodecs\

SOURCES += \
    camerathread.cpp \
    imageaccess.cpp \
    main.cpp \
    mainwindow.cpp \
    qclickedlabel.cpp \
    stackedpoint.cpp

HEADERS += \
    camerathread.h \
    imageaccess.h \
    mainwindow.h \
    qclickedlabel.h \
    stackedpoint.h

FORMS += \
    mainwindow.ui

#destination folder
CONFIG(debug,debug|release){
    BUILD_TYPE=debug
}else{
    BUILD_TYPE=release
}
DESTDIR = $$OUT_PWD/deploy/$$BUILD_TYPE

#target binaly pass
win32 {
    TARGET_NAME = $${TARGET}.exe
}
else: macx {
    TARGET_NAME = $${TARGET}.app
}
else: linux {
    TARGET_NAME = $${TARGET}
}
TARGET_PATH = $$DESTDIR/$${TARGET_NAME}

#register for post-link processing
win32 {
    QMAKE_POST_LINK =  $$dirname(QMAKE_QMAKE)/windeployqt $${TARGET_PATH} -qmldir=$$PWD --no-translations$$escape_expand(\n\t)
    QMAKE_POST_LINK += cd$$escape_expand(\n\t)
}
else: macx {
    QMAKE_POST_LINK =  $$dirname(QMAKE_QMAKE)/macdeployqt $${TARGET_PATH} -qmldir=$$PWD$$escape_expand(\n\t)
    QMAKE_POST_LINK += pwd$$escape_expand(\n\t)
}
else: linux {
    QMAKE_POST_LINK =  ~/linuxdeployqt-7-x86_64.AppImage $${TARGET_PATH} -qmldir=$$PWD$$escape_expand -unsupported-allow-new-glibc$$escape_expand(\n\t)
    QMAKE_POST_LINK += pwd$$escape_expand(\n\t)
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
