#include"MediaInfoDLL/MediaInfoDLL.h"
extern "C"{
#include"getwh.h"
}
void getWH(const char*filename,int*width,int *height)
{
    MediaInfoDLL::MediaInfo MI;
    MediaInfoDLL::String w,h;
    MI.Open(__T(filename));
    w = MI.Get(MediaInfoDLL::stream_t::Stream_Video,0,__T("Width"));
    h = MI.Get(MediaInfoDLL::stream_t::Stream_Video,0,__T("Height"));
    *width =atoi(w.c_str());
    *height=atoi(h.c_str());
    return ;
}
