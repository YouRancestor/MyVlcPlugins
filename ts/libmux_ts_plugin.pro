#-------------------------------------------------
#
# Project created by QtCreator 2017-04-19T16:44:43
#
#-------------------------------------------------

QT       -= core gui

TARGET = libmux_ts_plugin
TEMPLATE = lib

INCLUDEPATH += E:\GitProject\vlc-2.2.4\include \
    E:\GitProject\vlc-2.2.4\include\vlc \
    E:\GitProject\vlc-backup\contrib\i686-w64-mingw32\include \

DEFINES += MODULE_NAME=2_2_0b HAVE_LLDIV HAVE_GETENV HAVE_SWAB LIBVLC_USE_PTHREAD_CANCEL

SOURCES += \
    ts.c \
    csa.c \
    pes.c \
    nrand48.c

HEADERS += \
    csa.h \
    pes.h \
    dvbpsi_compat.h \
    bits.h

LIBS += -lvlc -lvlccore -ldvbpsi

unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_CFLAGS += -std=c11
