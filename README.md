# MyVlcPlugins
本项目为如何在VLC源码外构建VLC插件提供示例。  
项目中rtsp_svr模块完善了vlc自带的rtp模块（提供rtsp服务，支持udp承载rtsp和直播），实现了tcp承载rtsp和vod点播功能，并支持在Windows以及Linux上构建（推荐使用qmake）。  
其他模块功能不完整或有缺陷，且不保证能编译通过，仅供参考。  
