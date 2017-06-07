
TEMPLATE = subdirs

SUBDIRS += \
    rtp\libstream_out_rtp_plugin.pro \
    ts\libmux_ts_plugin.pro \
    ps\libmux_ps_plugin.pro \
    psdemux\libdemux_ps_plugin.pro \
    rtsp_svr\libstream_out_rtsp_svr_plugin.pro \
    avformat\libavformat_plugin.pro
