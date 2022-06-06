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
    imageaccess.cpp \
    main.cpp \
    mainwindow.cpp \
    qclickablelabel.cpp \
    qclickedlabel.cpp

HEADERS += \
    imageaccess.h \
    mainwindow.h \
    qclickablelabel.h \
    qclickedlabel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
