#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T16:44:43
#
#-------------------------------------------------

QT       -= core gui

TARGET = libmux_ps_plugin
TEMPLATE = lib

INCLUDEPATH += E:\GitProject\vlc-2.2.4\include \
    E:\GitProject\vlc-2.2.4\include\vlc \
    E:\GitProject\vlc-backup\contrib\i686-w64-mingw32\include \

DEFINES += MODULE_NAME=2_2_0b HAVE_LLDIV HAVE_GETENV HAVE_SWAB LIBVLC_USE_PTHREAD_CANCEL

SOURCES += \
    ps.c \
    pes.c

HEADERS += \
    pes.h \
    bits.h

LIBS += -lvlccore -ldvbpsi

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CFLAGS += -std=c11
