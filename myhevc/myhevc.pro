#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T21:43:56
#
#-------------------------------------------------

QT       -= gui

TARGET = myhevc
TEMPLATE = lib

DEFINES += MYHEVC_LIBRARY MODULE_NAME=2_2_0b

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += hevc.c \
    getwh.cpp \
    getwh_sps.cpp \
    SPSParser.cpp

HEADERS += \
    mpeg_parser_helpers.h \
    getwh.h \
    getwh_sps.h \
    SPSParser.h
INCLUDEPATH += /home/liwei/vlc-2.2.0/include/
INCLUDEPATH += E:\ffmpegSDK\MediaInfo\Developers\Source

#LIBS += -lmediainfo -lvlccore -lvlc

unix {
    target.path = /usr/lib
    INSTALLS += target
}


DEFINES  -= UNICODE
