#-------------------------------------------------
#
# Project created by QtCreator 2017-05-11T10:37:47
#
#-------------------------------------------------

QT       -= gui

TARGET = bmiavformat
TEMPLATE = lib

DEFINES += BMIAVFORMAT_LIBRARY MODULE_NAME=2_2_0b

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    avformat.c \
    demux.c \
    mux.c \
    asprintf.c \
    fourcc.c \
    chroma.c

HEADERS += avformat.h \
    avcodec.h \
    avcommon.h \
    chroma.h \
    avcommon_compat.h

#Linux
#INCLUDEPATH += /home/liwei/vlc-2.2.0/include/
#INCLUDEPATH += /home/liwei/vlc-2.2.0/
#INCLUDEPATH += /home/liwei/vlc-2.2.0/include/vlc/
#INCLUDEPATH += /home/liwei/vlc-2.2.0/modules/codec/
#INCLUDEPATH += /home/liwei/vlc-2.2.0/modules/demux/


#Win
INCLUDEPATH += E:\GitProject\vlc-2.2.4\include\
INCLUDEPATH += E:\GitProject\vlc-2.2.4\modules\codec\

INCLUDEPATH += E:\GitProject\vlc-2.2.4\modules\demux\
INCLUDEPATH += E:\ffmpegSDK\ffmpeg-2.8.2\include\

DEFINES += HAVE_LIBAVFORMAT_AVFORMAT_H HAVE_LIBAVCODEC_AVCODEC_H HAVE_LIBAVUTIL_AVUTIL_H


LIBS += -L E:\ffmpegSDK\ffmpeg-2.8.2\lib  -lavformat  -lavutil   -lavcodec -lvlccore -lvlc


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DEFINES +=ENABLE_SOUT
