QT -= core gui
QMAKE_CFLAGS += -std=gnu11

TEMPLATE = lib

INCLUDEPATH += E:\GitProject\vlc-2.2.4\include \
    E:\GitProject\vlc-2.2.4\include\vlc \
    E:\GitProject\vlc-2.2.4\modules\demux \
    E:\VisualStudio\live555-master\live555\BasicUsageEnvironment\include \
    E:\VisualStudio\live555-master\live555\liveMedia\include \
    E:\GitProject\vlc-backup\modules\demux\avformat \
    E:\GitProject\vlc-backup\contrib\i686-w64-mingw32\include \
    E:\GitProject\vlc-backup\modules\codec\avcodec
#    E:\Libraries\ffmpeg-3.0.1-win32-shared\include

DEFINES += HAVE_LLDIV HAVE_GETENV HAVE_SWAB HAVE_STRUCT_TIMESPEC HAVE_MAX_ALIGN_T MODULE_NAME=2_2_0b LIBVLC_USE_PTHREAD_CANCEL

DEFINES += ENABLE_SOUT HAVE_LIBAVUTIL_AVUTIL_H HAVE_LIBAVFORMAT_AVFORMAT_H HAVE_LIBAVCODEC_AVCODEC_H FF_API_PIX_FMT

LIBS += -lvlc -lvlccore -lWs2_32

LIBS += E:\Libraries\ffmpeg-3.0.1-win32-shared\lib\libavformat.dll.a
LIBS += E:\Libraries\ffmpeg-3.0.1-win32-shared\lib\libavutil.dll.a
LIBS += E:\Libraries\ffmpeg-3.0.1-win32-shared\lib\libavcodec.dll.a

HEADERS += \
    avformat.h

SOURCES += \
    avformat.c \
    demux.c \
    mux.c \
    chroma.c \
    fourcc.c

QMAKE_CXXFLAGS += -fpermissive
