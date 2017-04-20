QT -= core gui
QMAKE_CFLAGS += -std=gnu11

TEMPLATE = lib

INCLUDEPATH += E:\GitProject\vlc-2.2.4\include \
    E:\GitProject\vlc-2.2.4\include\vlc \
    E:\GitProject\vlc-2.2.4\modules\demux

DEFINES += HAVE_LLDIV HAVE_GETENV HAVE_SWAB HAVE_STRUCT_TIMESPEC HAVE_POLL HAVE_MAX_ALIGN_T MODULE_NAME=2_2_0b

LIBS += -lvlc -lvlccore -lWs2_32

HEADERS += \
    rtp.h

SOURCES += \
    rtcp.c \
    rtp.c \
    rtpfmt.c \
    rtsp.c \
    gmtime_r.c \
    vod.c

