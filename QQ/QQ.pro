#-------------------------------------------------
#
# Project created by QtCreator 2020-09-29T15:30:08
#
#-------------------------------------------------

QT       += core gui network testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QQ
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mydialog.cpp \
    qq.cpp \
    mycombobox.cpp \
    chat.cpp \
    send.cpp \
    receive.cpp \
    addfriend.cpp \
    lrc.cpp \
    emoji.cpp \
    rightmenu.cpp \
    personally.cpp \
    keyboard.cpp \
    mypushbutton.cpp \
    keyboradl.cpp \
    isadd.cpp

HEADERS  += mainwindow.h \
    mydialog.h \
    qq.h \
    mycombobox.h \
    chat.h \
    send.h \
    receive.h \
    addfriend.h \
    lrc.h \
    emoji.h \
    rightmenu.h \
    personally.h \
    keyboard.h \
    mypushbutton.h \
    keyboradl.h \
    isadd.h

FORMS    += mainwindow.ui \
    qq.ui \
    chat.ui \
    send.ui \
    receive.ui \
    addfriend.ui \
    lrc.ui \
    emoji.ui \
    rightmenu.ui \
    personally.ui \
    keyboard.ui \
    keyboradl.ui \
    isadd.ui

RESOURCES += \
    headpic/headpic.qrc \
    backpic/backpic.qrc \
    emoji/emoji.qrc
