QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    ../enshu3/colordetective.cpp \
    ../enshu3/cv.cpp \
    ../enshu3/form.cpp \
    ../enshu3/gamemode.cpp \
    ../enshu3/start.cpp \
    ../enshu3/target.cpp \
    ../enshu3/targeth.cpp \
    camerathread.cpp \
    imageaccess.cpp \
    main.cpp \
    mainwindow.cpp \
    qclickedlabel.cpp \
    stackedpoint.cpp

HEADERS += \
    ../enshu3/colordetective.h \
    ../enshu3/cv.h \
    ../enshu3/form.h \
    ../enshu3/gamemode.h \
    ../enshu3/start.h \
    ../enshu3/target.h \
    ../enshu3/targeth.h \
    camerathread.h \
    imageaccess.h \
    mainwindow.h \
    qclickedlabel.h \
    stackedpoint.h

FORMS += \
    ../enshu3/colordetective.ui \
    ../enshu3/form.ui \
    ../enshu3/gamemode.ui \
    ../enshu3/start.ui \
    ../enshu3/target.ui \
    ../enshu3/targeth.ui \
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

DISTFILES += \
    ../enshu3/data.png \
    ../enshu3/enshu3.pro.user.4540fc4 \
    ../enshu3/logo-1.png \
    ../enshu3/logo.png \
    ../enshu3/tantei.png \
    logo-1.png \
    logo.png \
    tantei.png

SUBDIRS += \
    ../enshu3/enshu3.pro \
    ../enshu3/enshu3.pro
