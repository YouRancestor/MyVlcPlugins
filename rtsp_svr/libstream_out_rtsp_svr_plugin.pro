QT -= core gui
QMAKE_CFLAGS += -std=gnu11

TEMPLATE = lib

INCLUDEPATH += E:\GitProject\vlc-2.2.4\include \
    E:\GitProject\vlc-2.2.4\include\vlc \
    E:\GitProject\vlc-2.2.4\modules\demux

DEFINES += HAVE_LLDIV HAVE_GETENV HAVE_SWAB HAVE_STRUCT_TIMESPEC HAVE_MAX_ALIGN_T MODULE_NAME=2_2_0b LIBVLC_USE_PTHREAD_CANCEL

LIBS += -lvlccore -lWs2_32

HEADERS += \
    input_interface.h \
    KeyValueList.h \
    rtp.h \
    vlm_internal.h \
    xiph.h

SOURCES += \
    KeyValueList.cpp \
    myrtp.c \
    myrtsp.c \
    rtcp.c \
    rtpfmt.c \
    vod.c \
    gmtime_r.c
